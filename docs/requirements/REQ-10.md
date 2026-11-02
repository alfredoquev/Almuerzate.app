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
| **Use Case name** | Ver comedores asignados [UC-01] |
| **Actor** | Estudiante beneficiario |
| **Goal** | Hacer conocer al estudiante el comedor al que deberá ir en cada día de la semana |
| **Trigger** | El estudiante consulta sus comedores asignados |
| **Precondition** | Los comedores ya fueron asignados (aún no se ha brindado el subsidio alimentario, o el semestre ya acabó) |

**Main Flow**
1. El estudiante consulta sus comedores asignados para la semana.
2. El sistema busca los comedores asignados para el estudiante.
3. El sistema muestra el nombre de cada comedor asignado junto con su día correspondiente de la semana.

**Alternative Flow**
> Los comedores no han sido asignados aún → El sistema informa al estudiante que aún no se le ha brindado el subsidio alimentario.
> El semestre ya acabó → El sistema informa al estudiante que ningún semestre está siendo cursado actualmente y que las asignaciones continuarán hasta el siguiente semestre.

**Exception**
> No se pudo obtener los comedores asignados desde el backend → No se muestra el horario de comedores y se redirige a una página con el código del error que llevó a este resultado. (códigos de estado HTTP)

**Postcondition**
> El estudiante beneficiario puede ver sus comedores asignados cuando ya se hayan realizado las asignaciones y entren en efecto los subsidios de alimentación.

**Flow Diagram**

```mermaid
flowchart TD
    A([El estudiante consulta sus comedores asignados]) --> B["El estudiante consulta sus comedores asignados para la semana"]
    B --> C["El sistema busca los comedores asignados para el estudiante"]
    C --> D{"La información de las asignaciones está disponible"}
    D -- "La asignación existe y está disponible" --> G["Se muestra el nombre de cada comedor asignado junto con su día correspondiente de la semana"]
    G --> H(["El estudiante beneficiario puede ver sus comedores asignados"])
    D -- "No se han realizado asignaciones aún" --> E["Se informa al estudiante que aún no se ha brindado el subsidio alimentario"]
    E --> I(["No se muestran ningún comedor asignado"])
    D -- "No se pudo obtener los comedores asignados" --> F["No se muestra el horario de comedores"]
    F --> J(["Se redirige a una página con código de error"])
    D -- "No hay ningún semestre en curso" --> K["Se informa que ningún semestre está en curso y que las asignaciones siguen el siguiente semestre"]
    K --> I

    classDef mainflow fill:#1E2761,color:#ffffff,stroke:#1E2761;
    classDef alt fill:#F2A541,color:#1E2761,stroke:#F2A541;
    classDef exception fill:#B3261E,color:#ffffff,stroke:#B3261E;
    class B,C,G mainflow;
    class E,K alt;
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
