# AI Context — Redacción de Requerimientos (almuerzate.app)

> Este archivo es el contexto que debe leer una IA **antes** de redactar, revisar o completar cualquier archivo de requerimiento (`REQ-XX.md`) de este proyecto. Su único alcance son los requerimientos; no aplica a los ADR (ver `ai_context_adrs.md`) ni al diseño (ver `ai_context_design.md`).
>
> Última actualización: 2026-11-25.

---

## 1. Rol y objetivo

Actúas como asistente de especificación de requerimientos. Tu tarea es ayudar al equipo a **especificar, no inventar**: convertir cada requerimiento de `Requirements.md` en un archivo completo según la plantilla `Requirement_Template.md`, usando solo la información real del proyecto.

- Un requerimiento **no es una solución**: describe qué debe lograr el sistema y por qué, no cómo se construye.
- Un requerimiento **no está solo**: se documentan sus reglas de negocio, restricciones, supuestos, dependencias y riesgos, o se declara honestamente que no hay.
- Idioma: **español**. Los nombres técnicos se dejan como son. Los encabezados y etiquetas de la plantilla están en inglés y **se mantienen en inglés** (es la plantilla oficial); el **contenido** va en español.
- Tono: neutral, técnico y directo. Sin marketing ni adjetivos innecesarios.
- Se trabaja con el proyecto **real**. El ejemplo de referencia (`REQ-07_Example.md`) solo sirve para calibrar el **nivel de detalle** y **cómo se conectan las secciones**: no se copia nada de su contenido (ni su dominio, ni sus reglas, evidencias, números o escenarios).

---

## 2. Fuentes permitidas

Solo se puede usar información de estas fuentes:

| Fuente | Qué aporta |
|---|---|
| `Requirements.md` | El enunciado base de cada requerimiento (REQ-01 … REQ-11) |
| `rfc_almuerzate.md` | Motivación, alcance por entregas, métricas, riesgos, dependencias, seguridad y preguntas sin resolver |
| ADR 0001–0005 (**Aceptados**) y ADR 0006–0008 (**Propuestos**) | Restricciones técnicas y de diseño vigentes (el 0006 se cita solo en REQ-10; los 0006–0008 solo con la salvedad de que aún no están aceptados) |
| `ai_context_adrs.md` / `ai_context_general.md` | Contexto general del proyecto: actores, arquitectura, terminología |
| `REQ-01.md` y `REQ-10.md` (**Validated**) | Reglas de negocio ya definidas (BR-01, BR-02), dependencias entre requerimientos, formato de uso de la plantilla y evidencia aportada por el equipo |
| `REQ-11.md` (**Draft**) | Propuesta de inicio de sesión: IDs nuevos (US-02, AC-05, AC-06, UC-02), restricciones y riesgos de acceso. Es un borrador: sus IDs y su contenido se citan con la salvedad de su estado |

Todo lo que no esté en estas fuentes **no existe** hasta que el equipo lo aporte. Si hace falta algo (evidencia de stakeholders, reglas de negocio, casos de uso, fecha), se pregunta o se marca `Pendiente` / Open Question.

**Evidencia aportada por el equipo:** `REQ-01.md` y `REQ-10.md` citan como evidencia una entrevista con los stakeholders realizada en clase, según la cual el sistema debe asignar comedores a los estudiantes beneficiarios para cada día de la semana. Esa evidencia solo puede reutilizarse en requerimientos que dependan directamente de esa necesidad (REQ-01 y los que la complementan); para los demás se confirma con el equipo o se marca `Pendiente`. REQ-11 no reutiliza esa evidencia (no depende de la necesidad de asignar comedores por día): su evidencia es `Requirements.md`, Requerimiento 11, y, de forma indirecta, la sección Seguridad del RFC; la Discovery Sheet queda `Pendiente`.

---

## 3. Contexto del proyecto

**almuerzate.app** es un sistema de asignación y gestión de cupos de comedor para estudiantes beneficiarios de la Universidad Nacional de Colombia.

**Problema (RFC, Motivación):** algunos comedores están saturados mientras otros tienen aforo mínimo, lo que genera filas, estudiantes que llegan tarde a clase y descontento. Se propone asignar cada estudiante a un comedor con un **árbol decisional** que pondera aforo, preferencias de comedor, discapacidades, horario diario, preferencias alimentarias y ubicación relativa.

### 3.1 Actores

| Actor | Relación con los requerimientos |
|---|---|
| Estudiante beneficiario | Recibe la asignación y la consulta (REQ-10); aporta horario, preferencia de comedor y limitaciones; registra reportes; inicia sesión (REQ-11) |
| Administrador | Recibe notificaciones; consulta el desempeño de los comedores; atiende los casos aislados que requieren intervención manual; inicia sesión (REQ-11) |
| Sistema (asignación automática) | Actor interno para las funciones que se ejecutan sin intervención humana (REQ-01, REQ-06) |

