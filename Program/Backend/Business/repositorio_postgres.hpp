#pragma once

#include <pqxx/pqxx>

#include <iomanip>
#include <sstream>
#include <string>

#include "asignaciones.hpp"

using namespace std;

// ---------------------------------------------------------------------
// SUPUESTOS SOBRE EL ESQUEMA (ajustar a las tablas reales):
//
//   periodo_asignacion(fecha_inicio DATE, fecha_fin DATE)
//   asignacion(id_estudiante TEXT, dia INT, nombre_comedor TEXT,
//              periodo_inicio DATE)
//
//   - dia: 1 = lunes ... 5 = viernes
//   - Un periodo "vigente" es el que contiene la fecha actual.
//   - Una asignacion pertenece a un periodo por su fecha de inicio.
// ---------------------------------------------------------------------

struct ConfiguracionConexion {
    string host = "localhost";
    int puerto = 5432;
    string nombreBD;
    string usuario;
    string clave;

    string toConnectionString() const {
        ostringstream cadena;
        cadena << "host=" << host
               << " port=" << puerto
               << " dbname=" << nombreBD
               << " user=" << usuario
               << " password=" << clave;
        return cadena.str();
    }
};

class RepositorioAsignacionesPostgres : public RepositorioAsignaciones {
public:
    explicit RepositorioAsignacionesPostgres(const ConfiguracionConexion& configuracion)
        : conexion(configuracion.toConnectionString()) {}

    RepositorioAsignacionesPostgres(const RepositorioAsignacionesPostgres&) = delete;
    RepositorioAsignacionesPostgres& operator=(const RepositorioAsignacionesPostgres&) = delete;

    string getNombreBD() const { return conexion.dbname(); }

    // -----------------------------------------------------------------
    // Devuelve el periodo que contiene la fecha actual, o nullopt si no hay
    // -----------------------------------------------------------------
    optional<PeriodoAsignacion> buscarPeriodoVigente() override {
        pqxx::work transaccion(conexion);

        pqxx::result filas = transaccion.exec(
            "SELECT EXTRACT(YEAR  FROM fecha_inicio)::int AS anio_inicio, "
            "       EXTRACT(MONTH FROM fecha_inicio)::int AS mes_inicio, "
            "       EXTRACT(DAY   FROM fecha_inicio)::int AS dia_inicio, "
            "       EXTRACT(YEAR  FROM fecha_fin)::int    AS anio_fin, "
            "       EXTRACT(MONTH FROM fecha_fin)::int    AS mes_fin, "
            "       EXTRACT(DAY   FROM fecha_fin)::int    AS dia_fin "
            "FROM periodo_asignacion "
            "WHERE CURRENT_DATE BETWEEN fecha_inicio AND fecha_fin "
            "LIMIT 1");

        if (filas.empty()) {
            return nullopt;
        }

        const pqxx::row fila = filas[0];
        Fecha inicio{fila["anio_inicio"].as<int>(),
                     fila["mes_inicio"].as<int>(),
                     fila["dia_inicio"].as<int>()};
        Fecha fin{fila["anio_fin"].as<int>(),
                  fila["mes_fin"].as<int>(),
                  fila["dia_fin"].as<int>()};

        return PeriodoAsignacion(inicio, fin);
    }

    // -----------------------------------------------------------------
    // Devuelve los comedores del estudiante en ese periodo (0 o 5 filas)
    // -----------------------------------------------------------------
    vector<ComedorAsignado> buscarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const PeriodoAsignacion& periodo) override {
        pqxx::work transaccion(conexion);

        pqxx::result filas = transaccion.exec_params(
            "SELECT dia, nombre_comedor "
            "FROM asignacion "
            "WHERE id_estudiante = $1 AND periodo_inicio = $2::date "
            "ORDER BY dia",
            idEstudiante, fechaATexto(periodo.getInicio()));

        vector<ComedorAsignado> comedores;
        for (const pqxx::row& fila : filas) {
            comedores.emplace_back(enteroADia(fila["dia"].as<int>()),
                                   fila["nombre_comedor"].as<string>());
        }
        return comedores;
    }

    // -----------------------------------------------------------------
    // Reemplaza las asignaciones del estudiante en el periodo vigente.
    // Borrado e insercion van en una sola transaccion: o se guardan todas
    // o ninguna.
    // -----------------------------------------------------------------
    void guardarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const vector<ComedorAsignado>& comedores) override {
        // La interfaz no recibe el periodo, asi que se usa el vigente
        optional<PeriodoAsignacion> periodo = buscarPeriodoVigente();
        if (!periodo) {
            throw runtime_error("No hay periodo de asignacion vigente");
        }
        string inicio = fechaATexto(periodo->getInicio());

        pqxx::work transaccion(conexion);

        transaccion.exec_params(
            "DELETE FROM asignacion "
            "WHERE id_estudiante = $1 AND periodo_inicio = $2::date",
            idEstudiante, inicio);

        for (const ComedorAsignado& comedor : comedores) {
            transaccion.exec_params(
                "INSERT INTO asignacion (id_estudiante, dia, nombre_comedor, periodo_inicio) "
                "VALUES ($1, $2, $3, $4::date)",
                idEstudiante, diaAEntero(comedor.getDia()),
                comedor.getNombreComedor(), inicio);
        }

        transaccion.commit();
    }

private:
    static string fechaATexto(const Fecha& fecha) {
        ostringstream texto;
        texto << setfill('0') << setw(4) << fecha.anio << '-'
              << setw(2) << fecha.mes << '-'
              << setw(2) << fecha.dia;
        return texto.str();
    }

    static DiaSemana enteroADia(int valor) {
        switch (valor) {
            case 1: return DiaSemana::LUNES;
            case 2: return DiaSemana::MARTES;
            case 3: return DiaSemana::MIERCOLES;
            case 4: return DiaSemana::JUEVES;
            case 5: return DiaSemana::VIERNES;
        }
        throw runtime_error("Valor de dia invalido en la base de datos");
    }

    static int diaAEntero(DiaSemana dia) { return static_cast<int>(dia) + 1; }

    pqxx::connection conexion;
};
