<?php
declare(strict_types=1);

header('Content-Type: application/json; charset=utf-8');

// ---- Rutas (ajusta si tus carpetas son distintas) ----
$dirLib     = __DIR__ . '/../Backend/Business';
// .dll en Windows, .so en Linux/Docker
$rutaLib    = $dirLib . (PHP_OS_FAMILY === 'Windows' ? '/main.dll' : '/main.so');
// Ruta ABSOLUTA del script que C++ ejecuta con "php ..."
$rutaScript = realpath($dirLib . '/query-fetcher.php');

try {
    if ($rutaScript === false) {
        throw new RuntimeException('No se encontró query-fetcher.php');
    }

    $ffi = FFI::cdef("
        char* imprimirJson(const char* rutaScript);
        void  liberarCadena(char* s);
    ", $rutaLib);

    $ptr = $ffi->imprimirJson($rutaScript);
    if (FFI::isNull($ptr)) {
        throw new RuntimeException('imprimirJson() falló en la librería C++');
    }

    try {
        $result = FFI::string($ptr);
    } finally {
        $ffi->liberarCadena($ptr);   // liberar siempre, aunque algo falle
    }

    $decoded = json_decode($result, true, 512, JSON_THROW_ON_ERROR);

    // Si query-fetcher.php ya reporta un error propio, se propaga como error
    if (is_array($decoded) && isset($decoded['status']) && $decoded['status'] !== 'success') {
        throw new RuntimeException($decoded['message'] ?? 'Error en query-fetcher.php');
    }

    // Si lo que imprime query-fetcher.php ya viene envuelto como
    // {"status":..., "queryResult":...}, se toma solo el contenido interno
    if (is_array($decoded) && array_key_exists('queryResult', $decoded)) {
        $decoded = $decoded['queryResult'];
    }

    $response = [
        'status'      => 'success',
        'queryResult' => $decoded,
    ];
} catch (Throwable $e) {
    http_response_code(500);
    $response = ['status' => 'error', 'message' => $e->getMessage()];
}

echo json_encode($response, JSON_UNESCAPED_UNICODE);