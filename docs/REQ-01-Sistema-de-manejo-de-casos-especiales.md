<!--
HOW TO USE THIS TEMPLATE
------------------------
1. Copy this file and rename it to the requirement's ID, e.g. REQ-01.md.
2. Fill in every section. If a section genuinely does not apply to this
   requirement (not every requirement has a Business Rule or a Constraint),
   write "N/A — none identified" and say why in one line. Never invent
   content to fill a blank cell — that breaks the rule we've followed since
   Class 8: specification is not invention.
3. If something is unknown rather than inapplicable, use the Open
   Questions section instead of guessing.
4. See REQ-07_Example.md (Food Delivery System) for a fully filled-in
   reference before you start.
-->

# [REQ-ID] — [Short Requirement Title]

| | |
|---|---|
| **Status** | Draft / Validated / Open Question |
| **Team / Author(s)** | |
| **Date** | |
| **Linked User Story / AC** | |

---

## 1. Overview

**Requirement Statement**
> The system shall [capability/behavior] [under what condition, if applicable].

**Type:** Functional / Non-functional
*(Classified using the Perfect Technology Filter from Class 8: imagine a perfect computer — infinite speed, unlimited memory, zero failures, zero cost. Would this requirement still matter? Yes → likely functional. The limitation disappears with perfect technology → likely non-functional.)*

**Source / Evidence**
> Where did this come from? (stakeholder, interview, workshop, existing system, regulation — reference your Class 8 Discovery Sheet entry if you have one)

**Need**
> What is the underlying need, stated as a goal — not a solution? (e.g. "know when my order will arrive," not "build a GPS map")

**Value / Rationale**
> Why does this requirement matter? What becomes better for the user/business if it's satisfied? *(This is usually the same as the "so that" of your User Story below — write it once and reuse it, don't rewrite it from scratch.)*

---

## 2. Context — A Requirement Rarely Stands Alone

*(Class 10. Fill honestly — "N/A — none identified" is a valid, expected answer for several of these.)*

**Business Rule(s) / Reglas de Dominio**
>R-1: El programa de bienestar universitario asigna a cada estudiante beneficiario un comedor donde recibe almuerzo, total o parcialmente gratis. Solo los estudiantes beneficiarios pueden solicitar cambio.
>R-2: No existe un catálogo cerrado de “circunstancias excepcionales”. El administrador evalúa cada caso según los argumentos del solicitante.
>R-3: Cada comedor tiene un aforo operativo del 80 % de su capacidad máxima real, para mantenerlo subsaturado y balancear la carga entre comedores.
>R-4: Un cambio aprobado es permanente durante el semestre, y puede modificarse con una nueva solicitud.
>R-5: Toda solicitud debe incluir una justificación escrita. La evidencia adjunta es opcional.
>R-6: Un estudiante puede tener una sola solicitud pendiente a la vez. Si le rechazan una, puede volver a solicitar aportando argumentos nuevos.
>R-7 (propuesta): El administrador responde en un máximo de 3 días hábiles. Todo rechazo debe incluir el motivo.
>R-8 (propuesta): El comedor destino no debería superar el 85 % de aforo de ser posible. Si el administrador decide excederlo, debe dejar justificación escrita, y nunca puede superar la capacidad máxima real.
>R-9: Cada decisión queda registrada (quién, cuándo, resultado y motivo).
>R-10: El estudiante es notificado de la decisión.

**Constraint(s) / Limitaciones**
Roles: estudiante y administrador son un atributo de la cuenta de usuario, definido por el rol que la persona tiene en el sistema de la universidad. El demo debe simular esa fuente de roles.
Arquitectura: la solicitud combina el backend en C++ (árbol de decisión, ADR-0004) con el criterio del administrador. El árbol sugiere alternativas y el administrador decide, priorizando la conveniencia del estudiante. El formato JSON y el ADR que reemplaza al ADR-0002 siguen POR CONFIRMAR.
Datos: solo datos de prueba. Aun así, el adjunto de evidencia permanece en el demo (confirmado). Si el sistema se usara con datos reales, debe aplicar la Ley 1581 de 2012 (habeas data), porque la evidencia puede contener datos de salud.
Despliegue: Vercel, con qué parte va allí POR CONFIRMAR. Esto limita dónde se pueden guardar los archivos adjuntos.
Seguridad: el ADR-0002 la deja fuera de alcance, pero el panel de administración debe al menos restringirse por rol.

