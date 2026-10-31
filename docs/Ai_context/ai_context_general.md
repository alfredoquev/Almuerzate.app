# AI Context General — almuerzate.app

> Contexto del proyecto para que una IA entienda de qué trata, qué se decidió y qué se espera construir. No impone formato de salida: el formato lo define cada petición. Fuentes: RFC "Almuérzate", ADR 0001–0005 (aceptados) y `Requirements.md`.

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

- **Estudiantes beneficiarios:** consultan el comedor que les fue asignado; esperan una mejora en el servicio.
- **Administradores:** consultan estadísticas y desempeño de los comedores, reciben notificaciones de reportes o fallas y pueden hacer reasignaciones de emergencia.
- **Trabajadores de los comedores:** enfrentan menos saturación en horas pico y pueden ofrecer mejor servicio.

---

## 3. Requerimientos (`Requirements.md`)

| N.º | El sistema debe… |
|---|---|
| 1 | Asignar a cada estudiante beneficiario un comedor para cada día de la semana |
| 2 | Considerar el horario del estudiante al asignar |
| 3 | Considerar la preferencia del estudiante sobre el comedor |
| 4 | Considerar las condiciones del comedor (capacidad, horario de servicio, menú disponible) |
| 5 | Considerar las limitaciones del estudiante (discapacidad, preferencia alimentaria) |
| 6 | Ejecutar la asignación de forma automática, sin intervención manual salvo casos aislados que la requieran |
| 7 | Permitir a los usuarios registrar reportes de inconformidades o fallas |
| 8 | Notificar a los administradores cada vez que se registre un reporte o se detecte una falla |
| 9 | Permitir a los administradores consultar el desempeño de los comedores (capacidad, horario y funcionamiento) |

**Lectura rápida:** los requerimientos 1–6 son el núcleo (asignación); 7–9 cubren reportes, notificación y consulta de desempeño.

---

## 4. Arquitectura y decisiones técnicas (ADR aceptados)

**En una línea:** el backend en C++ (incluido el árbol decisional) corre en el servidor y consulta PostgreSQL; PHP entrega al navegador solo los datos ya procesados que se necesitan para visualizar; el frontend es una aplicación web que los muestra.

| ADR | Decisión | Por qué | Costo aceptado |
|---|---|---|---|
| 0001 | **PostgreSQL** como base de datos principal | Integridad transaccional (ACID) para asignar cupos, buen rendimiento y soporte maduro desde C++ (p. ej. `libpqxx`), consultas complejas y datos agregados para paneles | Servidor de BD independiente; requiere diseño cuidadoso del esquema e indexación |
| 0002 | **PHP** conecta frontend y backend: corre en el servidor, invoca el ejecutable C++ (árbol decisional) y envía al cliente solo los datos requeridos | Reutiliza el C++ existente sin reescribirlo, integración simple y separación de responsabilidades (backend analiza y gestiona datos; frontend visualiza) | El equipo debe aprender PHP; cada solicitud lanza un proceso (puede penalizar el rendimiento con alto flujo de datos); hay que validar los argumentos que PHP pasa al ejecutable para evitar inyección de comandos |
| 0003 | **Tailwind CSS** para la maquetación e interfaz | Iteración rápida, curva de aprendizaje baja, ecosistema amplio e integración sencilla con librerías JS de gráficos como Chart.js | Acoplamiento al entorno web y necesidad de una capa de comunicación JS ↔ C++ (PHP, ADR-0002) |
| 0004 | **C++** para el backend y el árbol decisional, ejecutado en el servidor | Buen manejo de estructuras de datos y grandes volúmenes de datos por el control de memoria; el equipo ya conoce el lenguaje | Complejidad en la implementación de estructuras de datos; conexión con el frontend vía PHP |
| 0005 | **Frontend web con HTML, JavaScript y CSS** (mantiene Tailwind) | Accesible desde navegador sin instalar app móvil; alcance de interfaz acotado y datos que llegan ya procesados | Se renuncia a herramientas de React/Vue; más código propio que escribir y mantener |

### Alternativas descartadas
- **Base de datos:** MySQL/MariaDB, SQLite, MongoDB.
- **Integración:** Emscripten (obligaría a ejecutar C++ en el cliente) y bibliotecas C++ (acoplan la conexión a C++).
- **Interfaz:** Slint y Dear ImGui.
- **Backend:** Java (el equipo no lo domina).
- **Frontend:** React, Vue y React Native.

### Implicaciones prácticas
- El C++ **no** se ejecuta en el cliente.
- El navegador nunca habla directo con el backend: pasa por PHP.
- Las gráficas y paneles estadísticos usan librerías JS (Chart.js) alimentadas con datos del backend.
- La persistencia es relacional y transaccional.

