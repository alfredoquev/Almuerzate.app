# AI Context General — almuerzate.app

> Contexto del proyecto para que una IA entienda de qué trata, qué se decidió y qué se espera construir. No impone formato de salida: el formato lo define cada petición. Fuentes: RFC "Almuérzate", ADR 0001–0008 (aceptados), ADR-0009 (propuesto), `Requirements.md`, los requerimientos ya redactados (`REQ-01.md`, `REQ-10.md` y el borrador `REQ-11.md`) y los diseños arquitectónicos y detallados de REQ-10 y REQ-11.
>
> Última actualización: 2026-11-28.

---

## 1. Qué es el proyecto

**Almuérzate** es un sistema de asignación de comedores para estudiantes beneficiarios de la **Universidad Nacional de Colombia**. Equipo: @amadodev07, @juanfpg1, @xedroc422, @alfredoquev y @julondono07.

**Problema:** algunos comedores están saturados mientras otros tienen un aforo mínimo. Consecuencias: filas interminables, estudiantes que llegan tarde a clase y descontento general en la comunidad estudiantil.

**Premisa:** distribuir mejor a los estudiantes entre los comedores reducirá el tiempo de espera y mejorará el servicio.

**Cómo se asigna:** mediante un **árbol de decisión** (árbol decisional) que determina el mejor comedor para cada estudiante según varios criterios.

### Criterios de asignación (RFC)
- Aforo de cada comedor
- Preferencias de comedor del estudiante
- Discapacidades del estudiante (si las hay)
- Horario del estudiante cada día
- Preferencias alimentarias del estudiante
- Ubicación relativa del estudiante respecto a cada comedor

---

## 2. Actores e impacto

- **Estudiantes beneficiarios:** consultan el comedor que les fue asignado; inician sesión (Req. 11); esperan una mejora en el servicio.
- **Administradores:** consultan estadísticas y desempeño de los comedores, reciben notificaciones de reportes o fallas, pueden hacer reasignaciones de emergencia e inician sesión (Req. 11).
- **Trabajadores de los comedores:** enfrentan menos saturación en horas pico y pueden ofrecer mejor servicio.
- **Sistema (asignación automática):** actor interno para las funciones que se ejecutan sin intervención humana (Req. 1 y 6).

---

## 3. Requerimientos (`Requirements.md`)

| N.º | El sistema debe… |
|---|---|
| 1 | Asignar a cada estudiante beneficiario un comedor para cada día de la semana, y mantener esa asignación constante hasta el final del semestre |
| 2 | Considerar el horario del estudiante al asignar |
| 3 | Considerar la preferencia del estudiante sobre el comedor |
| 4 | Considerar las condiciones del comedor (capacidad, horario de servicio, menú disponible) |
| 5 | Considerar las limitaciones del estudiante (discapacidad, preferencia alimentaria) |
| 6 | Ejecutar la asignación de forma automática, sin intervención manual salvo casos aislados que la requieran |
| 7 | Permitir a los usuarios registrar reportes de inconformidades o fallas |
| 8 | Notificar a los administradores cada vez que se registre un reporte o se detecte una falla |
| 9 | Permitir a los administradores consultar el desempeño de los comedores (capacidad, horario y funcionamiento) |
| 10 | Mostrar a los estudiantes beneficiarios sus asignaciones de comedor para el periodo de asignación vigente |
| 11 | Permitir que el estudiante beneficiario y el administrador inicien sesión en la página de Almuérzate |

**Lectura rápida:** los requerimientos 1–6 son el núcleo (asignación); 7–9 cubren reportes, notificación y consulta de desempeño; 10 es la consulta del estudiante a lo que el núcleo asignó (solo lee, no calcula); 11 es el inicio de sesión de estudiantes beneficiarios y administradores (transversal: no asigna ni consulta asignaciones, y cubre la identificación de quien usa el sistema).

### Estado de la documentación

