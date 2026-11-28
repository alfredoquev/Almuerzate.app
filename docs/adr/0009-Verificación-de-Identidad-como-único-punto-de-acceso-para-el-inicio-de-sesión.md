# ADR-0009: Verificación de Identidad como único punto de acceso para el inicio de sesión

Autores:
 - @amadodev07
 - @juanfpg1
 - @xedroc422
 - @alfredoquev
 - @julondono07

Fecha: 2026-11-28

## Estado

Propuesto

## Contexto

REQ-11 exige permitir que el estudiante beneficiario y el administrador inicien sesión en la página web de Almuérzate, reconociendo su rol para conceder acceso únicamente a lo que corresponde a cada uno y proteger los datos personales de los estudiantes frente a terceros (RFC, Seguridad). El flujo de UC-02 contempla tres resultados: sesión iniciada con reconocimiento de rol (AC-05), credenciales no válidas (AC-06) y falla de acceso a los datos (problema de comunicación con opción de reintento). Además, REQ-11 registra riesgos de impacto High: verificación de identidad deficiente (Medium/High), inyección de comandos (Medium/High) y falla de acceso a los datos (Low/High). El sistema opera con una interfaz web que recibe datos ya procesados y un backend en el servidor que gestiona los datos y la lógica bajo los mecanismos de integración y persistencia ya aceptados (ADR-0001, ADR-0002, ADR-0004, ADR-0005, ADR-0007 y ADR-0008). Hay que decidir si el componente encargado de la interfaz de inicio de sesión puede validar credenciales o consultar directamente el registro de cuentas, o si toda solicitud debe pasar por un componente intermedio de verificación.

## Decisión

Presentación de Inicio de Sesión no accederá directamente a Registro de Cuentas ni validará credenciales por sí misma: toda solicitud de inicio de sesión pasará por Verificación de Identidad, que valida el formato de las entradas, consulta el registro, determina el rol y devuelve el resultado del inicio de sesión (sesión iniciada, credenciales no válidas o problema de comunicación).

## Alternativas consideradas

- **Presentación de Inicio de Sesión consulta directamente a Registro de Cuentas y valida credenciales en la interfaz:** descartada porque dispersaría la validación de entradas, el manejo de los tres resultados de UC-02 y la gestión de fallos de acceso a los datos en la capa de presentación, exponiendo reglas de verificación y riesgos de seguridad (RFC, Seguridad) ante entradas malformadas.

## Consecuencias

### Positivas (+)

- Centraliza en un solo componente la validación de entradas antes de alcanzar el acceso a datos (mitigando el riesgo de inyección de comandos), la resolución de los resultados de UC-02 y la determinación del rol.
- Mantiene la presentación desacoplada del almacenamiento de cuentas y del mecanismo interno de verificación, siguiendo la misma separación de responsabilidades adoptada en ADR-0006.
- Evita exponer detalles técnicos de fallos internos o información sobre la existencia de cuentas hacia el cliente web, cumpliendo con los requisitos de seguridad del RFC y AC-06.

### Negativas / Desafíos (-)

- Añade un componente y un salto adicional en el flujo de inicio de sesión, obligando a cruzar la frontera de integración entre la presentación y el backend del servidor (ADR-0007 y ADR-0008).
- El estado de problema de comunicación puede originarse en dos puntos (fallo de acceso a los datos en Registro de Cuentas o falta de respuesta/error de comunicación entre Presentación de Inicio de Sesión y Verificación de Identidad), y ambos deben tratarse de forma uniforme ofreciendo el reintento.
