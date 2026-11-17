#pragma once

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

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

    // "0..1" PeriodoAsignacion -> optional
    virtual optional<PeriodoAsignacion> buscarPeriodoVigente() = 0;

    // "0 o 5" ComedorAsignado -> vector vacio o con 5 elementos
    virtual vector<ComedorAsignado> buscarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const PeriodoAsignacion& periodo) = 0;

    virtual void guardarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const vector<ComedorAsignado>& comedores) = 0;
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

private:
    RepositorioAsignaciones& repositorio;
};

// ---------------------------------------------------------------------
// PresentacionAsignaciones
// ---------------------------------------------------------------------
class PresentacionAsignaciones {
public:
    PresentacionAsignaciones(ConsultaAsignaciones& consulta, CatalogoMensajes& catalogo)
        : consulta(consulta), catalogo(catalogo) {}

    void mostrar(const IdEstudiante& idEstudiante) {
        ResultadoConsulta resultado = consulta.consultarComedoresAsignados(idEstudiante);

        cout << catalogo.mensajePara(resultado.getEstado()) << "\n";

        if (resultado.estaDisponible()) {
            for (const ComedorAsignado& comedor : resultado.getComedoresAsignados()) {
                cout << "  " << nombreDia(comedor.getDia()) << ": "
                     << comedor.getNombreComedor() << "\n";
            }
        }
    }

    // SUPUESTO: el diagrama no define quien dispara el reintento ni con que
    // frecuencia; aqui simplemente repite la consulta y la muestra.
    void reintentar(const IdEstudiante& idEstudiante) { mostrar(idEstudiante); }

private:

    ConsultaAsignaciones& consulta;
    CatalogoMensajes& catalogo;
};