| Documento | Estado |
|---|---|
| `REQ-01.md` | Validated |
| `REQ-10.md` | Validated (se alimenta solo de REQ-01) |
| `REQ-11.md` | Draft (autores y fecha Pendiente; IDs US-02, AC-05, AC-06 y UC-02 propuestos; prioridad Must propuesta) |
| `REQ-02` a `REQ-09` | Aún no redactados |
| Diseño arquitectónico y detallado de REQ-10 (v1.0) | Draft; cubre REQ-10 y toma REQ-01 solo como productor de los datos (el inicio de sesión se diseña por separado en REQ-11) |
| Diseño arquitectónico y detallado de REQ-11 (v1.0) | Draft; cubre el inicio de sesión de estudiantes beneficiarios y administradores (3 componentes, 12 clases), alineado con ADR-0007, ADR-0008 y ADR-0009 |
| ADR 0001–0008 | Aceptados (0001–0005: 2026-10-30; 0006: fecha pendiente; 0007 y 0008: 2026-11-22) |
| ADR-0009 | Propuesto; fecha 2026-11-28 |

### Reglas de negocio ya definidas por el equipo
- **BR-01:** la asignación de comedores a los estudiantes beneficiarios debe tener frecuencia diaria.
- **BR-02:** las asignaciones solo se pueden realizar mientras se esté cursando un semestre académico.
- REQ-11 no define reglas de negocio nuevas.

---

## 4. Arquitectura y decisiones técnicas (ADR 0001–0008 aceptados; ADR-0009 propuesto)

**En una línea:** el backend en C++ (incluido el árbol decisional) se compila como librería compartida que PHP carga en el servidor mediante FFI (ADR-0007) y consulta PostgreSQL ejecutando un script PHP que devuelve JSON (ADR-0008); PHP entrega al navegador solo los datos ya procesados que se necesitan para visualizar; el frontend es una aplicación web que los muestra.

| ADR | Decisión | Por qué | Costo aceptado |
|---|---|---|---|
| 0001 | **PostgreSQL** como base de datos principal | Integridad transaccional (ACID) para asignar cupos, buen rendimiento y soporte maduro desde C++ (p. ej. `libpqxx`), consultas complejas y datos agregados para paneles | Servidor de BD independiente; requiere diseño cuidadoso del esquema e indexación |
| 0002 | **PHP** conecta frontend y backend: corre en el servidor y envía al cliente solo los datos requeridos *(su mecanismo de invocación por ejecutable fue reemplazado por ADR-0007)* | Reutiliza el C++ existente sin reescribirlo, integración simple y separación de responsabilidades (backend analiza y gestiona datos; frontend visualiza) | El equipo debe aprender PHP |
| 0003 | **Tailwind CSS** para la maquetación e interfaz | Iteración rápida, curva de aprendizaje baja, ecosistema amplio e integración sencilla con librerías JS de gráficos como Chart.js | Acoplamiento al entorno web y necesidad de una capa de comunicación JS ↔ C++ (PHP, ADR-0002) |
| 0004 | **C++** para el backend y el árbol decisional, ejecutado en el servidor | Buen manejo de estructuras de datos y grandes volúmenes de datos por el control de memoria; el equipo ya conoce el lenguaje | Complejidad en la implementación de estructuras de datos; conexión con el frontend vía PHP |
| 0005 | **Frontend web con HTML, JavaScript y CSS** (mantiene Tailwind) | Accesible desde navegador sin instalar app móvil; alcance de interfaz acotado y datos que llegan ya procesados | Se renuncia a herramientas de React/Vue; más código propio que escribir y mantener |
| 0006 | **Consulta de Asignaciones como único punto de acceso a los comedores asignados**: Presentación de Asignaciones no accede directamente a Registro de Asignaciones | Los cuatro resultados del flujo de REQ-10 (disponible, no asignados aún, sin periodo vigente, problema de comunicación) y los fallos de acceso a los datos se resuelven en un solo componente | Un componente y un salto adicionales para una función de solo lectura (algo más de latencia); el estado de problema de comunicación puede originarse en dos puntos (fallo de acceso a los datos y falta de respuesta de Consulta de Asignaciones) y debe tratarse igual |
| 0007 | **Librería compartida de C++**: el backend se compila como librería (`.dll` en Windows) que PHP carga mediante FFI; reemplaza el mecanismo de invocación del ADR-0002 | La implementación real carga una librería y no invoca un ejecutable; no se lanza un proceso del backend por solicitud y PHP recibe directamente el JSON | Requiere habilitar FFI en PHP; una librería por sistema operativo; firmas declaradas en PHP sincronizadas con el C++ (un desajuste puede tumbar el proceso); liberación manual de la memoria reservada por C++ |
| 0008 | **Script PHP para acceder a PostgreSQL**: el backend C++ ejecuta el script que consulta la base de datos y devuelve JSON | Acceso a la base de datos más sencillo de aprender y de entender que una conexión directa con `libpqxx` | Cada consulta lanza un proceso `php` desde C++; el flujo pasa por PHP dos veces (PHP → C++ → PHP); el backend depende de que `php` esté en el servidor; se deja de aprovechar `libpqxx` (ADR-0001) |
| 0009 *(Propuesto)* | **Verificación de Identidad como único punto de acceso para el inicio de sesión**: Presentación de Inicio de Sesión no accede directamente a Registro de Cuentas ni valida credenciales | Centraliza la validación de entradas, los tres resultados de UC-02 (sesión iniciada, credenciales no válidas y problema de comunicación) y la determinación de rol en un solo componente (mismo patrón que ADR-0006) | Un componente y un salto adicionales cruzando la frontera de integración (ADR-0007 y ADR-0008); el estado de problema de comunicación puede originarse en dos puntos y debe tratarse de forma uniforme |