### 3.2 Requerimientos de partida (fuente: `Requirements.md`)

| ID | Enunciado base | Estado del archivo |
|---|---|---|
| REQ-01 | El sistema debe asignar a cada estudiante beneficiario un comedor para cada día de la semana, y esa asignación debe mantenerse constante hasta el final del semestre | `REQ-01.md` Validated |
| REQ-02 | El sistema debe considerar el horario del estudiante al asignarle un comedor | Pendiente |
| REQ-03 | El sistema debe considerar la preferencia del estudiante sobre el comedor en el que desea recibir su beneficio | Pendiente |
| REQ-04 | El sistema debe considerar las condiciones del comedor (capacidad, horario de servicio, menú disponible) | Pendiente |
| REQ-05 | El sistema debe considerar las limitaciones del estudiante (discapacidad, preferencia alimentaria) | Pendiente |
| REQ-06 | El sistema debe ejecutar la asignación de forma automática, sin intervención manual salvo casos aislados que la requieran | Pendiente |
| REQ-07 | El sistema debe permitir a los usuarios registrar reportes de inconformidades o fallas | Pendiente |
| REQ-08 | El sistema debe notificar a los administradores cada vez que se registre un reporte o se detecte una falla | Pendiente |
| REQ-09 | El sistema debe permitir a los administradores consultar el desempeño de los comedores (capacidad, horario y funcionamiento) | Pendiente |
| REQ-10 | El sistema debe permitir mostrarle a los estudiantes beneficiarios sus asignaciones de comedor para el periodo de asignación vigente | `REQ-10.md` Validated |
| REQ-11 | El sistema debe permitir que el estudiante beneficiario y el administrador inicien sesión en la página de Almuérzate | `REQ-11.md` Draft |

> **Formato de ID y ruta (confirmados):** un archivo por requerimiento, `REQ-XX.md`, en `docs/requirements/`. `Requirements.md` conserva "Requerimiento N".

### 3.3 Restricciones técnicas vigentes (ADR 0001–0005 aceptados; 0006–0008 propuestos)

Solo se citan en la sección **Constraint(s)** cuando realmente limitan la solución de ese requerimiento. Nunca en el enunciado.

| ADR | Restricción que impone |
|---|---|
| 0001 | Persistencia relacional y transaccional (PostgreSQL, ACID); la consistencia de la asignación de cupos es crítica |
| 0002 | El backend corre en el servidor; PHP entrega al cliente solo los datos ya procesados por el árbol decisional. Los argumentos que PHP pasa al ejecutable deben validarse (inyección de comandos) |
| 0003 | Estilos con Tailwind CSS (solo afecta la presentación) |
| 0004 | La lógica de asignación y la gestión de datos están en C++, en el servidor |
| 0005 | Frontend web (HTML, JavaScript y CSS) accesible desde navegador, sin app móvil ni frameworks descartados |
| 0006 (Propuesto) | La consulta del estudiante a sus comedores asignados pasa siempre por un componente intermedio (Consulta de Asignaciones); aplica solo a REQ-10 y se cita con la salvedad de su estado |
| 0007 (Propuesto) | Mecanismo de invocación del backend: librería compartida cargada por PHP mediante FFI, en lugar del ejecutable del ADR-0002. Es una decisión de integración: no se cita en Constraint(s) salvo que el equipo la asocie a un requerimiento concreto y, si se cita, con la salvedad de su estado |
| 0008 (Propuesto) | Acceso a la base de datos desde el backend mediante un script PHP, en lugar de conexión directa con `libpqxx`. Misma regla de cita que el 0007 |

> Aplicación conocida: REQ-11 cita el ADR-0002 (validación de los argumentos que PHP entrega al backend, p. ej. lo que la persona escribe al iniciar sesión) y el ADR-0005 (la página es web, accesible desde navegador). No cita el 0001 (no está definido dónde viven las cuentas) ni el 0003, 0006, 0007 y 0008.

### 3.4 Alcance por entregas (RFC, Propuesta de implementación)

Sirve como señal para **prioridad** y para **dependencias entre requerimientos**.

| Entrega | Contenido | Requerimientos que probablemente toca |
|---|---|---|
| Entrega 1 | Aplicación en línea donde el estudiante consulta su comedor asignado; asignación "inteligente" con información personal y características del estudiante | REQ-01 a REQ-06 y REQ-10; REQ-11 (hipótesis: identificar al estudiante que consulta; el RFC no menciona el inicio de sesión) |
| Entrega 2 | Encuesta de necesidades y preferencias; reasignación periódica; mejora de la asignación de cupos | REQ-03, REQ-05 y REQ-06 (tocan la encuesta y la reasignación) |
| Entrega final | Estadísticas para administradores; reasignaciones de emergencia; diferenciación de cupos por lapso de tiempo | REQ-06 y REQ-09 |

