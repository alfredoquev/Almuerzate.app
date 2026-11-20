#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <stdexcept>
#include "clases.hpp"

using namespace std;

// ---------------------------------------------------------------------
// Macro de exportacion: Windows (.dll) necesita __declspec(dllexport);
// Linux (.so) exporta con visibility("default").
// ---------------------------------------------------------------------
#ifdef _WIN32
  #define API __declspec(dllexport)
#else
  #define API __attribute__((visibility("default")))
#endif

static const char *nombreDia(DiaSemana dia)
{
    switch (dia)
    {
    case DiaSemana::LUNES:
        return "Lunes";
    case DiaSemana::MARTES:
        return "Martes";
    case DiaSemana::MIERCOLES:
        return "Miercoles";
    case DiaSemana::JUEVES:
        return "Jueves";
    case DiaSemana::VIERNES:
        return "Viernes";
    }
    return "";
}

static const char *nombreEstado(EstadoConsulta estado)
{
    switch (estado)
    {
    case EstadoConsulta::DISPONIBLE:
        return "DISPONIBLE";
    case EstadoConsulta::NO_ASIGNADOS_AUN:
        return "NO_ASIGNADOS_AUN";
    case EstadoConsulta::SIN_PERIODO_VIGENTE:
        return "SIN_PERIODO_VIGENTE";
    case EstadoConsulta::PROBLEMA_DE_COMUNICACION:
        return "PROBLEMA_DE_COMUNICACION";
    }
    return "";
}

// ---------------------------------------------------------------------
// API visible desde PHP (FFI). Solo tipos de C: char*, int, etc.
// ---------------------------------------------------------------------
extern "C" {

// Ejecuta el script PHP indicado y devuelve su salida como cadena C.
// - rutaScript: ruta absoluta al script (la pasa PHP).
// - Devuelve NULL si hubo cualquier error.
// - La memoria devuelta DEBE liberarse con liberarCadena().
API char* imprimirJson(const char* rutaScript)
{
    if (!rutaScript) return nullptr;
    try {
        RepositorioAsignacionesPhp repositorio(rutaScript);
        string json = repositorio.leerJson();

        // Copia en memoria propia: el std::string se destruye al salir
        char* out = static_cast<char*>(malloc(json.size() + 1));
        if (!out) return nullptr;
        memcpy(out, json.c_str(), json.size() + 1);
        return out;
    } catch (...) {
        return nullptr;   // ninguna excepcion puede cruzar hacia PHP
    }
}

// Libera la memoria devuelta por imprimirJson() (mismo allocator que la creo)
API void liberarCadena(char* s)
{
    free(s);
}

} // extern "C"