### Alternativas descartadas
- **Base de datos:** MySQL/MariaDB, SQLite, MongoDB.
- **Integración:** Emscripten (obligaría a ejecutar C++ en el cliente; el ADR-0007 dice que podría volver a evaluarse, pero no está habilitado) y bibliotecas C++ (acoplan la conexión a C++).
- **Invocación del backend (ADR-0007):** ejecutable invocado por PHP (mecanismo del ADR-0002, que el 0007 reemplaza).
- **Acceso a la base de datos (ADR-0008):** conexión directa desde C++ con `libpqxx` (más compleja de aprender y entender).
- **Interfaz:** Slint y Dear ImGui.
- **Backend:** Java (el equipo no lo domina).
- **Frontend:** React, Vue y React Native.
- **Consulta de asignaciones (ADR-0006):** que la presentación consulte directamente al registro de asignaciones (cada resultado y cada fallo tendrían que resolverse en la interfaz).
- **Inicio de sesión (ADR-0009, propuesto):** que la presentación consulte directamente a Registro de Cuentas y valide credenciales en la interfaz (dispersaría validaciones, fallos y riesgos de seguridad en la presentación).

### Implicaciones prácticas
- El C++ **no** se ejecuta en el cliente.
- El navegador nunca habla directo con el backend: pasa por PHP.
- Las gráficas y paneles estadísticos usan librerías JS (Chart.js) alimentadas con datos del backend.
- La persistencia es relacional y transaccional.
- La consulta del estudiante a sus comedores asignados pasa siempre por Consulta de Asignaciones (ADR-0006).
- No se lanza un proceso del backend por solicitud (el costo del ADR-0002 deja de aplicar a la invocación PHP → C++), pero PHP debe tener FFI habilitado (`ffi.enable=true`), hay que compilar la librería por sistema operativo, mantener las firmas sincronizadas y liberar manualmente la memoria reservada por C++ (ADR-0007).
- El backend no se conecta directamente a PostgreSQL; ejecuta un script PHP por consulta que devuelve JSON (ADR-0008). Los componentes del diseño y sus capacidades se nombran por responsabilidad, no por tecnología.
- Inicio de sesión (REQ-11, diseño v1.0 Draft): el navegador no habla directo con el backend; Presentación de Inicio de Sesión cruza la frontera FFI/JSON hacia Verificación de Identidad (ADR-0007 y ADR-0009), que valida las entradas antes de lanzar el subproceso `php` hacia PostgreSQL (ADR-0008). Siguen sin definir las credenciales concretas de cada rol, quién crea/mantiene las cuentas y cómo se conserva la sesión activa entre solicitudes.

### Componentes del diseño ya definidos (diseños de REQ-10 y REQ-11 v1.0)
Los nombres siguen la responsabilidad, no la tecnología. El detalle y las clases están en `ai_context_design.md` y en los documentos de diseño de cada requerimiento.

