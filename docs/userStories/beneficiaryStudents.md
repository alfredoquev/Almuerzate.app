
## Historia de estudiante beneficiado

Como estudiante beneficiado del servicio de alimentación de la UNAL, quiero consultar fácilmente la información de mi comedor asignado para acceder al servicio.

Criterios de Aceptación - Accesibilidad y Completitud (la información basta para ir al comedor sin buscar en demas subpaginas o hacer el procesos mas dificil).

| ID  | Parto desde | El sistema debe |
| ------------- | ------------- | ------------- |
| CA-01  | Estoy en cualquier pantalla (main)  | Renderizar el navbar con la opción "Mi comedor"  |
| CA-02  | Doy clic en "Mi comedor" sin sesión  | Redirigir al login y validar credenciales (datos de prueba)  |
| CA-03  | Estoy autenticado en "Mi comedor"  | Llamar al backend C++, que consulta en PostgreSQL la asignación y responde un JSON  |
| CA-04  | El front recibió el JSON  | Mostrar una tarjeta con el comedor del día actual: nombre, edificio y horario de atención  |
| CA-05  | Estoy viendo la tarjeta de información del día  | Mostrar la vista semanal (L–V) con el comedor de cada día y su motivo (ej. "Cercanía")  |
| CA-06  | No tengo asignación registrada  | Mostrar el mensaje "Aún no tienes comedor asignado" en lugar de un error  |

## Historia de estudiante beneficiado

Como estudiante beneficiado del servicio de alimentación de la UNAL, inconforme con mi comedor asignado, quiero solicitar un cambio de comedor para acceder al servicio de manera conveniente.

Criterios de Aceptación - Trazabilidad y Control (el cambio lo decide un administrativo delegado, no lo aplica ni el usuario ni se hace automáticamente).

| ID  | Parto desde | El sistema debe |
| ------------- | ------------- | ------------- |
| CA-01  | Estoy en la vista semanal  | Renderizar el botón "Solicitar cambio" junto a cada día|
| CA-02  | Doy clic en "Solicitar cambio" | Abrir un formulario (Google Forms) con día, comedor deseado (opcional) y motivo (obligatorio)  |
| CA-03  | Envío el formulario completo  | Validar los campos y guardar la solicitud en PostgreSQL con estado "Pendiente"  |
| CA-04  | La solicitud se guardó  | Mostrar la confirmación y la solicitud en mi historial con su estado  |
| CA-05  | Soy administrador en el panel  | Listar las solicitudes pendientes con los datos del estudiante, su asignación actual y la ocupación de cada comedor |
| CA-06  | El administrador aprueba o rechaza | Actualizar el estado, y si aprueba, modificar la asignación solo de ese día  |
| CA-07  | Vuelvo a "Mi comedor" | Reflejar la nueva asignación o el motivo del rechazo  |

## Historia de estudiante beneficiado

Como estudiante beneficiado del servicio de alimentación de la UNAL, quiero acceder al comedor que mejor se adapte a mis preferencias, como cercanía, horario y menú.

Criterios de Aceptación - Personalización

| ID  | Parto desde | El sistema debe |
| ------------- | ------------- | ------------- |
| CA-01  | Estoy en cualquier pantalla (main)  |Renderizar en el navbar el acceso a "Mis preferencias" |
| CA-02  | Entro por primera vez a esta sección | Presentar la encuesta de preferencias (cercanía, horario y menú) |
| CA-03  | Envío la encuesta  | Guardar las preferencias en PostgreSQL asociadas a mi usuario  |
| CA-04  | Hay preferencias guardadas  | El árbol de decisión en C++ las incluye como variable al calcular la asignación |
| CA-05  | Consulto mi asignación  | Mostrar el motivo de cada día ("Cercanía", "Tu horario", etc.) |
| CA-06  | Edito mis preferencias | Guardar el cambio y aplicarlo en la siguiente reasignación, avisándome cuándo será |