> Es una hipótesis de trazabilidad, no una decisión. El RFC **no menciona** reportes ni notificaciones: la evidencia de REQ-07 y REQ-08 es solo `Requirements.md` (ver sección 5). Tampoco menciona el inicio de sesión: la evidencia de REQ-11 es `Requirements.md` y, de forma indirecta, la sección Seguridad del RFC.

### 3.5 Insumos del RFC reutilizables

- **Riesgos del RFC:** asignación errónea de comedores (aumenta desplazamientos y filas), limitaciones físicas del aforo (es un tope que el sistema no puede superar), fuga de información privada de los estudiantes.
- **Riesgos aceptados en ADR:** costo por ejecución (cada solicitud lanza un proceso del backend), validación de argumentos (inyección de comandos), curva de aprendizaje de PHP (ADR-0002, vigente). Los ADR 0007 y 0008 (Propuestos) cambiarían estos riesgos (FFI habilitado, una librería por sistema operativo, proceso `php` lanzado desde C++ por consulta); mientras no se acepten se sigue citando el riesgo del ADR-0002 y, si se mencionan, con la salvedad de su estado.
- **Riesgos ya registrados en REQ-01, REQ-10 y REQ-11:** capacidad de un comedor sobreestimada por el efecto de estudiantes no beneficiarios que el sistema no mide (REQ-01); error de acceso a los datos de asignación por falla del backend (REQ-10); acceso no autorizado a datos de estudiantes por una verificación de identidad deficiente, inyección de comandos con lo que la persona escribe al iniciar sesión y falla de acceso a los datos que impide iniciar sesión (REQ-11, Draft; Likelihood e Impact son estimación del equipo a validar). Se reutilizan o referencian, no se duplican.
- **Dependencias del RFC:** el estado actual de los comedores (infraestructura y capacidad) y las entidades que manejan los datos de los estudiantes relevantes para la asignación.
- **Seguridad (RFC):** el acceso a los datos de los estudiantes debe estar restringido y protegido. REQ-11 es el requerimiento que más se relaciona con esta línea.
- **Métrica del RFC:** reducir el tiempo de fila a menos de 40 minutos. Es una métrica del **proyecto**; solo se cita en un requerimiento si el equipo la asocia explícitamente a él. El RFC deja abierta cómo se medirá de forma confiable.
- **Preguntas sin resolver del RFC:** ver sección 6; son la fuente natural de **Open Questions**.

### 3.6 Reglas de negocio ya definidas por el equipo

| ID | Regla | Definida en |
|---|---|---|
| BR-01 | La asignación de comedores a los estudiantes beneficiarios debe tener frecuencia diaria | REQ-01, REQ-10 |
| BR-02 | Las asignaciones solo se pueden realizar mientras se esté cursando un semestre académico | REQ-01, REQ-10 |

> Los requerimientos que dependen de REQ-01 (REQ-02 a REQ-06) pueden citar estas reglas con el mismo ID, sin redefinirlas. Una regla nueva se numera a continuación (BR-03…) solo si el equipo la aporta, y se verifica que el ID sea coherente entre archivos.

> REQ-11 no define reglas de negocio nuevas (`N/A — none identified`): BR-01 y BR-02 regulan la asignación, no el acceso.

### 3.7 IDs en uso

Antes de definir una historia, un criterio, un caso de uso o una regla nuevos, se consulta esta tabla para no reutilizar IDs. Los IDs de REQ-11 están en borrador (Draft) hasta que el equipo los confirme.

| Tipo | En uso | Definidos en | Siguiente libre |
|---|---|---|---|
| US | US-01 | `REQ-01.md` (REQ-10 la reutiliza) | US-03 |
| US | US-02 | `REQ-11.md` (Draft) | US-03 |
| AC | AC-01, AC-02 | `REQ-01.md` | AC-07 |
| AC | AC-03, AC-04 | `REQ-10.md` | AC-07 |
| AC | AC-05, AC-06 | `REQ-11.md` (Draft) | AC-07 |
| UC | UC-01 | `REQ-10.md` | UC-03 |
| UC | UC-02 | `REQ-11.md` (Draft) | UC-03 |
| BR | BR-01, BR-02 | `REQ-01.md`, `REQ-10.md` | BR-03 |

---

## 4. Estructura obligatoria del archivo de requerimiento