| Componente | Requerimiento | Responsabilidad |
|---|---|---|
| Presentación de Asignaciones | REQ-10 | Muestra al estudiante sus comedores asignados o el mensaje que corresponde |
| Consulta de Asignaciones | REQ-10 | Decide qué debe ver el estudiante: sus asignaciones o la razón por la que no están disponibles |
| Registro de Asignaciones | REQ-10 | Persiste y entrega las asignaciones y el periodo de asignación vigente |
| Cálculo de Asignaciones | REQ-01 (contexto de REQ-10) | Calcula y asigna un comedor por día (REQ-01); su diseño interno no se detalla en el diseño de REQ-10 |
| Presentación de Inicio de Sesión | REQ-11 | Recibe credenciales en la página web, solicita la verificación (vía FFI/JSON), habilita el acceso según rol o muestra el aviso correspondiente y permite reintentar |
| Verificación de Identidad | REQ-11 | Único punto de acceso para validar credenciales, consultar la cuenta, determinar el rol y decidir el resultado del inicio de sesión (ADR-0009) |
| Registro de Cuentas | REQ-11 | Accede a la persistencia de cuentas en PostgreSQL (vía script PHP/JSON, ADR-0008) y entrega la cuenta registrada con su identidad y rol |

> El diseño de REQ-11 cubre la verificación de identidad y rol, pero sigue abierta la brecha de cómo se conserva la sesión activa entre solicitudes web y cómo `Consulta de Asignaciones` (REQ-10) recibe de forma segura la identidad del estudiante autenticado (`IdUsuario`).

---

## 5. Plan de entregas (RFC)

1. **Entrega 1:** aplicación en línea donde el estudiante consulta el comedor asignado (Req. 10); asignación "inteligente" según información personal y características del estudiante (Req. 1 a 6). El RFC no menciona el inicio de sesión (Req. 11); su asociación con esta entrega es una hipótesis.
2. **Entrega 2:** encuesta para conocer necesidades y preferencias; sistema de reasignación periódica; mejora del proceso de asignación de cupos.
3. **Entrega final:** sistema de estadísticas para administradores; reasignaciones de emergencia; diferenciación en la asignación de cupos según el lapso de tiempo que el estudiante elija (a partir de la encuesta).

---

## 6. Métricas de éxito (RFC)

| Qué se mide | Objetivo |
|---|---|
| Estandarización del aforo | Que ningún comedor esté saturado ni vacío; monitoreo de ocupación por comedor y franja horaria, y diferencia entre comedores |
| Tiempo de fila | Menos de 40 minutos, para acceder al servicio dentro de la hora de almuerzo establecida por la universidad |
| Desempeño del sistema | Tiempo de respuesta de la asignación y de la consulta del comedor; usuarios concurrentes en horas pico |
| Calidad de la asignación | Porcentaje de estudiantes que piden cambio o reasignación (indicador indirecto de asignaciones erróneas) |

---

## 7. Riesgos y dependencias

**Riesgos**
- **Asignación errónea:** si el árbol decisional asigna mal, pueden aumentar los tiempos de desplazamiento y de fila.
- **Límites físicos:** el aforo real de cada comedor es un tope que el sistema no puede superar.
- **Fuga de datos personales:** se manejan ubicación, condiciones y preferencias; el acceso debe estar restringido y protegido (es el punto más sensible).
- **Riesgos técnicos aceptados (ADR):** curva de aprendizaje de PHP (ADR-0002); requisitos de FFI habilitado, una librería compartida por sistema operativo, sincronización de firmas entre PHP y C++ (un desajuste puede tumbar el proceso) y liberación manual de memoria (ADR-0007); lanzamiento de un proceso `php` desde C++ por cada consulta a PostgreSQL, doble paso por PHP (`PHP → C++ → PHP`) y necesidad de validar argumentos pasados al subproceso contra inyección de comandos (ADR-0008).
- **Error de acceso a los datos de asignación (REQ-10):** probabilidad Low, impacto High; mitigación: se muestra un mensaje de error y se da al estudiante la opción de reintentar.
- **Capacidad mal estimada (REQ-01):** estudiantes no beneficiarios afectan el aforo real y el sistema no lo mide; probabilidad Alta, impacto Medio.
- **Acceso no autorizado y entrada del usuario (REQ-11, Draft):** verificación de identidad deficiente que exponga datos de estudiantes (relacionado con la fuga de datos personales del RFC), inyección de comandos al invocar el subproceso `php` desde C++ con lo que la persona escribe al iniciar sesión (ADR-0008), fallos en el borde FFI/JSON (ADR-0007) y falla de acceso a los datos que impida iniciar sesión. Probabilidad e impacto: estimación del equipo a validar.

