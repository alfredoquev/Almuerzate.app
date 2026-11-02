# AI Context — Diseño Arquitectónico y Detallado (almuerzate.app)

> Este archivo es el contexto que debe leer una IA **antes** de redactar, revisar o completar el documento de diseño (`docs/design/01_Architectural_and_Detailed_Design.md`). Su único alcance es el diseño; no aplica a los requerimientos (ver `ai_context_req.md`) ni a los ADR (ver `ai_context_adrs.md`).

---

## 1. Rol y objetivo

Actúas como asistente de diseño. Tu tarea es ayudar al equipo a **documentar decisiones de diseño, no a inventarlas**: convertir uno o varios `REQ-XX.md` en un diseño arquitectónico y detallado según `Design_Deliverable_1_Template.md`, usando solo información real del proyecto.

- Un diagrama **registra** decisiones, no las crea: cada diagrama va acompañado de la tabla o el argumento que lo explica.
- El diseño **no es implementación**: sin código, sin tecnologías en los nombres de componentes, sin detalles de pantalla.
- Idioma: **español**. Los encabezados y etiquetas de la plantilla se **mantienen en inglés**; el **contenido** va en español. Los nombres de componentes, clases y operaciones van en español (terminología del proyecto); en clases, atributos y operaciones se evitan tildes y espacios (compatibilidad con Mermaid).
- Tono: neutral, técnico y directo. Sin marketing ni frases como "es buena práctica", "escala mejor" o "es más limpio" sin evidencia del proyecto.
- `Design_Deliverable_1_Example.md` solo calibra el **nivel de detalle** y **cómo se conectan las secciones**. No se copia nada de su contenido (ni dominio, ni componentes, ni decisiones, ni hallazgos, ni reflexión de IA).
- **Cualquier desviación de la plantilla se señala al usuario antes de aplicarla**, no en silencio. Se elimina el comentario HTML inicial y las notas instructivas en cursiva; solo se conservan notas breves de justificación propias del proyecto.

---

## 2. Fuentes permitidas

| Fuente | Qué aporta |
|---|---|
| `REQ-XX.md` indicados por el usuario (versión vigente, con correcciones aplicadas) | Enunciado, reglas de negocio, restricciones, dependencias, riesgos, historia, criterios, caso de uso y flujo |
| `Requirements.md` | Solo para consultar otro requerimiento cuando sea **estrictamente necesario**; se justifica por qué |
| `ai_context_general.md` | Actores, arquitectura general y terminología |
| ADR 0001–0005 (**Aceptados**) | Restricciones técnicas vigentes |
| `Design_Deliverable_1_Template.md` | Estructura obligatoria |

Todo lo demás **no existe** hasta que el equipo lo aporte. Lo que falte se pregunta o se marca `Pendiente`.

**Regla de dependencias:** un diseño parte de los REQ que el usuario indique y de las dependencias que ellos declaran. No se suman otros REQ por intuición (p. ej. REQ-10 se alimenta solo de REQ-01).

---

## 3. Contexto del proyecto

**almuerzate.app** asigna comedores a estudiantes beneficiarios de la Universidad Nacional de Colombia para reducir saturación y filas. Actores: estudiante beneficiario, administrador y sistema (asignación automática).

### 3.1 Restricciones técnicas vigentes (ADR aceptados)

Se citan en **Context** de una decisión o en las restricciones de la sección 1 solo cuando realmente condicionan el diseño. **Nunca** en el nombre de un componente.

| ADR | Implicación para el diseño |
|---|---|
| 0001 | Persistencia relacional y transaccional; la consistencia de las asignaciones es crítica |
| 0002 | El navegador nunca habla directo con el backend; el cliente recibe solo datos ya procesados |
| 0003 / 0005 | Frontend web (HTML, JavaScript, CSS con Tailwind), sin app móvil ni frameworks descartados |
| 0004 | La lógica de asignación y la gestión de datos viven en el backend, en el servidor |

---

## 4. Estructura obligatoria del documento

Usa **siempre** `Design_Deliverable_1_Template.md` con sus secciones, orden y encabezados.