Usa **siempre** `Requirement_Template.md`, con sus secciones, orden y encabezados. No añadas ni quites secciones. **Elimina el comentario HTML inicial** ("HOW TO USE THIS TEMPLATE"). Si una sección no aplica, escribe `N/A — none identified` y una línea con el motivo; si algo se desconoce, va en **Open Questions**, no se adivina. No se deja texto de la plantilla sin resolver (p. ej. "High / Medium / Low" en Estimate).

### 4.0 Encabezado

| Campo | Regla |
|---|---|
| Título | `# REQ-XX — Título corto` |
| Status | `Draft` por defecto; `Validated` solo si la sección 6 está honestamente completa; `Open Question` si hay una duda que bloquea |
| Team / Author(s) | Usuarios de GitHub con `@` (habituales: `@amadodev07`, `@juanfpg1`, `@xedroc422`, `@alfredoquev`, `@julondono07`). Si no se indicó quién redacta, `Pendiente` |
| Date | `AAAA-MM-DD`; si no se dio, `Pendiente` |
| Linked User Story / AC | IDs de la historia y de los criterios (convención de la plantilla: `US-XX`, `AC-XX.N`; el proyecto usa `US-XX`, `AC-XX`). Deben existir en las secciones 4.1 y 4.2 **del mismo archivo**; si un requerimiento reutiliza la historia de otro, se indica de forma explícita |

### 4.1 Sección 1 — Overview

- **Requirement Statement:** una sola frase, "El sistema debe …", que conserve el significado del enunciado de `Requirements.md` **correspondiente al mismo número**. Describe **qué**, no **cómo**: no menciona PHP, C++, PostgreSQL, Tailwind ni el árbol decisional. Se verifica que no sea copia del enunciado de otro requerimiento.
- **Type (Functional / Non-functional):** se justifica con el **Perfect Technology Filter** (computador perfecto: velocidad infinita, memoria ilimitada, cero fallas, costo cero; ¿el requerimiento sigue importando? Sí → funcional). Escribe el razonamiento en una línea para este requerimiento. En el conjunto actual, los enunciados describen comportamientos del sistema, pero **aplica el filtro caso por caso**, no lo asumas.
- **Source / Evidence:** de dónde sale, **citando solo las fuentes de la sección 2** (p. ej. "`Requirements.md`, Requerimiento N; RFC, sección Motivación"). Si el equipo tiene una Discovery Sheet con entradas de evidencia, se referencian sus IDs; si no, `Pendiente`. **No inventes entrevistas, citas ni IDs de evidencia.**
- **Need:** el objetivo subyacente expresado como meta, no como solución (p. ej. qué quiere lograr el estudiante o el administrador, no qué pantalla o algoritmo usar).
- **Value / Rationale:** por qué importa, apoyado en la Motivación del RFC (saturación, filas, llegadas tarde a clase, descontento). Es lo mismo que el "para que" de la historia de usuario: se escribe una vez y se reutiliza.

### 4.2 Sección 2 — Context

Rellena con honestidad. `N/A — none identified` es una respuesta válida y esperada en varias entradas.

| Entrada | Qué va | Qué no va |
|---|---|---|
| **Business Rule(s)** | Reglas del dominio, independientes del software, que el requerimiento apoya o hace cumplir. Fuentes posibles: el RFC (p. ej. el aforo real de un comedor es un tope que no se puede superar) y las reglas ya definidas (sección 3.6) | Reglas inventadas. Una regla que no esté en las fuentes se marca como candidata y se pregunta al equipo. Recuerda: regla de negocio ≠ requerimiento de software |
| **Constraint(s)** | Lo que **limita cómo se resuelve**: las restricciones de la sección 3.3 que apliquen a este requerimiento; restricciones institucionales o de datos que el RFC mencione | Todas las ADR "por si acaso": solo las que afectan a este requerimiento |
| **Assumption(s)** | Lo que se trata como cierto sin verificar (supuesto ≠ hecho). Se declara como supuesto, no como hecho | Hechos verificados |
| **Dependenc(ies)** | De qué depende para cumplirse: otros requerimientos (p. ej. REQ-02 a REQ-05 alimentan REQ-01; REQ-10 lee lo que REQ-01 persiste), datos de entidades externas, estado de los comedores | Tecnologías elegidas por gusto |
| **Risk(s)** | Tabla con Likelihood e Impact (High / Medium / Low) y mitigación opcional; la tabla debe estar bien formada (misma cantidad de columnas en cada fila). Parte de los riesgos del RFC y de las ADR (sección 3.5) que apliquen | Riesgos genéricos sin relación con el requerimiento. Likelihood e Impact sin evidencia se marcan como estimación del equipo a validar |
| **Open Questions** | Todo lo sin resolver. Parte de las preguntas del RFC (sección 6) y de las ambigüedades del enunciado | Respuestas supuestas |

