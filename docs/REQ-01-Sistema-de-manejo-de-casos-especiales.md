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

**Business Rule(s)**
> What rule(s) exist in the business/domain, independent of software, that this requirement supports or enforces? Remember: Business Rule ≠ Software Requirement — not every rule needs one.

**Constraint(s)**
> What limits how this requirement can be solved (regulation, existing technology, contract, interoperability, organizational policy)? A constraint reduces the available design space — it doesn't describe what must be satisfied, it describes what limits the solution.

**Assumption(s)**
> What are we currently treating as true, without full verification, to keep moving? (Assumption ≠ Fact.)

**Dependenc(ies)**
> What does this requirement rely on to be satisfied (another requirement, an external system/API, a data source, a third party, an organizational process)?

**Risk(s)**
> What uncertain event or condition could negatively affect this requirement or its delivery? For each risk, note a rough Likelihood and Impact (High / Medium / Low) and, if you have one, a brief mitigation note.

| Risk | Likelihood | Impact | Mitigation (optional) |
|---|---|---|---|
| | | | |

**Open Questions**
> Anything still unresolved — ambiguity, a question no stakeholder has answered yet, a missing piece of evidence. List it here instead of guessing.

# 3. Priority & Estimation

**Priority model used:** MoSCoW *(Modelo alternativo considerado: Kano)*

### Priority assigned: Must Have

> **Justificación:** Debido a que la saturación de los comedores y las fallas en la asignación de cupos o turnos impactan directamente la continuidad de la operación diaria y la atención al usuario, asignamos la prioridad **Must Have**. Aceptamos el trade-off de destinar capacidad de desarrollo en las primeras iteraciones al diseño de herramientas de monitoreo y gestión administrativa en lugar de avanzar en funcionalidades secundarias. Consideramos la prioridad **Should Have** y la descartamos porque carecer de un mecanismo de contingencia desde el lanzamiento expondría al sistema a colapsos operativos no gestionables.
>
> *(Nota sobre la alternativa: Se evaluó la matriz **Kano**, pero fue descartada debido a que mide la satisfacción y el deleite del usuario en lugar de la criticidad operativa y el alcance del MVP, para lo cual **MoSCoW** resulta más efectivo).*

### Estimate (confidence): Medium

> **Justificación:** Mantenemos un nivel de confianza **Medium** debido a que, mientras la captura del reporte por parte del usuario es un flujo estándar, la dimensión real del módulo administrativo dependerá del nivel de detalle en la consulta. Para facilitar una toma de decisiones rápida ante picos de saturación, las herramientas de consulta deberán incorporar filtros por comedor o sede, categoría del incidente (fallas de asignación vs. sobrecupo), nivel de severidad y rango temporal, sumado a vistas resumidas para la reasignación de aforos. La definición final de la interactividad de estos filtros determinará el esfuerzo exacto de desarrollo.

## 4. Representations — Requirement ≠ Representation

*(Class 9. Each representation reveals different information. Fill in all three below — for this capstone requirement, all three are required.)*

### 4.1 User Story

> Como **administrativo**,
> Quiero **ver un resumen de los casos inesperados en la asignación de comedores**,
> Para que **los casos no manejados automáticamente por el sistema se puedan resolver fácilmente**.

*Check yourself: is the "I want" describing the need, or already prescribing a solution?*

### 4.2 Acceptance Criteria

*(At least two scenarios: one normal path, one alternative or exception.)*

**Escenario 1 — Hay casos pendientes**
> **Dado que** hay casos pendientes por resolver
> **Cuando** el administrador abre el menú de "Manejo de casos atípicos"
> **Entonces,** el botón de acceso al menú de "Manejo de casos atípicos" mostrará el número de casos pendientes; luego al hacer clic en el, se muestra una lista de los casos, donde cada uno incluye nombre del estudiante, código del caso, fecha, información provista por el estudiante y botones de aceptar o rechazar. 

**Escenario 2 — No hay casos pendientes**
> **Dado que** no haya casos pendientes por resolver
> **Cuando** el administrador abre el menú de "Manejo de casos atípicos"
> **Entonces,** mostrar un mensaje que indique que no hay casos pendientes por resolver

**Escenario 3 — Se acepta un caso**
> **Dado que** se acepte un caso
> **Cuando** el administrador se encuentre en el menú de "Manejo de casos atípicos"
> **Entonces,** se muestra un menú que lista comedores sugeridos en orden de mejor a peor, al hacer clic en un comedor se habilita la opción de reasignar el comedor y resolver el caso

**Escenario 4 — Se rechaza un caso**
> **Dado que** se rechace un caso
> **Cuando** el administrador se encuentre en el menú de "Manejo de casos atípicos"
> **Entonces,** se muestra un menú con un campo de texto donde se deberá escribir la razón por la que se rechazó el caso y un botón para confirmar el rechazo del caso

**Escenario 5 — Los comedores sugeridos están disponibles**
> **Dado que** se pueda conseguir la lista de comedores sugeridos
> **Cuando** el administrador acepte un caso
> **Entonces,** se mostrará la lista de comedores sugeridos, organizados de mejor a peor

**Escenario 6 — Los comedores sugeridos NO están disponibles**
> **Dado que** no se pueda conseguir la lista de comedores sugeridos
> **Cuando** el administrador acepte un caso
> **Entonces,** se mostrará un mensaje de error informando que no se pudo obtener la lista de comedores

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
