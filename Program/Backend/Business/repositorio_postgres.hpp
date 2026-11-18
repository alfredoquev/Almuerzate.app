#pragma once

#include <pqxx/pqxx>

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include "asignaciones.hpp"

using namespace std;

// ---------------------------------------------------------------------
// ESQUEMA QUE USA ESTE REPOSITORIO (almuerzate.app):
//
//   estudiante(id_estudiante INT, es_beneficiario BOOL)
//   comedor(id_comedor INT, nombre TEXT UNIQUE, capacidad_maxima INT)
//   periodo_asignacion(id_periodo INT, fecha_inicio DATE, fecha_fin DATE,
//                      es_vigente BOOL)        -- solo uno puede ser TRUE
//   asignacion(id_asignacion INT, id_estudiante INT, id_comedor INT,
//              id_periodo INT, dia_semana TEXT)
//
//   - dia_semana: 'LUNES', 'MARTES', 'MIERCOLES', 'JUEVES', 'VIERNES'
//   - Un periodo "vigente" es el que tiene es_vigente = TRUE.
//   - IdEstudiante en C++ es string, pero en la BD es INTEGER: se
//     convierte con idComoEntero().
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
    // Abre la conexion al crear el objeto. Lanza excepcion si falla.
    explicit RepositorioAsignacionesPostgres(const ConfiguracionConexion& configuracion)
        : conexion(configuracion.toConnectionString()) {}

    RepositorioAsignacionesPostgres(const RepositorioAsignacionesPostgres&) = delete;
    RepositorioAsignacionesPostgres& operator=(const RepositorioAsignacionesPostgres&) = delete;

    string getNombreBD() const { return conexion.dbname(); }

    // -----------------------------------------------------------------
    // Devuelve el periodo marcado como vigente, o nullopt si no hay
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
            "WHERE es_vigente "
            "LIMIT 1");

        if (filas.empty()) {
            return nullopt;
        }

        const auto fila = filas[0];
        Fecha inicio{fila["anio_inicio"].as<int>(),
                     fila["mes_inicio"].as<int>(),
                     fila["dia_inicio"].as<int>()};
        Fecha fin{fila["anio_fin"].as<int>(),
                  fila["mes_fin"].as<int>(),
                  fila["dia_fin"].as<int>()};

        return PeriodoAsignacion(inicio, fin);
    }

    // -----------------------------------------------------------------
    // Devuelve los comedores del estudiante en ese periodo (0 o 5 filas),
    // ordenados de lunes a viernes.
    // El periodo se identifica por sus fechas (la clase PeriodoAsignacion
    // no guarda el id).
    // -----------------------------------------------------------------
    vector<ComedorAsignado> buscarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const PeriodoAsignacion& periodo) override {
        int id = idComoEntero(idEstudiante);

        pqxx::work transaccion(conexion);

        pqxx::result filas = transaccion.exec_params(
            "SELECT a.dia_semana, c.nombre "
            "FROM asignacion a "
            "JOIN comedor c            ON c.id_comedor = a.id_comedor "
            "JOIN periodo_asignacion p ON p.id_periodo = a.id_periodo "
            "WHERE a.id_estudiante = $1 "
            "  AND p.fecha_inicio  = $2::date "
            "  AND p.fecha_fin     = $3::date "
            "ORDER BY CASE a.dia_semana "
            "           WHEN 'LUNES' THEN 1 WHEN 'MARTES' THEN 2 "
            "           WHEN 'MIERCOLES' THEN 3 WHEN 'JUEVES' THEN 4 "
            "           WHEN 'VIERNES' THEN 5 END",
            id,
            fechaATexto(periodo.getInicio()),
            fechaATexto(periodo.getFin()));

        vector<ComedorAsignado> comedores;
        for (const auto& fila : filas) {
            comedores.emplace_back(textoADia(fila["dia_semana"].as<string>()),
                                   fila["nombre"].as<string>());
        }
        return comedores;
    }

    // -----------------------------------------------------------------
    // Reemplaza las asignaciones del estudiante en el periodo vigente.
    // Todo va en una sola transaccion: o se guardan todas o ninguna.
    // Lanza excepcion si no hay periodo vigente o si un nombre de comedor
    // no existe.
    // -----------------------------------------------------------------
    void guardarComedoresAsignados(
        const IdEstudiante& idEstudiante,
        const vector<ComedorAsignado>& comedores) override {
        int id = idComoEntero(idEstudiante);

        pqxx::work transaccion(conexion);

        // La interfaz no recibe el periodo, asi que se usa el vigente
        pqxx::result periodo = transaccion.exec(
            "SELECT id_periodo FROM periodo_asignacion WHERE es_vigente LIMIT 1");
        if (periodo.empty()) {
            throw runtime_error("No hay periodo de asignacion vigente");
        }
        int idPeriodo = periodo[0]["id_periodo"].as<int>();

        transaccion.exec_params(
            "DELETE FROM asignacion "
            "WHERE id_estudiante = $1 AND id_periodo = $2",
            id, idPeriodo);

        for (const ComedorAsignado& comedor : comedores) {
            // Busca el id del comedor por nombre dentro del mismo INSERT.
            pqxx::result insertado = transaccion.exec_params(
                "INSERT INTO asignacion (id_estudiante, id_comedor, id_periodo, dia_semana) "
                "SELECT $1::int, c.id_comedor, $2::int, $3::text "
                "FROM comedor c "
                "WHERE c.nombre = $4::text",
                id, idPeriodo, diaATexto(comedor.getDia()),
                comedor.getNombreComedor());

            if (insertado.affected_rows() == 0) {
                throw runtime_error("El comedor no existe: " + comedor.getNombreComedor());
            }
        }

        transaccion.commit();
    }

private:
    // IdEstudiante es string en C++ pero INTEGER en la BD
    static int idComoEntero(const IdEstudiante& id) {
        try {
            size_t usados = 0;
            int valor = stoi(id, &usados);
            if (usados != id.size()) {
                throw invalid_argument("");
            }
            return valor;
        } catch (const exception&) {
            throw invalid_argument("idEstudiante debe ser un numero entero: '" + id + "'");
        }
    }

    static string fechaATexto(const Fecha& fecha) {
        ostringstream texto;
        texto << setfill('0') << setw(4) << fecha.anio << '-'
              << setw(2) << fecha.mes << '-'
              << setw(2) << fecha.dia;
        return texto.str();
    }

    static DiaSemana textoADia(const string& texto) {
        if (texto == "LUNES")     return DiaSemana::LUNES;
        if (texto == "MARTES")    return DiaSemana::MARTES;
        if (texto == "MIERCOLES") return DiaSemana::MIERCOLES;
        if (texto == "JUEVES")    return DiaSemana::JUEVES;
        if (texto == "VIERNES")   return DiaSemana::VIERNES;
        throw runtime_error("Valor de dia_semana invalido en la base de datos: " + texto);
    }

    static string diaATexto(DiaSemana dia) {
        switch (dia) {
            case DiaSemana::LUNES:     return "LUNES";
            case DiaSemana::MARTES:    return "MARTES";
            case DiaSemana::MIERCOLES: return "MIERCOLES";
            case DiaSemana::JUEVES:    return "JUEVES";
            case DiaSemana::VIERNES:   return "VIERNES";
        }
        throw runtime_error("DiaSemana invalido");
    }

    pqxx::connection conexion;
};
