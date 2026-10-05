## Historia de administrador

Como administrador del sistema, quiero acceder a un panel de estadísticas con la estimación de ocupación de los comedores, para monitorear el comportamiento del servicio y evaluar si las asignaciones automáticas están siendo efectivas.

Criterios de Aceptación - Monitoreo y Análisis

| ID | Parto desde | El sistema debe |
| :--- | :--- | :--- |
| CA-01 | El inicio de sesión con rol de administrador | Mostrar un enlace en la navegación exclusivo para acceder al "Panel de Estadísticas". |
| CA-02 | El Panel de Estadísticas | Visualizar el estimado de ocupación actual (asignaciones + bias) de todos los comedores en una misma vista (ej. gráficos de barras o donas). |
| CA-03 | El Panel de Estadísticas | Permitir filtrar o cambiar la vista para ver las proyecciones de ocupación de distintos días de la semana y franjas horarias. |
| CA-04 | El cálculo de la plataforma | Reflejar en estas estadísticas el impacto en tiempo real de cualquier reasignación manual que el administrador haya aprobado recientemente. |

## Historia de administrador

Como administrador del sistema, quiero gestionar los casos atípicos y solicitudes de los estudiantes, para resolver inconvenientes que la asignación automática no pudo prever y garantizar un buen servicio.

Criterios de Aceptación - Trazabilidad y Resolución

| ID | Parto desde | El sistema debe |
| :--- | :--- | :--- |
| CA-01 | El "Panel de casos atípicos" | Listar todas las solicitudes pendientes, mostrando nombre del estudiante, comedor asignado actual, comedor solicitado y fecha de la petición. |
| CA-02 | Dar clic sobre una solicitud específica | Desplegar el detalle completo: historial del estudiante, justificación (motivo) enviada y comentarios específicos que haya adjuntado. |
| CA-03 | La vista de detalle de la solicitud | Renderizar botones de acción claros para "Aprobar" o "Rechazar" el caso. |
| CA-04 | Dar clic en "Aprobar" o "Rechazar" | Desplegar un campo de texto obligatorio para que el administrador redacte el motivo o comentario de su decisión. |
| CA-05 | Confirmar la decisión | Guardar el estado, registrar el comentario del administrador en la base de datos, y (si fue aprobada) ejecutar la reasignación en el sistema. |
