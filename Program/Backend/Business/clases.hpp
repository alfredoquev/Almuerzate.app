#pragma once

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>
#include <cstdio>   // popen / pclose / fgets / FILE

// popen/pclose se llaman _popen/_pclose en Windows (MSVC)
#ifdef _WIN32
  #define popen  _popen
  #define pclose _pclose
#endif

using namespace std;

// ---------------------------------------------------------------------
// Funciones auxiliares
// ---------------------------------------------------------------------

// Ejecuta el script PHP y devuelve todo lo que imprime con echo.
// rutaScript debe ser una ruta ABSOLUTA: el directorio de trabajo del
// proceso que carga la libreria (Apache, php-fpm, php CLI) no es fijo.
static string obtenerEchoPhp(const string &rutaScript)
{
    // Comillas para soportar rutas con espacios (Windows y Linux)
    string cmd = "php \"" + rutaScript + "\"";
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        throw runtime_error("No se pudo ejecutar php");

    string salida;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), pipe))
        salida += buffer;

    if (pclose(pipe) != 0)
        throw runtime_error("php terminó con error");
    return salida;
}

// ---------------------------------------------------------------------
// Tipos auxiliares (el diagrama los usa pero no los define)
// ---------------------------------------------------------------------
using IdEstudiante = string;

struct Fecha {
    int anio;
    int mes;
    int dia;
};

enum class DiaSemana { LUNES, MARTES, MIERCOLES, JUEVES, VIERNES };

// ---------------------------------------------------------------------
// EstadoConsulta <<enumeration>>
// ---------------------------------------------------------------------
enum class EstadoConsulta {
    DISPONIBLE,
    NO_ASIGNADOS_AUN,
    SIN_PERIODO_VIGENTE,
    PROBLEMA_DE_COMUNICACION
};

// ---------------------------------------------------------------------
// PeriodoAsignacion
// ---------------------------------------------------------------------
class PeriodoAsignacion {
public:
    PeriodoAsignacion(Fecha inicio, Fecha fin) : inicio(inicio), fin(fin) {}

    // Getters no incluidos en el diagrama; necesarios para que un
    // repositorio concreto pueda usar el periodo como criterio de busqueda.
    const Fecha& getInicio() const { return inicio; }
    const Fecha& getFin() const { return fin; }

private:
    Fecha inicio;
    Fecha fin;
};

// ---------------------------------------------------------------------
// ComedorAsignado
// ---------------------------------------------------------------------
class ComedorAsignado {
public:
    ComedorAsignado(DiaSemana dia, string nombreComedor)
        : dia(dia), nombreComedor(nombreComedor) {}

    // Getters no incluidos en el diagrama; necesarios para mostrar.
    DiaSemana getDia() const { return dia; }
    const string& getNombreComedor() const { return nombreComedor; }

private:
    DiaSemana dia;
    string nombreComedor;
};

// ---------------------------------------------------------------------
// RepositorioAsignaciones <<interface>>
// ---------------------------------------------------------------------
class RepositorioAsignaciones {
public:
    virtual ~RepositorioAsignaciones() = default;

    virtual optional<PeriodoAsignacion> buscarPeriodoVigente() = 0;

    // "0 o 5" ComedorAsignado -> vector vacio o con 5 elementos
    virtual vector<ComedorAsignado> buscarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const PeriodoAsignacion& periodo) = 0;

    virtual void guardarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const vector<ComedorAsignado>& comedores) = 0;

    virtual string leerJson() const { return ""; }
};

// ---------------------------------------------------------------------
// RepositorioAsignacionesPhp (enfocado en la conexion con los echo PHP)
// ---------------------------------------------------------------------

class RepositorioAsignacionesPhp : public RepositorioAsignaciones {
public:
    explicit RepositorioAsignacionesPhp(string rutaScript)
        : rutaScript(move(rutaScript)) {}

    optional<PeriodoAsignacion> buscarPeriodoVigente() override {
        string json = obtenerEchoPhp(rutaScript);   // dato actual
        // TODO: extraer inicio y fin de json; nullopt si no hay periodo
        return nullopt;
    }

    vector<ComedorAsignado> buscarComedoresAsignados(
            const IdEstudiante& idEstudiante,
            const PeriodoAsignacion& periodo) override {
        string json = obtenerEchoPhp(rutaScript);   // dato actual
        // TODO: parsear json y construir los ComedorAsignado del estudiante
        return {};
    }

    string leerJson() const override { return obtenerEchoPhp(rutaScript); }

    void guardarComedoresAsignados(
            const IdEstudiante&, const vector<ComedorAsignado>&) override {
        throw runtime_error("repositorio de solo lectura");
    }

private:
    string rutaScript;
};

// ---------------------------------------------------------------------
// ResultadoConsulta
// ---------------------------------------------------------------------
class ResultadoConsulta {
public:
    ResultadoConsulta(EstadoConsulta estado,
                      vector<ComedorAsignado> comedoresAsignados = {})
        : estado(estado), comedoresAsignados(move(comedoresAsignados)) {}

    bool estaDisponible() const { return estado == EstadoConsulta::DISPONIBLE; }

    EstadoConsulta obtenerEstado() const { return estado; }
    const vector<ComedorAsignado>& obtenerComedoresAsignados() const {
        return comedoresAsignados;
    }

private:
    EstadoConsulta estado;
    vector<ComedorAsignado> comedoresAsignados;
};

// ---------------------------------------------------------------------
// CatalogoMensajes
// ---------------------------------------------------------------------
class CatalogoMensajes {
public:
    string mensajePara(EstadoConsulta estado) const {
        switch (estado) {
            case EstadoConsulta::DISPONIBLE:
                return "Estos son tus comedores asignados.";
            case EstadoConsulta::NO_ASIGNADOS_AUN:
                return "Tus comedores aun no han sido asignados.";
            case EstadoConsulta::SIN_PERIODO_VIGENTE:
                return "No hay un periodo de asignacion vigente.";
            case EstadoConsulta::PROBLEMA_DE_COMUNICACION:
                return "Hubo un problema de comunicacion. Intenta nuevamente.";
        }
        return "";
    }
};

// ---------------------------------------------------------------------
// ConsultaAsignaciones
// ---------------------------------------------------------------------

class ConsultaAsignaciones {
public:
    explicit ConsultaAsignaciones(RepositorioAsignaciones& repositorio)
        : repositorio(repositorio) {}
        ResultadoConsulta consultarComedoresAsignados(const IdEstudiante& idEstudiante) {
            try {
                optional<PeriodoAsignacion> periodo = repositorio.buscarPeriodoVigente();
                if (!periodo) {
                    return ResultadoConsulta(EstadoConsulta::SIN_PERIODO_VIGENTE);
                }

                vector<ComedorAsignado> comedores =
                    repositorio.buscarComedoresAsignados(idEstudiante, *periodo);
                if (comedores.empty()) {
                    return ResultadoConsulta(EstadoConsulta::NO_ASIGNADOS_AUN);
                }

                return ResultadoConsulta(EstadoConsulta::DISPONIBLE, move(comedores));
            } catch (const exception&) {
                return ResultadoConsulta(EstadoConsulta::PROBLEMA_DE_COMUNICACION);
            }
        }

        // TEMPORAL: depuración
    void imprimirJson() const {
        cout << repositorio.leerJson() << endl;
    }

private:
    RepositorioAsignaciones& repositorio;
};
