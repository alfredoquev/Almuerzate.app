#include <cstdlib>
#include <iostream>

#include "asignaciones.hpp"
#include "repositorio_postgres.hpp"

using namespace std;

static const char* nombreDia(DiaSemana dia) {
    switch (dia) {
        case DiaSemana::LUNES:     return "Lunes";
        case DiaSemana::MARTES:    return "Martes";
        case DiaSemana::MIERCOLES: return "Miercoles";
        case DiaSemana::JUEVES:    return "Jueves";
        case DiaSemana::VIERNES:   return "Viernes";
    }
    return "";
}

static const char* nombreEstado(EstadoConsulta estado) {
    switch (estado) {
        case EstadoConsulta::DISPONIBLE:               return "DISPONIBLE";
        case EstadoConsulta::NO_ASIGNADOS_AUN:         return "NO_ASIGNADOS_AUN";
        case EstadoConsulta::SIN_PERIODO_VIGENTE:      return "SIN_PERIODO_VIGENTE";
        case EstadoConsulta::PROBLEMA_DE_COMUNICACION: return "PROBLEMA_DE_COMUNICACION";
    }
    return "";
}

int main(int argc, char* argv[]) {
    // Uso: ./test_bd [idEstudiante]   (por defecto: estudiante1)
    IdEstudiante idEstudiante = (argc > 1) ? argv[1] : "1";

    ConfiguracionConexion configuracion;
    configuracion.host = "localhost";
    configuracion.puerto = 5432;
    configuracion.nombreBD = "almuerzate";   // cambiar por los datos reales
    configuracion.usuario = "postgres";
    const char* clave = getenv("PGPASSWORD_ALMUERZATE");
    configuracion.clave = clave ? clave : "";

    cout << "Estudiante de prueba: " << idEstudiante << "\n\n";

    // ---------------------------------------------------------------
    // Paso 1: conexion
    // ---------------------------------------------------------------
    cout << "[1] Conectando a PostgreSQL...\n";
    try {
        RepositorioAsignacionesPostgres repositorio(configuracion);
        cout << "    OK, base de datos: " << repositorio.getNombreBD() << "\n\n";

        // -----------------------------------------------------------
        // Paso 2: periodo vigente
        // -----------------------------------------------------------
        cout << "[2] buscarPeriodoVigente()\n";
        optional<PeriodoAsignacion> periodo;
        try {
            periodo = repositorio.buscarPeriodoVigente();
            if (periodo) {
                const Fecha& i = periodo->getInicio();
                const Fecha& f = periodo->getFin();
                cout << "    Periodo: " << i.anio << "-" << i.mes << "-" << i.dia
                     << " a " << f.anio << "-" << f.mes << "-" << f.dia << "\n\n";
            } else {
                cout << "    No hay periodo vigente\n\n";
            }
        } catch (const exception& error) {
            cout << "    ERROR: " << error.what() << "\n\n";
        }

        // -----------------------------------------------------------
        // Paso 3: comedores del estudiante (solo si hay periodo)
        // -----------------------------------------------------------
        cout << "[3] buscarComedoresAsignados(\"" << idEstudiante << "\")\n";
        if (periodo) {
            try {
                vector<ComedorAsignado> comedores =
                    repositorio.buscarComedoresAsignados(idEstudiante, *periodo);
                cout << "    Comedores encontrados: " << comedores.size() << "\n";
                for (const ComedorAsignado& comedor : comedores) {
                    cout << "    " << nombreDia(comedor.getDia()) << ": "
                         << comedor.getNombreComedor() << "\n";
                }
                cout << "\n";
            } catch (const exception& error) {
                cout << "    ERROR: " << error.what() << "\n\n";
            }
        } else {
            cout << "    Omitido: no hay periodo vigente\n\n";
        }

        // -----------------------------------------------------------
        // Paso 4: el flujo completo, igual que en main
        // -----------------------------------------------------------
        cout << "[4] ConsultaAsignaciones::consultarComedoresAsignados()\n";
        ConsultaAsignaciones consulta(repositorio);
        CatalogoMensajes catalogo;
        ResultadoConsulta resultado = consulta.consultarComedoresAsignados(idEstudiante);
        cout << "    Estado:  " << nombreEstado(resultado.obtenerEstado()) << "\n";
        cout << "    Mensaje: " << catalogo.mensajePara(resultado.obtenerEstado()) << "\n";

    } catch (const exception& error) {
        cout << "    ERROR de conexion: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