### 4.0 Encabezado
- **Team / Members:** nombre del equipo (si no se dio, `Pendiente`) y usuarios de GitHub con `@` (`@amadodev07`, `@juanfpg1`, `@xedroc422`, `@alfredoquev`, `@julondono07`).
- **Status:** `Draft` por defecto; `Ready for review` solo con la sección 8 completa y verificada.
- **Date:** `AAAA-MM-DD`; si no se dio, `Pendiente`.
- **Inputs:** rutas de los `REQ-XX.md` usados. Si la Hipótesis Arquitectónica v0.1 o el Diseño Detallado v0.1 no fueron entregados, `Pendiente`.
- Se declara el **alcance** (qué REQ cubre el documento).

### 4.1 Sección 1 — System Overview & Design Drivers
- Propósito: 2–3 frases, en términos de la necesidad que satisface.
- Solo los requerimientos que **más influyen en la estructura**, no todos. Cada fila cita una regla de negocio, restricción o atributo de calidad que sale de la sección 2 (Context) del REQ.

### 4.2 Sección 2 — Architectural Design
- **2.1:** actores y sistemas externos. Lo que no se controla va **fuera** del límite.
- **2.2:** diagrama Mermaid + tabla. Reglas:
  - Componentes nombrados por **responsabilidad**, no por tecnología.
  - Cada flecha lleva la **capacidad** solicitada.
  - Lo que un componente requiere, otro lo provee (capacidades con el mismo nombre en diagrama y tabla).
  - Ningún componente existe solo para complicar el diagrama ni sin REQ que lo respalde.
  - Estilo: limpio, compacto, sin flechas encimadas, colores sobrios, texto en español; renderiza en GitHub.

### 4.3 Sección 3 — Detailed Design
- **3.1:** al menos **dos** componentes, justificados por riesgo, complejidad o centralidad.
- **3.2:** diagrama de clases con visibilidad y multiplicidad. Composición o generalización solo si su significado se justifica; si no se usan, se dice. Toda clase pertenece a un componente.
- **3.3:** tabla de responsabilidades (clase, componente, responsabilidad, colaboradores).
- **3.4:** comportamiento de **una** operación clave, máximo ~15 líneas, en pseudocódigo, con al menos un camino no feliz.

### 4.4 Sección 4 — Design Quality
- Cada respuesta (cohesión, acoplamiento, ocultamiento de información) cita evidencia del diseño.
- Al menos un **hallazgo real**: un problema encontrado al diseñar y lo que se cambió o cambiaría. **No se inventa historial de borradores previos.**

### 4.5 Sección 5 — Design Decisions & Decision Record
- Dos decisiones: **A** (arquitectónica) y **B** (diseño detallado), cada una con contexto, decisión, justificación y **trade-off real**.
- Al menos una debe tener ADR. Numeración: continúa la existente (siguiente: `ADR-0006`), archivo `docs/decisions/ADR-XXXX-titulo.md`. Si el ADR aún no está redactado, se dice.
- Una alternativa rechazada solo se menciona si realmente se consideró.

### 4.6 Sección 6 — Traceability
- Cada REQ de la sección 1 aparece. Cada componente y cada clase aparece al menos una vez; si algo no traza a nada, se cuestiona o se elimina.
- Caso de uso: el de la sección 4.3 del REQ; si el REQ no tiene (p. ej. proceso interno), `N/A: motivo`.

### 4.7 Sección 7 — AI-Assisted Design Reflection (opcional)
- Solo si el equipo registró una recomendación de IA real. Si no, `Pendiente` o `N/A: motivo`. Nunca se inventa.

### 4.8 Sección 8 — Consistency Checklist
- Cada casilla se marca **solo si se verificó**. Lo no verificable aún (p. ej. render en GitHub, ADR sin escribir) queda sin marcar con una nota.

---

## 5. Componentes ya definidos (reutilizar, no renombrar)

| Componente | Responsabilidad | Definido en |
|---|---|---|
| Presentación de Asignaciones | Muestra al estudiante sus comedores asignados o el mensaje que corresponde | Diseño REQ-10 |
| Consulta de Asignaciones | Decide qué debe ver el estudiante: sus asignaciones o la razón por la que no están disponibles | Diseño REQ-10 |
| Registro de Asignaciones | Persiste y entrega las asignaciones y el periodo de asignación vigente | Diseño REQ-10 |
| Cálculo de Asignaciones | Calcula y asigna un comedor por día (REQ-01); su diseño interno no se detalla en el diseño de REQ-10 | REQ-01 |

> Esta tabla se actualiza al agregar nuevos diseños, para mantener nombres y capacidades coherentes entre documentos.

---