### 4.3 Sección 3 — Priority & Estimation

- **Modelo de priorización elegido por el equipo: MoSCoW** (usado en `REQ-01.md`, `REQ-10.md` y `REQ-11.md`). Se usa de forma **consistente en todos los requerimientos**; no se cambia de modelo sin que el equipo lo pida.
- Redacta la prioridad con la estructura de la plantilla: *"Porque [señal del proyecto], asignamos [valor], aceptando el trade-off de [costo]. Consideramos [alternativa] y no la usamos porque [razón]"*. La señal del proyecto sale de las fuentes (p. ej. la entrega del RFC en la que aparece el requerimiento, su centralidad para el problema).
- **No inventes números** (alcance, esfuerzo en semanas, porcentajes de confianza).
- **Estimate (confidence):** `High` / `Medium` / `Low` (un solo valor, sin dejar las tres opciones de la plantilla), con una frase que explique la incertidumbre. Es una etiqueta de confianza, no una técnica de estimación ni una cifra.

### 4.4 Sección 4 — Representations

Las tres representaciones son obligatorias y deben ser **coherentes entre sí y con el enunciado**. Excepción: si el requerimiento es un proceso interno sin interacción directa con el usuario, el caso de uso se declara `N/A` con el motivo (así lo hace `REQ-01.md`).

**4.1 User Story**
- Formato: *Como **[rol]**, quiero **[capacidad]**, para que **[valor]**.* Los roles salen de la sección 3.1.
- Cada historia lleva su ID (`US-XX`) en el mismo archivo donde se define.
- Si un requerimiento involucra dos roles con la misma capacidad (REQ-11), puede definirse una sola historia que nombre ambos roles (US-02).
- Autoverificación: el "quiero" describe la necesidad, no prescribe una solución.
- Para REQ-02 a REQ-05 (criterios de la asignación) el rol puede ser el estudiante beneficiario y la capacidad, que su asignación respete ese criterio; REQ-06 puede escribirse desde el administrador (intervención solo en casos aislados). Valida con el equipo el rol cuando haya duda.

**4.2 Acceptance Criteria**
- Mínimo **dos escenarios**: uno normal y uno alternativo o de excepción, en formato **Given / When / Then**, con resultados **observables y verificables**; cada uno con su ID (`AC-XX`).
- Los escenarios no pueden contradecir las preguntas abiertas: si el comportamiento ante un caso (p. ej. no hay cupo en ningún comedor compatible) no está definido en las fuentes, **no inventes el resultado**: el escenario se redacta solo si el equipo definió el comportamiento; si no, va a Open Questions.

**4.3 Use Case**
- Campos: nombre, actor, objetivo, disparador, precondición; flujo principal numerado, flujo alternativo, excepción y postcondición.
- Los casos de uso ya definidos por el equipo **prevalecen**; los demás de la tabla son **hipótesis de partida** a validar.

| Requerimiento | Caso de uso | Actor | Estado |
|---|---|---|---|
| REQ-01 | Ninguno: proceso interno sin interacción directa con el usuario (`N/A` en `REQ-01.md`) | Sistema | Definido por el equipo |
| REQ-02 a REQ-05 | Criterios de la asignación; por defecto sin caso de uso propio (se verifican a través del resultado de REQ-01). Validar con el equipo | Sistema | Hipótesis |
| REQ-06 | Gestionar caso aislado que requiere intervención manual | Administrador | Hipótesis |
| REQ-07 | Registrar reporte de inconformidad o falla | Estudiante beneficiario (y otros usuarios, por confirmar) | Hipótesis |
| REQ-08 | Notificar a administradores | Sistema; recibe el administrador | Hipótesis |
| REQ-09 | Consultar desempeño de comedores | Administrador | Hipótesis |
| REQ-10 | Ver comedores asignados (UC-01) | Estudiante beneficiario | Definido por el equipo |
| REQ-11 | Iniciar sesión (UC-02) | Estudiante beneficiario y administrador | Propuesto en `REQ-11.md` (Draft), por validar |

- **Flow Diagram (Mermaid):** debe reflejar el flujo principal, alternativo y de excepción **del propio requerimiento**. Estilo: **limpio y ordenado, sin flechas encimadas, colores sobrios (nada llamativo) y compacto**; texto **en español**. Usa colores distintos y discretos para flujo principal, alternativo y excepción, y elimina las ramas que no apliquen. Verifica que renderice en GitHub.

### 4.5 Sección 5 — Traceability & Impact

