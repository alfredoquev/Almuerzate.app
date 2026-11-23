# ADR-0007: Integrar PHP con el backend mediante una librería compartida de C++

Autores:
 - @julondono07

Fecha: 2026-11-22

## Estado

Aceptado

## Contexto

El ADR-0002 define PHP como conexión entre el frontend y el backend, e indica
que PHP invoca el ejecutable en C++ (árbol decisional). El backend se
ejecuta en el servidor y debe entregar a PHP los datos ya procesados.
Se requiere un mecanismo de invocación sencillo y acorde con el alcance
demostrativo del proyecto.

## Decisión

Vamos a compilar el backend en C++ como librería compartida (`.dll` en
Windows) y PHP la cargará mediante FFI.
Este ADR reemplaza el mecanismo de invocación del ADR-0002.

## Alternativas consideradas

- **Ejecutable invocado por PHP (ADR-0002):** Se descartó porque la
  implementación real no corresponde con exactitud a lo planteado en el
  ADR-0002: se había considerado que el sistema implementado era el
  descrito allí, pero PHP carga una librería compartida en lugar de
  invocar un ejecutable.
- **Emscripten:** Se descartó en el ADR-0002 porque requiere ejecutar el
  código C++ dentro del cliente, y el backend se ejecuta en el servidor.
  Podría volver a evaluarse como alternativa.

## Consecuencias

### Positivas (+)
- No se lanza un proceso del backend por cada solicitud.
- PHP recibe directamente la cadena JSON producida por el backend.

### Negativas / Desafíos (-)
- Requiere habilitar FFI en la configuración de PHP (`ffi.enable=true`).
- Hay que compilar una librería por sistema operativo.
- La firma de las funciones se declara en PHP y debe mantenerse
  sincronizada con el C++ (un desajuste puede tumbar el proceso).
- La memoria reservada por C++ debe liberarse manualmente.
