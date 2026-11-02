# ADR-0006: Consulta de Asignaciones como único punto de acceso a los comedores asignados

Autores:
 - @amadodev07
 - @juanfpg1
 - @xedroc422
 - @alfredoquev
 - @julondono07

Fecha: Pendiente

## Estado

Propuesto

## Contexto

REQ-10 exige mostrar al estudiante beneficiario sus comedores asignados para el periodo de asignación vigente. El flujo de UC-01 tiene cuatro resultados: la asignación existe y está disponible, no se han realizado asignaciones aún, no hay un periodo de asignación vigente, o no se pudo obtener los comedores asignados (se informa un problema de comunicación y se pide reintentar). REQ-10 registra como riesgo (probabilidad Low, impacto High) que el acceso a los datos de asignación no sea posible por un error en el backend. Los comedores asignados los calcula y persiste el proceso de REQ-01; REQ-10 solo los lee. El estudiante usa un frontend web que recibe datos ya procesados (ADR-0002, ADR-0005). Hay que decidir si el componente que muestra los comedores asignados puede consultar directamente el registro de asignaciones o si la consulta pasa por un componente intermedio.

## Decisión

Presentación de Asignaciones no accederá directamente a Registro de Asignaciones: toda consulta pasará por Consulta de Asignaciones, que decide y devuelve el estado de la consulta (disponible, no asignados aún, sin periodo vigente o problema de comunicación). Si Presentación de Asignaciones no obtiene respuesta de Consulta de Asignaciones, lo trata como problema de comunicación.

## Alternativas consideradas

- **Presentación de Asignaciones consulta directamente a Registro de Asignaciones:** descartada porque cada uno de los cuatro resultados y cada fallo de acceso a los datos tendrían que resolverse en la interfaz, y todo cambio en el registro llegaría hasta ella.

## Consecuencias

### Positivas (+)

- Los resultados del flujo de UC-01 y los fallos de acceso a los datos se resuelven en un solo componente.
- La presentación depende solo del resultado de la consulta; un cambio en el almacenamiento no la alcanza.
- Es compatible con ADR-0002: el cliente recibe datos ya procesados.

### Negativas / Desafíos (-)

- Un componente y un salto adicionales para una función de solo lectura, con algo más de latencia.
- El estado de problema de comunicación puede originarse en dos puntos (fallo de acceso a los datos y falta de respuesta de Consulta de Asignaciones), y ambos deben tratarse igual.