**Assumption(s)**
>La asignación base (R1 / Entrega 1) ya existe cuando se hacen solicitudes.
>Siempre existe al menos un comedor alternativo con cupo disponible.
>Los administradores revisan las solicitudes con regularidad.
>El estudiante accede a la PWA Progressive Web App con conexión.
>Para el demo, las cuentas y roles se simulan con datos de prueba.
>Un cambio aprobado respeta almenos el 85 % del aforo, salvo excepción justificada (R-8).

**Dependenc(ies) / Dependencias**
>R1 / Entrega 1: sin asignación previa no hay nada que cambiar.
>Backend C++ (árbol de decisión): sugiere comedores alternativos al administrador.
>PostgreSQL: guarda solicitudes, decisiones, historial y asignaciones.
>Cuentas y roles: vienen del sistema de usuarios de la universidad (simulado en el demo).
>Estimado de ocupación (POR CONFIRMAR): sin él, el administrador no ve la ocupación proyectada al decidir.
>Notificación al estudiante: mecanismo mínimo propuesto, in-app.
>Reasignación periódica (Entrega 2): debe respetar los cambios manuales aprobados. Se deriva de BR-4, porque un cambio “permanente para el semestre” no puede ser pisado por el recálculo.

**Risk(s) / Riesgos**
| Risk |	Likelihood |	Impact |	Mitigation |
| La reasignación automática sobrescribe un cambio manual |	Medium |	High |	Marcar la asignación como “manual” con precedencia sobre el recálculo |
|Se acumulan solicitudes y el administrador no da abasto |	Medium |	High |	Plazo de respuesta (R-7) y vista de pendientes ordenada por antigüedad |
|Se aprueban cambios que dejan un comedor sobre el 85 % |	Medium |	Medium |	Mostrar ocupación proyectada al decidir y exigir justificación si se excede |
|Uso indebido o criterio arbitrario por falta de reglas de excepcionalidad |	Medium |	Medium |	Justificación obligatoria, registro de decisiones y una solicitud pendiente por estudiante |
|El estimado de ocupación no está definido y el administrador decide a ciegas |	High |	Medium |	Definirlo antes de la entrega del panel, o usar un conteo simple de asignados por comedor |
|El almacenamiento de archivos adjuntos no encaja con el despliegue en Vercel |	Medium |	Medium |	Decidir dónde se guardan antes de implementar; limitar tipo y tamaño |
| No hay un stakeholder real que valide las reglas |	High |	Medium |	Tratar las reglas como supuestos y validarlas con el profesor o un stakeholder simulado |

**Open Questions**
1.Alcance por día: R1 asigna comedor por día, pero el cambio es “permanente para el semestre”. ¿El cambio reemplaza toda la asignación semanal o puede pedirse solo para días concretos?
2.Entrega: el RFC ubica el panel de administración y la reasignación de emergencia en la entrega final, pero REQ-1 es Must Have. ¿Qué parte de REQ-1 entra en cada entrega?
3.Estimado de ocupación: ¿cómo se calcula y es dependencia dura de REQ-1?
4.Notificación: ¿es solo in-app o también por correo?
5.Reglas propuestas: BR-5 a BR-10 (plazo de 3 días, una solicitud pendiente, justificar rechazos) no están validadas.
6.Almacenamiento: ¿dónde se guardan los archivos de evidencia, y con qué límites de tipo y tamaño?
7.Interpretación del 80 %: asumí que es 80 % de la capacidad máxima real. ¿Es correcto?
8.Validación: el estado dice Validated, pero no hay historias de usuario ni stakeholder confirmado. ¿Quién o qué lo validó?
9.Backend: ¿las solicitudes pasan por la API en C++ o solo la sugerencia del árbol? Falta el ADR nuevo.

---

## 3. Priority & Estimation

**Priority model used:** MoSCoW / Kano / RICE / WSJF / Value-Effort *(pick the one that fits the decision you're actually making — see Class 10's comparison if you need to decide which)*