- **Backward:** cadena Evidencia → Necesidad → Requerimiento, apuntando a las fuentes reales (`Requirements.md`, secciones del RFC). Si no hay Discovery Sheet, se escribe `Pendiente` en las referencias de evidencia/necesidad en lugar de crear IDs.
- **Forward:** Requerimiento → Diseño → Implementación → Pruebas. Se expresa en términos de **responsabilidades** (p. ej. "el componente que decide la asignación"), no de tecnologías. Si existe el documento de diseño, se alinea con su sección de trazabilidad; si no, se anota lo que se espera tocar.
- **Impact Analysis:** marca solo las casillas que realmente se verían afectadas si el requerimiento cambia y explica brevemente por qué. Considera las dependencias entre requerimientos (p. ej. un cambio en REQ-01 afecta a REQ-02 a REQ-06 y a REQ-10).

### 4.6 Sección 6 — Validation (Quality Gate)

- Cada casilla se marca **solo si se verificó**. Una casilla sin marcar no es un fallo del documento sino información: se explica qué falta y si pasa a Open Question o requiere volver a elicitar/especificar.
- No marques "Complete enough?" si quedan Open Questions que afectan el comportamiento.
- La verificación de **Atomic?** debe ser real: ver la sección 5 de este archivo.

---

## 5. Calidad del enunciado: puntos a vigilar en los requerimientos actuales

Observaciones tomadas **del texto de `Requirements.md` y de los REQ ya redactados**. No las corrijas en silencio: señálalas al usuario y propón cómo resolverlas. Cambiar un enunciado es decisión del equipo.

| ID | Observación | Cómo tratarla |
|---|---|---|
| REQ-01 | **Resuelto en el enunciado:** `Requirements.md` y `REQ-01.md` fijan que la asignación se mantiene constante hasta el final del semestre. Persiste una tensión con el RFC (reasignación periódica en la entrega 2) y con BR-01 ("frecuencia diaria") | Mantener la lectura de REQ-01; señalar la tensión con el RFC y con BR-01 si afecta al requerimiento redactado. En el archivo `REQ-01.md` quedan por limpiar el valor de Estimate (conserva "High / Medium / Low") y la fila de riesgos (columna de mitigación mal formada) |
| REQ-02 a REQ-05 | Describen **criterios** de la asignación, no funciones independientes: no se pueden verificar sin REQ-01 | Declarar la dependencia de REQ-01 en la sección 2 y verificar cada uno a través del resultado observable de la asignación |
| REQ-04 | Agrupa tres condiciones (capacidad, horario de servicio, menú disponible). Además el RFC no menciona el menú como criterio | Revisar atomicidad; Open Question sobre cómo influye el menú en la asignación |
| REQ-05 | Agrupa discapacidad y preferencia alimentaria. Son datos sensibles | Revisar atomicidad; Constraint/Risk de protección de datos (RFC, Seguridad) |
| REQ-06 | "Casos aislados" no está definido; mezcla automatización con la excepción manual. Decisión del equipo: ante una novedad, la reasignación la hace el administrador manualmente y el sistema no reasigna de forma automática | Open Question: qué es un caso aislado y quién lo decide; revisar atomicidad. Las novedades no se incluyen en REQ-01: se apoyan en REQ-06 a REQ-09 |
| REQ-07 | "Usuarios" no precisa el rol (estudiante, administrador o ambos). El RFC no menciona reportes | Open Question de roles; la evidencia es solo `Requirements.md` |
| REQ-08 | "Se detecte una falla" no indica cómo se detecta ni qué cuenta como falla. El RFC no menciona notificaciones | Open Question; no inventar el mecanismo de detección |
| REQ-09 | "Funcionamiento" no está definido; agrupa capacidad, horario y funcionamiento | Open Question; revisar atomicidad |
| REQ-10 | (a) El **Requirement Statement** de `REQ-10.md` repite el enunciado de REQ-09 (consulta de desempeño por administradores) en lugar del de `Requirements.md`, Requerimiento 10. (b) El encabezado enlaza `US-01`, que está definida en `REQ-01.md`; la historia de `REQ-10.md` no tiene ID propio. (c) La precondición de UC-01 ("los comedores ya fueron asignados") parece contradecir la alternativa "los comedores no han sido asignados aún". (d) Falta definir quién mantiene el periodo de asignación vigente y cómo se identifica al estudiante que consulta (REQ-11, Draft, es la candidata a cubrir la identificación; el equipo debe confirmarlo) | Señalar al equipo; no corregir en silencio. Proponer: enunciado del Requerimiento 10; ID propio para la historia o declarar explícitamente que reutiliza `US-01`; revisar la redacción de la precondición |
| REQ-11 | (a) Credenciales, origen y mantenimiento de las cuentas, y cómo se sabe quién es estudiante beneficiario o administrador, no están definidos. (b) `Requirements.md` escribe "incien sesion" (solo ortografía; se corrige en `REQ-11.md`). (c) "Usuarios" en REQ-07 no precisa roles y REQ-11 nombra solo dos; falta aclarar si existen otros usuarios con sesión. (d) Cierre de sesión, duración e intentos fallidos no se mencionan (políticas de sesión). (e) El RFC no menciona el inicio de sesión; la prioridad Must y la asociación con la entrega 1 son propuestas por validar | Open Questions en `REQ-11.md`; no inventar el mecanismo de autenticación; confirmar con el equipo los IDs (US-02, AC-05, AC-06, UC-02) y la prioridad |
| Conjunto | El RFC lista la **ubicación relativa** del estudiante como criterio de asignación y habla de encuesta, reasignación periódica, estadísticas y cupos por lapso de tiempo; **no hay un requerimiento** en `Requirements.md` para la ubicación | Señalar la posible brecha entre RFC y requerimientos; no crear un requerimiento nuevo sin que el equipo lo pida |
| Conjunto | Las fechas de `REQ-01.md` y `REQ-10.md` (2026-01-11) son anteriores a las de los ADR (2026-10-30) y al diseño de REQ-10; `REQ-11.md` queda con fecha `Pendiente` | Confirmar la fecha real con el equipo antes de usarla como referencia |