**Dependencias**
- Estado actual de los comedores (infraestructura y capacidad).
- Entidades que administran los datos de los estudiantes relevantes para la asignación.
- Momento en que entran en efecto los subsidios de alimentación en la Universidad Nacional de Colombia sede Bogotá (REQ-01).
- REQ-10 depende de REQ-01 (las asignaciones deben existir y estar persistidas).
- REQ-11 (por confirmar): REQ-10 necesita saber qué estudiante consulta; REQ-07 y REQ-09 también involucran personas identificadas.
- REQ-11 depende de la información sobre quiénes son estudiantes beneficiarios (entidades que administran esos datos); el origen de la información de los administradores está sin definir.

**Alternativas al sistema (descartadas en el RFC):** asignación aleatoria (ignora las características de cada estudiante) y mantener el sistema actual (no ha resuelto saturación ni filas).

---

## 8. Preguntas abiertas del RFC y lo que dicen las fuentes

Cada pregunta se responde solo con lo que ya está en el RFC, los ADR y los requerimientos. Cuando las fuentes no lo definen, se indica como **Sin definir** en lugar de suponer una respuesta.

**1. ¿Cómo se redistribuye a los estudiantes cuando el aforo cambia de forma imprevista (cierre temporal, falla en el servicio)?** — *Parcialmente respondida*
- La falla se registra mediante un reporte (Req. 7) o se detecta, y el sistema notifica a los administradores (Req. 8).
- La redistribución la hacen los administradores mediante **reasignaciones de emergencia** (RFC, entrega final), que corresponden a los casos aislados con intervención manual (Req. 6). El equipo decidió que el sistema no reasigna automáticamente ante una novedad.
- Los administradores pueden consultar capacidad, horario y funcionamiento de los comedores (Req. 9) para decidir.
- Sin definir: el criterio con el que se redistribuye y si el sistema propone alguna reasignación.

**2. ¿Con qué frecuencia se actualizan los datos de los estudiantes (ubicación, horario) y quién los mantiene?** — *Pendiente*

**3. ¿Cómo se ponderan los componentes del árbol de decisión cuando entran en conflicto?** — *Parcialmente respondida*
- El aforo es un tope físico que el sistema no puede superar (RFC, riesgos), por lo que actúa como límite y no como un criterio más. REQ-01 lo recoge como restricción: un comedor lleno no se puede asignar.
- Los criterios a considerar son horario, preferencia de comedor, condiciones del comedor y limitaciones del estudiante (Req. 2 a 5), más la ubicación relativa (RFC).
- Sin definir: el peso o la prioridad entre estos criterios cuando se contradicen.

**4. ¿Cómo se trata a los estudiantes que no completen la encuesta de preferencias?** — *Parcialmente respondida*
- La encuesta se incorpora en la entrega 2; en la entrega 1 la asignación ya se hace con la información personal y las características del estudiante, sin depender de ella.
- Sin definir: qué ocurre con quien no la responda una vez exista (por ejemplo, si se mantiene la asignación basada en su información personal).

**5. ¿Qué convenios o permisos se necesitan con las entidades que administran los datos de los estudiantes?** — *Sin definir*
- Las fuentes solo establecen que estas entidades son una dependencia del sistema y que el acceso a los datos debe estar restringido y protegido (RFC, seguridad).
- No se especifican convenios ni permisos concretos. Es probable que también afecte a REQ-11 (cómo se sabe quién es estudiante beneficiario); por confirmar.

**6. ¿Cómo se mide de forma confiable el tiempo real de fila para validar la meta de 40 minutos?** — *Pendiente*

