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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <idEstudiante>\n";
        return 1;
    }
    IdEstudiante idEstudiante = argv[1];

    // Configuracion de la conexion (cambiar por los datos reales)
    ConfiguracionConexion configuracion;
    configuracion.host = "localhost";
    configuracion.puerto = 5432;
    configuracion.nombreBD = "almuerzate";
    configuracion.usuario = "postgres";
    const char* clave = getenv("PGPASSWORD_ALMUERZATE");
    configuracion.clave = clave ? clave : "";

    CatalogoMensajes catalogo;

    try {
        RepositorioAsignacionesPostgres repositorio(configuracion);
        ConsultaAsignaciones consulta(repositorio);

        ResultadoConsulta resultado = consulta.consultarComedoresAsignados(idEstudiante);

        cout << catalogo.mensajePara(resultado.obtenerEstado()) << "\n";

        if (resultado.estaDisponible()) {
            for (const ComedorAsignado& comedor : resultado.obtenerComedoresAsignados()) {
                cout << nombreDia(comedor.getDia()) << ": "
                     << comedor.getNombreComedor() << "\n";
            }
        }
    } catch (const exception&) {
        // La conexion fallo antes de que existiera ConsultaAsignaciones
        cout << catalogo.mensajePara(EstadoConsulta::PROBLEMA_DE_COMUNICACION) << "\n";
    }

    return 0;
}