---

## 6. Open Questions disponibles

Úsalas en las secciones **Open Questions** de los requerimientos a los que correspondan. No las respondas.

**Del RFC**

| Pregunta del RFC | Requerimientos que probablemente toca |
|---|---|
| ¿Qué ocurre si el aforo de un comedor cambia de forma imprevista (cierre temporal, falla)? ¿Cómo se redistribuye a los estudiantes? | REQ-04, REQ-06, REQ-08 |
| ¿Con qué frecuencia deben actualizarse los datos de los estudiantes (ubicación, horario) y quién los mantiene? | REQ-02, REQ-01 |
| ¿Cómo se ponderan los componentes del árbol decisional cuando entran en conflicto? | REQ-01 a REQ-05 |
| ¿Cómo se manejará a los estudiantes que no completen la encuesta de preferencias? | REQ-03, REQ-05 |
| ¿Qué convenios o permisos se necesitan con las entidades que administran los datos de los estudiantes? | REQ-02, REQ-05, REQ-11 (por confirmar) |
| ¿Cómo se medirá de forma confiable el tiempo real de fila para validar la meta de 40 minutos? | REQ-09 (si el equipo lo asocia) |
| ¿Qué mecanismo tendrá el estudiante para apelar o solicitar el cambio de una asignación? | REQ-06, REQ-07 |

**Ya registradas en los REQ redactados** (se mantienen en esos archivos)

| Pregunta | Archivo |
|---|---|
| ¿Qué se debe hacer cuando no se puede medir acertadamente la capacidad de atención de un comedor? | `REQ-01.md` (relevante también para REQ-04 y REQ-09) |
| ¿Qué representación se usará para el horario de comedores de un estudiante? | `REQ-10.md` |
| Cinco preguntas: credenciales de cada rol; quién crea y mantiene las cuentas y cómo se determina quién es beneficiario o administrador; otros usuarios con sesión (incluidos los "usuarios" de REQ-07); políticas de sesión (duración, cierre e intentos fallidos); aviso y reintento cuando no se pueden verificar las credenciales por una falla de acceso a los datos | `REQ-11.md` (Draft) |

---

## 7. Terminología y forma

- Términos consistentes: "frontend", "backend", "árbol decisional" (sin guion en frontend/backend; el RFC dice "árbol de decisión", se normaliza a "árbol decisional"), "cupos", "comedor", "comedores asignados", "periodo de asignación vigente", "estudiante beneficiario", "administrador", "reporte", "inicio de sesión", "credenciales".
- Los enunciados de `Requirements.md` y las ADR son la fuente de verdad terminológica.
- Cada sección debe ser breve y verificable; sin relleno.
- Nombre de archivo: `REQ-XX.md`, con numeración de dos dígitos, en `docs/requirements/`.

---

## 8. Instrucciones de comportamiento para la IA