---

## 5. Plan de entregas (RFC)

1. **Entrega 1:** aplicación en línea donde el estudiante consulta el comedor asignado; asignación "inteligente" según información personal y características del estudiante.
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
- **Riesgos técnicos aceptados (ADR):** costo por ejecución de PHP al lanzar un proceso por solicitud, validación de argumentos contra inyección de comandos y curva de aprendizaje de PHP.

**Dependencias**
- Estado actual de los comedores (infraestructura y capacidad).
- Entidades que administran los datos de los estudiantes relevantes para la asignación.

**Alternativas al sistema (descartadas en el RFC):** asignación aleatoria (ignora las características de cada estudiante) y mantener el sistema actual (no ha resuelto saturación ni filas).

---

## 8. Preguntas abiertas del RFC y lo que dicen las fuentes

Cada pregunta se responde solo con lo que ya está en el RFC, los ADR y los requerimientos. Cuando las fuentes no lo definen, se indica como **Sin definir** en lugar de suponer una respuesta.

**1. ¿Cómo se redistribuye a los estudiantes cuando el aforo cambia de forma imprevista (cierre temporal, falla en el servicio)?** — *Parcialmente respondida*
- La falla se registra mediante un reporte (Req. 7) o se detecta, y el sistema notifica a los administradores (Req. 8).
- La redistribución la hacen los administradores mediante **reasignaciones de emergencia** (RFC, entrega final), que corresponden a los casos aislados con intervención manual (Req. 6).
- Los administradores pueden consultar capacidad, horario y funcionamiento de los comedores (Req. 9) para decidir.
- Sin definir: el criterio con el que se redistribuye y si el sistema propone alguna reasignación.

**2. ¿Con qué frecuencia se actualizan los datos de los estudiantes (ubicación, horario) y quién los mantiene?** — *Pendiente*

**3. ¿Cómo se ponderan los componentes del árbol de decisión cuando entran en conflicto?** — *Parcialmente respondida*
- El aforo es un tope físico que el sistema no puede superar (RFC, riesgos), por lo que actúa como límite y no como un criterio más.
- Los criterios a considerar son horario, preferencia de comedor, condiciones del comedor y limitaciones del estudiante (Req. 2 a 5), más la ubicación relativa (RFC).
- Sin definir: el peso o la prioridad entre estos criterios cuando se contradicen.

**4. ¿Cómo se trata a los estudiantes que no completen la encuesta de preferencias?** — *Parcialmente respondida*
- La encuesta se incorpora en la entrega 2; en la entrega 1 la asignación ya se hace con la información personal y las características del estudiante, sin depender de ella.
- Sin definir: qué ocurre con quien no la responda una vez exista (por ejemplo, si se mantiene la asignación basada en su información personal).

**5. ¿Qué convenios o permisos se necesitan con las entidades que administran los datos de los estudiantes?** — *Sin definir*
- Las fuentes solo establecen que estas entidades son una dependencia del sistema y que el acceso a los datos debe estar restringido y protegido (RFC, seguridad).
- No se especifican convenios ni permisos concretos.

**6. ¿Cómo se mide de forma confiable el tiempo real de fila para validar la meta de 40 minutos?** — *Pendiente*

**7. ¿Qué mecanismo tiene el estudiante para apelar o solicitar el cambio de una asignación?** — *Parcialmente respondida*
- El estudiante puede registrar un reporte de inconformidad o falla (Req. 7), que se notifica a los administradores (Req. 8).
- El administrador atiende los casos aislados que requieren intervención manual (Req. 6) y puede reasignar (RFC, reasignaciones de emergencia).
- El porcentaje de estudiantes que piden cambio o reasignación se usa como métrica de calidad (RFC).
- Sin definir: un proceso formal de apelación (plazos, criterios de aceptación, respuesta al estudiante).

**Resumen:** 4 preguntas con respuesta parcial (1, 3, 4, 7), 2 pendientes (2, 6) y 1 sin definir en las fuentes (5). Lo pendiente o sin definir queda abierto hasta que el equipo lo decida.

---

## 9. Notas de alcance

- El RFC incluye la **ubicación relativa** como criterio y menciona encuesta, reasignación periódica y estadísticas por entregas; `Requirements.md` aún no contiene requerimientos explícitos para ubicación ni para la encuesta.
- El proyecto es de escala acotada: se prefiere una integración simple y no sobredimensionar la solución.
- Terminología usada en las fuentes: comedor, cupos, aforo, estudiante beneficiario, administrador, árbol decisional (o árbol de decisión), frontend, backend.