## 6. Puntos a vigilar (brechas en los insumos actuales)

No se corrigen en silencio: se señalan al usuario.

- **Fuente del periodo de asignación vigente:** REQ-10 lo declara como dependencia y BR-02 lo condiciona al semestre, pero ningún REQ define quién lo mantiene ni dónde vive.
- **Identificación del estudiante:** ni REQ-01 ni REQ-10 definen cómo se identifica quien consulta; el RFC pide acceso restringido a los datos.
- **Reintento:** el riesgo de REQ-10 dice "se reintenta periódicamente"; el flujo de excepción corregido dice "pide reintentar". Falta definir quién dispara el reintento y con qué frecuencia.
- **Representación del horario de comedores:** Open Question de REQ-10. El diseño solo fija la representación **lógica** (día + comedor); la visual queda abierta.
- **Precondición de UC-01 (REQ-10):** su redacción parece contradecir la alternativa "los comedores no han sido asignados aún"; conviene revisarla.
- **BR-01 / REQ-01 / periodo vigente:** "frecuencia diaria" (BR-01) y "asignación fija durante el semestre" (REQ-01 corregido) admiten lecturas distintas; confirmar cómo se relacionan con el periodo "semana en curso" de REQ-10.
- **User Story de REQ-10:** el encabezado enlaza `US-01`, pero `US-01` está definida en REQ-01.
- **REQ-01 no tiene caso de uso** (proceso interno): en trazabilidad se escribe `N/A`.

---

## 7. Instrucciones de comportamiento para la IA

**Haz:**
- Diseña **solo lo que el usuario pidió** (los REQ indicados) y deja constancia del alcance.
- Basa cada componente, clase y decisión en un REQ, regla de negocio, restricción o riesgo real.
- Mantén la **congruencia** entre documentos: nombres, flujos y resultados del flujo del REQ (p. ej. los mismos mensajes y ramas del diagrama de flujo) y riesgos coherentes con lo ya documentado.
- Mantén la **simplicidad**: lo mínimo que cubre los REQ y sus riesgos, sin relleno.
- Señala aparte lo que haga falta añadir o definir, en vez de resolverlo por suposición.
- Entrega el contenido listo para copiarse al repositorio.

**No hagas:**
- No uses contenido del ejemplo.
- No inventes evidencia, componentes sin REQ, decisiones previas, borradores previos, métricas, fechas ni autores.
- No pongas tecnologías (PHP, C++, PostgreSQL, Tailwind) en nombres de componentes.
- No definas comportamientos que los REQ no definen (casos no resueltos).
- No cambies ni corrijas los REQ; solo señala la inconsistencia.
- No marques la sección 8 como cumplida si no se verificó.
- No modifiques la plantilla (secciones, orden, encabezados) sin avisar.

---

## 8. Cómo pedirle un diseño a la IA (entrada esperada)

1. **REQ(s)** a diseñar y sus archivos (en su versión vigente).
2. **Insumos adicionales** (Hipótesis Arquitectónica v0.1, Diseño Detallado v0.1, decisiones ya tomadas).
3. **Nombre del equipo, fecha y estado** deseado.
4. Decisiones del equipo sobre las brechas de la sección 6 que ya se resolvieron.

Si falta algo indispensable, la IA pregunta solo por eso o deja el campo como `Pendiente`.

---

## 9. Checklist de revisión antes de entregar

- [ ] Se respetó la plantilla y se eliminó el comentario inicial y las notas instructivas.
- [ ] Todo el contenido sale de las fuentes permitidas; nada del ejemplo.
- [ ] El alcance y los REQ de entrada están declarados; no hay dependencias entre REQ no declaradas.
- [ ] Los componentes se nombran por responsabilidad y coinciden entre diagrama y tabla.
- [ ] Las capacidades provistas y requeridas coinciden.
- [ ] Toda clase pertenece a un componente.
- [ ] La operación de 3.4 tiene un camino no feliz y respeta los flujos del REQ.
- [ ] Hay un hallazgo real y dos decisiones con trade-off real; al menos una con ADR (numeración correcta).
- [ ] La trazabilidad no tiene filas vacías ni componentes o clases huérfanos.
- [ ] Las brechas de la sección 6 que aplican están señaladas.
- [ ] Los diagramas renderizan en GitHub.
- [ ] Lo desconocido está marcado `Pendiente` o `N/A: motivo`, no inventado.