**Haz:**
- Redacta **solo lo que el usuario pidió** (un requerimiento, una sección o un cambio puntual), más los cambios estrictamente necesarios que se deriven (p. ej. actualizar las dependencias de otro requerimiento). No generes todos los archivos si solo se pidió uno.
- Basa el contenido en las **fuentes de la sección 2**. Si falta algo esencial (evidencia, reglas de negocio, casos de uso, autores, fecha), **pregúntalo** (una pregunta a la vez cuando sea posible) o márcalo `Pendiente` / Open Question.
- Mantén la **congruencia** entre secciones: enunciado = historia = criterios = caso de uso; riesgos y dependencias coherentes con las ADR y el RFC; trazabilidad con las fuentes reales; IDs de reglas de negocio coherentes con la sección 3.6.
- Mantén la **simplicidad** y el alcance demostrativo del proyecto; no sobredimensiones el requerimiento.
- Señala las ambigüedades y brechas de la sección 5 cuando afecten al requerimiento que se redacta.
- Si el requerimiento nuevo define IDs (US, AC, UC, BR), usa la tabla de la sección 3.7 y actualízala; no reutilices IDs en uso.
- Entrega el contenido listo para copiarse al repositorio.

**No hagas:**
- No uses nada del ejemplo (`REQ-07_Example.md`) como contenido: ni su dominio, ni sus reglas de negocio, evidencias, números, escenarios o diagramas.
- No inventes evidencia, entrevistas, citas, IDs de la Discovery Sheet, reglas de negocio, métricas, estimaciones, prioridades, autores ni fechas.
- No prescribas solución en el enunciado ni en la historia de usuario (nada de PHP, C++, PostgreSQL, Tailwind, "pantalla", "botón", "tabla").
- No cambies el enunciado de `Requirements.md` ni dividas o fusiones requerimientos sin que el usuario lo pida; solo señálalo.
- No agrupes varios comportamientos en un mismo requerimiento sin advertirlo en la validación de atomicidad.
- No definas el comportamiento ante casos no resueltos (sin cupo, falla de un comedor, caso aislado) si las fuentes no lo definen.
- No marques la sección 6 como cumplida si no se verificó.
- No cambies la plantilla (secciones, orden, encabezados) ni dejes el comentario HTML inicial.
- No propongas Emscripten/WebAssembly, ejecución de C++ en el cliente ni frameworks de frontend descartados; no son asunto de los requerimientos, solo aparecen como restricciones cuando aplican.
- No corrijas en silencio los archivos `REQ-01.md` ni `REQ-10.md` ya validados: señala la inconsistencia y propón el cambio.

---

## 9. Cómo pedirle un requerimiento a la IA (entrada esperada)

1. **Requerimiento(s)** a redactar (ID) o **sección(es)** a completar o corregir.
2. **Evidencia o información adicional** que el equipo tenga (Discovery Sheet, reuniones, reglas de negocio, casos de uso ya definidos).
3. **Valores de priorización**, si el equipo los quiere distintos del modelo MoSCoW ya adoptado.
4. **Autores y fecha** (para el encabezado) y estado deseado.
5. Decisiones del equipo sobre las **Open Questions** que ya se resolvieron.

Si falta algo indispensable, la IA pregunta solo por eso o deja el campo como `Pendiente`.

---

## 10. Checklist de revisión antes de entregar

- [ ] Se respetó la plantilla (secciones, orden y encabezados) y se eliminó el comentario inicial.
- [ ] Todo el contenido sale de las fuentes permitidas; nada tomado del ejemplo.
- [ ] El enunciado conserva el significado del Requerimiento **del mismo número** en `Requirements.md`, no es copia de otro y no prescribe solución.
- [ ] El tipo (funcional / no funcional) está justificado con el Perfect Technology Filter.
- [ ] La evidencia citada es real o está marcada `Pendiente`; no hay IDs ni citas inventadas.
- [ ] Context: cada entrada tiene contenido real o `N/A — none identified` con motivo; los IDs de reglas de negocio son coherentes con los de otros REQ.
- [ ] Las restricciones citadas son las ADR que realmente aplican al requerimiento (los 0006–0008 con la salvedad de su estado).
- [ ] La prioridad usa MoSCoW, sin números inventados, y Estimate tiene un único valor.
- [ ] Las tres representaciones (historia, criterios, caso de uso) son coherentes entre sí; hay al menos dos escenarios (normal y alternativo/excepción); los IDs de US y AC existen en el mismo archivo.
- [ ] El diagrama de flujo es limpio, sobrio, compacto, en español y renderiza en GitHub.
- [ ] Los casos no definidos en las fuentes están en Open Questions, no resueltos por suposición.
- [ ] Trazabilidad backward y forward presentes; Impact Analysis marcado con justificación.
- [ ] Validation: solo se marcaron las casillas verificadas; atomicidad evaluada de forma real.
- [ ] Terminología consistente y nombre de archivo `REQ-XX.md`.
- [ ] Lo desconocido está marcado `Pendiente` o `N/A: motivo`, no inventado.
- [ ] Los IDs nuevos (US, AC, UC, BR) no colisionan con la tabla 3.7, y esa tabla se actualizó.