**7. ¿Qué mecanismo tiene el estudiante para apelar o solicitar el cambio de una asignación?** — *Parcialmente respondida*
- El estudiante puede registrar un reporte de inconformidad o falla (Req. 7), que se notifica a los administradores (Req. 8).
- El administrador atiende los casos aislados que requieren intervención manual (Req. 6) y puede reasignar (RFC, reasignaciones de emergencia).
- El porcentaje de estudiantes que piden cambio o reasignación se usa como métrica de calidad (RFC).
- Sin definir: un proceso formal de apelación (plazos, criterios de aceptación, respuesta al estudiante).

**Resumen:** 4 preguntas con respuesta parcial (1, 3, 4, 7), 2 pendientes (2, 6) y 1 sin definir en las fuentes (5). Lo pendiente o sin definir queda abierto hasta que el equipo lo decida.

### Preguntas abiertas propias de los requerimientos ya redactados
- **REQ-01:** ¿qué se debe hacer cuando no se puede medir acertadamente la capacidad de atención de un comedor?
- **REQ-10:** ¿qué representación visual se usará para el horario de comedores de un estudiante? (el diseño solo fija la representación lógica: día + comedor).
- **REQ-11:** credenciales de cada rol; quién crea y mantiene las cuentas y cómo se determina quién es estudiante beneficiario o administrador; otros usuarios con sesión (estudiantes no beneficiarios, trabajadores de los comedores, los "usuarios" de REQ-07); políticas de sesión (duración, cierre e intentos fallidos); formalizar en `REQ-11.md` el comportamiento de reintento ante falla de acceso a los datos ya adoptado en el diseño v1.0.

---

## 9. Notas de alcance

- El RFC incluye la **ubicación relativa** como criterio y menciona encuesta, reasignación periódica y estadísticas por entregas; `Requirements.md` aún no contiene requerimientos explícitos para ubicación ni para la encuesta.
- El proyecto es de escala acotada: se prefiere una integración simple y no sobredimensionar la solución.
- Terminología usada en las fuentes: comedor, cupos, aforo, estudiante beneficiario, administrador, árbol decisional (o árbol de decisión), frontend, backend, periodo de asignación vigente, comedores asignados.
- **Brechas abiertas entre ADR aceptados (0001–0008):** el 0007 reemplaza el mecanismo de invocación del 0002 pero el 0002 sigue `Aceptado` y la plantilla no admite un reemplazo parcial; el 0007 cita como beneficio no lanzar un proceso por solicitud mientras el 0008 lanza un proceso `php` por consulta; el 0008 deja de aprovechar `libpqxx` (ADR-0001); el sistema operativo del servidor no está definido (el 0007 menciona `.dll` de Windows); ADR-0006 mantiene su fecha como `Pendiente`.
- **Alcance temporal de la asignación:** REQ-01 fija un comedor por día (lunes a viernes), igual para todas las semanas del semestre. El RFC menciona reasignación periódica (entrega 2); la relación entre ambos sigue sin definir.
- **Brechas abiertas entre REQ-01, REQ-10 y REQ-11** (se señalan, no se resuelven aquí):
  - Quién mantiene el periodo de asignación vigente y dónde vive.
  - Cómo se conserva la sesión activa y cómo recibe `Consulta de Asignaciones` (REQ-10) la identidad del estudiante autenticado por REQ-11 (`IdUsuario`) sin confiar en datos manipulables desde el navegador.
  - Cómo se relacionan BR-01 (frecuencia diaria) y la asignación fija del semestre con el periodo "semana en curso" de REQ-10.
  - El enunciado del archivo `REQ-10.md` repite el de REQ-09 (consulta de desempeño); el enunciado correcto es el de `Requirements.md`, Requerimiento 10.
- **Brechas abiertas de REQ-11** (se señalan, no se resuelven aquí): el RFC no menciona el inicio de sesión, por lo que su evidencia es `Requirements.md` y, de forma indirecta, la sección Seguridad del RFC; `Requirements.md` conserva el error ortográfico "incien sesion"; REQ-07 habla de "usuarios" sin precisar roles mientras REQ-11 nombra solo dos; la prioridad Must, los IDs (US-02, AC-05, AC-06, UC-02) y la asociación con la entrega 1 son propuestas por validar con el equipo.
