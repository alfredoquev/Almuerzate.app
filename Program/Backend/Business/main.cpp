#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <stdexcept>
#include "clases.hpp"
#include <iterator>

using namespace std;

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

// Ejecuta el script PHP y devuelve todo lo que imprime con echo

int main() {
    try {
        RepositorioAsignacionesPhp repositorio("query-fetcher.php");
        ConsultaAsignaciones consulta(repositorio);

        string prueba = consulta.imprimirJson();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}