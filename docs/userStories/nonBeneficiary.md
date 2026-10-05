## Historia de estudiante no beneficiario

Como estudiante no beneficiario, quiero consultar un estimado de ocupación por franja horaria de cada comedor, para decidir a cuál ir minimizando mi tiempo de espera en la fila.

Criterios de Aceptación - Visualización de Ocupación

| ID | Parto desde | El sistema debe |
| :--- | :--- | :--- |
| CA-01 | La vista de comedores (dashboard principal) | Mostrar el nivel de ocupación estimado para cada comedor mediante un indicador visual (ej. porcentaje, colores verde/amarillo/rojo). |
| CA-02 | La vista de comedores (dashboard principal) | Reflejar un nivel de ocupación que integre tanto los cupos asignados vigentes como la tendencia estadística de la encuesta universitaria, de forma transparente para el usuario. |
| CA-03 | La aplicación abierta en el dispositivo del usuario | Refrescar los datos de estimación correspondientes a la franja horaria actual cada 15 minutos sin requerir recargar toda la página. |
| CA-04 | Un fallo en la conexión de red | Mostrar la última estimación conocida e indicar la hora de la última actualización ("Actualizado hace X min"). |

## Historia de estudiante no beneficiario

Como estudiante no beneficiario, quiero consultar la ubicación de los comedores en un mapa interactivo, para saber cómo llegar fácilmente al que decida visitar.

Criterios de Aceptación - Ubicación e Interactividad

| ID | Parto desde | El sistema debe |
| :--- | :--- | :--- |
| CA-01 | La sección "Mapa de Comedores" | Renderizar un mapa interactivo basado en OpenStreetMap centrado en el campus universitario. |
| CA-02 | El mapa interactivo cargado | Resaltar la ubicación exacta de los comedores mediante marcadores o polígonos distinguibles. |
| CA-03 | Dar clic o tocar el marcador de un comedor | Desplegar una tarjeta flotante (tooltip/popup) con el nombre del comedor y un resumen de su horario de atención. |
| CA-04 | Un error de carga del proveedor de mapas (OSM) | Hacer un "fallback" automático y renderizar un mapa vectorial (SVG) estático del campus con los comedores señalados. |

## Historia de estudiante no beneficiario

Como estudiante no beneficiario, quiero conocer las tarifas del almuerzo y los horarios de atención, para poder presupuestar mi semana y llegar a tiempo al servicio.

Criterios de Aceptación - Disponibilidad de Información

| ID | Parto desde | El sistema debe |
| :--- | :--- | :--- |
| CA-01 | Cualquier pantalla (main) | Proveer un acceso visible (ej. en el footer o navbar) a la sección de "Información del servicio" o "Tarifas y Horarios". |
| CA-02 | La sección de Información del servicio | Mostrar una tabla o listado claro con los valores del almuerzo según perfil (si aplica) y la franja de horario en la que se sirve. |

## Historia de estudiante beneficiado 

Como estudiante beneficiado que ha reportado un caso atípico, quiero consultar el estado y la justificación de mi solicitud, para entender la decisión administrativa y tener claridad sobre mi asignación final.

Criterios de Aceptación - Seguimiento de Solicitudes

| ID | Parto desde | El sistema debe |
| :--- | :--- | :--- |
| CA-01 | La sección "Mi historial de solicitudes" | Listar todas mis solicitudes enviadas indicando claramente su estado actual ("Pendiente", "Aprobada", "Rechazada"). |
| CA-02 | Una solicitud con estado "Aprobada" o "Rechazada" | Desplegar el comentario o justificación específica redactada por el administrador que evaluó el caso. |
| CA-03 | La aprobación de un cambio de comedor | Actualizar automáticamente la información en mi vista de "Mi comedor" para reflejar la nueva asignación en los días correspondientes. |