**Priority assigned:**
> Because [the situation/signal from your project], we assigned [priority value], accepting the trade-off of [what you're giving up by not prioritizing it higher/lower]. We considered [alternative priority] and didn't use it because [reason].

**Estimate (confidence):** High / Medium / Low
> How sure are we about the size and shape of this work? *(This is a confidence tag, not a technique-based number — named estimation techniques like story points are Ingeniería de Software II content, not this course.)*

---

## 4. Representations — Requirement ≠ Representation

*(Class 9. Each representation reveals different information. Fill in all three below — for this capstone requirement, all three are required.)*

### 4.1 User Story

> As a **[role]**,
> I want **[capability]**,
> so that **[value]**.

*Check yourself: is the "I want" describing the need, or already prescribing a solution?*

### 4.2 Acceptance Criteria

*(At least two scenarios: one normal path, one alternative or exception.)*

**Scenario 1 — [short name]**
> **Given** [context]
> **When** [event/stimulus]
> **Then** [observable outcome]

**Scenario 2 — [short name, alternative or exception]**
> **Given** [context]
> **When** [event/stimulus]
> **Then** [observable outcome]

### 4.3 Use Case

| Field | |
|---|---|
| **Use Case name** | |
| **Actor** | |
| **Goal** | |
| **Trigger** | |
| **Precondition** | |

**Main Flow**
1.
2.
3.
4.

**Alternative Flow**
> [Condition] → [what happens instead]

**Exception**
> [Condition] → [what happens instead]

**Postcondition**
>

**Flow Diagram**
*(Replace the labels below with your own steps. Keep Main Flow, Alternative, and Exception visually distinct — delete whichever branch doesn't apply to your use case. This renders automatically on GitHub.)*

```mermaid
flowchart TD
    A([Trigger: what starts this use case]) --> B["1. Main flow step"]
    B --> C["2. Main flow step"]
    C --> D{"Decision point, if any"}
    D -- "Normal path" --> G["3. Main flow step"]
    G --> H(["Postcondition / goal reached"])
    D -- "Alternative condition" --> E["ALTERNATIVE: what happens instead"]
    E --> H
    D -- "Exception condition" --> F["EXCEPTION: what happens instead"]
    F --> H

    classDef mainflow fill:#1E2761,color:#ffffff,stroke:#1E2761;
    classDef alt fill:#F2A541,color:#1E2761,stroke:#F2A541;
    classDef exception fill:#B3261E,color:#ffffff,stroke:#B3261E;
    class B,C,G mainflow;
    class E alt;
    class F exception;
```

---

## 5. Traceability & Impact

*(Class 10. Conceptual, not a formal matrix.)*

**Backward — why does this requirement exist?**
> Evidence → Need → Requirement. Point to the specific evidence/need entries that justify this requirement (from your Discovery Sheet).

**Forward — what will this affect?**
> Requirement → future Design → Implementation → Tests. *(It's fine if Design hasn't happened yet — note what you expect this to touch once it does, and update this section once Class 11 work begins.)*

**Impact Analysis — if this requirement changes, what else might need to change?**
- [ ] Business Rules
- [ ] Constraints
- [ ] Dependencies
- [ ] Risks
- [ ] Acceptance Criteria
- [ ] Estimate
- [ ] Priority
- [ ] Future Design
- [ ] Future Tests

> Briefly note which of the above are actually likely to be affected, and why.

---

## 6. Validation — Quality Gate

*(Class 9 + Class 10. Self-audit before you commit this file. Check honestly — a "no" here means the requirement isn't ready yet, not that you should force a checkmark.)*

- [ ] **Valid?** Does it reflect a real, evidenced need — not an invented one?
- [ ] **Clear / Unambiguous?** Is there only one reasonable interpretation?
- [ ] **Atomic?** Is this one independently testable expectation, not several bundled together?
- [ ] **Necessary?** Does removing it actually break something real?
- [ ] **Feasible?** Can this realistically be built with what the team has?
- [ ] **Verifiable?** Can you demonstrate, concretely, whether it's satisfied?
- [ ] **Consistent?** Does it conflict with any other requirement in your set?
- [ ] **Complete enough?** Are there important functions or constraints still missing?
- [ ] **Traceable?** Can every part of this document be traced back to real evidence — not invented to fill a section?

> If any box is unchecked, say what's missing and whether it becomes an Open Question or sends you back to Class 8 (re-elicit) or Class 9 (re-specify).
