<!--
WORKED EXAMPLE — not a blank to fill in and not a project to reuse.
Food Delivery System, the running example from Classes 8 to 11. It covers three requirements
only (REQ-07, REQ-12, REQ-13) so it stays readable. Your deliverable must cover YOUR whole
system and YOUR own project. Use this for level of detail and for how the sections connect.
-->

# Food Delivery System — Architectural & Detailed Design v1.0

| | |
|---|---|
| **Team** | Equipo Almuerzate |
| **Members** | Brayan Santiago Amado Mejia, Diego Alejandro Gaviria Sabogal, Julian Camilo Londoño Zarate, Juan Felipe Pineda Gonzales, Jose Alfredo Quevedo Suarez.   |
| **Status** | Ready for review |
| **Date** | 2026-10-19 |
| **Inputs** | `docs/REQ-01-Sistema-de-manejo-de-casos-especiales.md` |

---

## 1. System Overview & Design Drivers

**System purpose**
> Let customers place food orders and follow them until delivery, reducing the uncertainty of waiting and avoiding wasted orders.

| Requirement / Use Case ID | Why it shapes the structure | Quality attribute, constraint or business rule involved |
|---|---|---|
| REQ-07 / Track Order | Needs data from a provider we do not control and must keep working when that data is missing | Constraint: integrate with the existing tracking provider. Quality: availability of the customer experience |
| REQ-12 / Cancel Order | Order state changes must be controlled in one place | Business Rule BR-01: an order can be cancelled only before preparation begins |
| REQ-13 / Cancel Order (rejection) | Same rule, opposite outcome, must stay consistent with REQ-12 | BR-01 |

---

## 2. Architectural Design
 
### 2.1 System Context & Boundary
 
| Outside the system | Type | How the system interacts with it |
|---|---|---|
| Estudiante beneficiario | Actor | Reporta un caso atípico, consulta el estado de sus solicitudes y ve la decisión tomada |
| Administrador del servicio de alimentación | Actor | Revisa los casos pendientes, acepta o rechaza cada uno y elige el comedor de reasignación |
| Sistema de usuarios de la universidad | External system (simulado con datos de prueba en el demo) | Le pedimos confirmar quién es el usuario y qué rol tiene. No controlamos su disponibilidad ni su formato |
 
*(La asignación base de comedores (R1 / Entrega 1) vive dentro del sistema, pero su cálculo con el árbol de decisión no se detalla aquí: REQ-01 solo necesita leerla y modificarla.)*
 
### 2.2 Components, Responsibilities & Capabilities
 
```mermaid
flowchart TB
    Estudiante([Estudiante beneficiario])
    Usuarios[("Sistema de usuarios de la universidad")]
    Admin([Administrador])

    subgraph SIS["Almuérzate — Manejo de casos atípicos"]
        IES["Interfaz de Solicitudes del Estudiante"]
        PCA["Panel de Casos Atípicos"]

        subgraph MS["Manejo de solicitudes"]
            GC["Gestión de Casos"]
            NE["Notificaciones al Estudiante"]
            GA["Gestión de Asignaciones"]
        end

        SC["Sugerencia de Comedores"]
        REG["Registros"]
    end

    Estudiante --> IES
    Admin --> PCA
    Usuarios -.->|"verificar usuario y rol"| IES
    Usuarios -.->|"verificar usuario y rol"| PCA

    IES -->|"enviar / seguir solicitudes"| GC
    IES -->|"consultar mis decisiones"| NE
    PCA -->|"listar / aceptar / rechazar casos"| GC
    PCA -->|"obtener comedores sugeridos"| SC
    SC -->|"obtener carga asignada"| GA
    GC -->|"reasignar / obtener asignación"| GA
    GC -->|"notificar decisión"| NE
    GC -->|"guardar / buscar"| REG
    GA -->|"guardar / buscar"| REG
    NE -->|"guardar / buscar"| REG

    classDef comp fill:#f4f4f4,stroke:#555555,color:#222222;
    classDef actor fill:#ffffff,stroke:#555555,color:#222222;
    classDef ext fill:#ffffff,stroke:#555555,stroke-dasharray:4 3,color:#222222;
    class IES,PCA,GC,SC,NE,GA,REG comp;
    class Estudiante,Admin actor;
    class Usuarios ext;
    style SIS fill:#ffffff,stroke:#555555,color:#222222
    style MS fill:#fafafa,stroke:#888888,stroke-dasharray:4 3,color:#222222
```
 
| Component | Main responsibility | Requirements / use cases supported | Provided capabilities | Required capabilities | Main dependencies |
|---|---|---|---|---|---|
| Interfaz de Solicitudes del Estudiante | Muestra al estudiante el formulario de solicitud, el estado de sus casos y la decisión, y reenvía sus acciones | REQ-01: US-05, US-06, UC-4, UC-5, AC-09, AC-10, AC-11 | Reportar un caso, consultar mis solicitudes y su decisión | Verificar usuario y rol (externo), enviar solicitud, consultar mis solicitudes, obtener opciones de solicitud, consultar mis decisiones | Gestión de Casos, Notificaciones al Estudiante, Sistema de usuarios |
| Panel de Casos Atípicos | Muestra al administrador los casos pendientes y la lista de comedores sugeridos, y reenvía su decisión | REQ-01: US-01 a US-04, UC-1, UC-2, UC-3, AC-01 a AC-07 | Mostrar y resolver casos pendientes | Verificar usuario y rol (externo), listar casos pendientes, aceptar caso, rechazar caso, obtener comedores sugeridos | Gestión de Casos, Sugerencia de Comedores, Sistema de usuarios |
| Gestión de Casos | Aplica las reglas de las solicitudes (justificación obligatoria, una pendiente por estudiante, motivo en todo rechazo, decisión registrada) y controla el estado de cada caso | REQ-01: US-01, US-02, US-03, US-05, UC-1 a UC-5, AC-01, AC-02, AC-04, AC-05, AC-09, AC-10; R-5, R-6, R-7, R-9 | Enviar solicitud, consultar mis solicitudes, obtener opciones de solicitud, listar casos pendientes, aceptar caso, rechazar caso | Reasignar estudiante, obtener asignación del estudiante, notificar decisión, buscar / guardar casos y decisiones | Gestión de Asignaciones, Notificaciones al Estudiante, Registros |
| Sugerencia de Comedores | Ordena los comedores alternativos de mejor a peor según la ocupación proyectada, o responde explícitamente que no pudo hacerlo (árbol de decisión en el backend, ADR-0004) | REQ-01: US-04, UC-2, AC-03, AC-06, AC-07; R-3, R-8 | Obtener comedores sugeridos | Obtener carga asignada por comedor | Gestión de Asignaciones |
| Gestión de Asignaciones | Entrega la asignación vigente de un estudiante, aplica la reasignación y la marca como manual para que ningún recálculo la pise | REQ-01: US-02, UC-2, UC-5, AC-04; R-4 | Obtener asignación del estudiante, reasignar estudiante, obtener carga asignada por comedor | Buscar / guardar asignaciones, obtener capacidades | Registros |
| Notificaciones al Estudiante | Deja disponible dentro de la PWA la decisión de cada caso (aceptado con nuevo comedor, o rechazado con motivo) | REQ-01: US-06, UC-2, UC-3, UC-4, AC-08; R-10, L9 | Notificar decisión, consultar mis decisiones | Buscar / guardar notificaciones | Registros |
| Registros | Guarda y entrega casos, decisiones, asignaciones, capacidades de los comedores y notificaciones | REQ-01: todos los anteriores; R-9 | Buscar / guardar casos y decisiones, buscar / guardar asignaciones, obtener capacidades, buscar / guardar notificaciones | none | none |
 
**Pendientes que afectan este diseño** (vienen de las *Open Questions* del REQ-01):
- La definición del estimado de ocupación (pregunta 3) no está cerrada. Mientras tanto, `Sugerencia de Comedores` usa lo que `Gestión de Asignaciones` le entrega (estudiantes asignados por comedor frente a su capacidad).
- No se decidió dónde se guardan los adjuntos de evidencia (pregunta 5), así que no hay un componente para ellos.
- Falta el ADR nuevo que defina qué parte pasa por la API en C++ (pregunta 8).

---

## 3. Detailed Design

### 3.1 Components selected

| Component | Why it was selected |
|---|---|
| Tracking Integration | Highest risk: depends on an external provider and must handle three outcomes (valid data, stale data, no response) |
| Order Management | Carries business rule BR-01. A mistake here breaks the cancellation behavior directly |

### 3.2 Class Diagram

```mermaid
classDiagram
    class EstimateResolver {
        +resolveEstimate(orderId : OrderId) DisplayEstimate
    }
    class TrackingIntegration {
        +getEstimatedArrival(orderId : OrderId) EtaResult
    }
    class RequestHandler {
        +fetch(orderId : OrderId) RawTrackingResponse
    }
    class ResponseParser {
        +parse(raw : RawTrackingResponse) TrackingData
    }
    class FallbackHandler {
        +unavailable(reason : String) EtaResult
    }
    class EtaResult {
        -available : Boolean
        -eta : Timestamp
        -reason : String
        +isAvailable() Boolean
    }
    class OrderManagement {
        +cancelOrder(orderId : OrderId) CancellationResult
    }
    class Order {
        -orderId : OrderId
        -status : OrderStatus
        +isCancellable() Boolean
        +cancel()
    }
    class OrderItem {
        -menuItemId : Integer
        -quantity : Integer
    }
    class OrderRepository {
        <<interface>>
        +findById(orderId : OrderId) Order
        +save(order : Order)
    }

    EstimateResolver --> TrackingIntegration : asks for estimate
    TrackingIntegration --> RequestHandler : uses
    TrackingIntegration --> ResponseParser : uses
    TrackingIntegration --> FallbackHandler : uses
    TrackingIntegration ..> EtaResult : returns
    OrderManagement --> Order : applies rule on
    OrderManagement ..> OrderRepository : depends on
    Order "1" *-- "1..*" OrderItem : contains
```

*(`Order` and `OrderItem` use composition: an item has no meaning outside its order. No generalization is used because nothing in this design is a specialized type of something else.)*

### 3.3 Class Responsibilities

| Class | Component | Responsibility | Key collaborators |
|---|---|---|---|
| EstimateResolver | Estimate Resolver | Turns an estimate result into what the customer should see | TrackingIntegration |
| TrackingIntegration | Tracking Integration | Coordinates the steps needed to produce an estimate or Unavailable | RequestHandler, ResponseParser, FallbackHandler |
| RequestHandler | Tracking Integration | Sends the request to the provider and returns the raw response | (provider) |
| ResponseParser | Tracking Integration | Validates the provider's response and converts it to data our system understands | none |
| FallbackHandler | Tracking Integration | Builds a consistent "unavailable" result with a reason | none |
| EtaResult | Tracking Integration | Carries either an ETA or an unavailable reason | none |
| OrderManagement | Order Management | Applies BR-01 and coordinates cancellation | Order, OrderRepository |
| Order | Order Management | Knows its status and whether it can still be cancelled | OrderItem |
| OrderItem | Order Management | Represents one menu item and quantity inside an order | none |
| OrderRepository | Order Records | Stores and retrieves orders (contract only, storage details hidden) | Order |

### 3.4 Behavior of one key operation

```text
getEstimatedArrival(orderId):
    raw = RequestHandler.fetch(orderId)
    IF raw is missing:
        RETURN FallbackHandler.unavailable("provider did not respond")
    data = ResponseParser.parse(raw)
    IF data is incomplete OR older than the freshness limit:
        RETURN FallbackHandler.unavailable("tracking data not usable")
    RETURN EtaResult(available, computeEta(data))
```

---

## 4. Design Quality — Cohesion, Coupling, Information Hiding

| Question | Your answer, with evidence from your design |
|---|---|
| **Cohesion** | `TrackingIntegration` only coordinates. Fetching, parsing and building the unavailable result each live in their own class. `Order` holds only status and eligibility. |
| **Coupling** | The strongest dependency is on the provider's response format, and it is contained in `ResponseParser`. `EstimateResolver` depends only on `EtaResult`. A provider change reaches one class. |
| **Information hiding** | The raw provider response, the freshness check and the storage details behind `OrderRepository` stay internal. Other components see only `EtaResult` and the repository contract. |

**Real findings and what we changed**
> The first draft had `EstimateResolver` reading the provider's timestamp format to decide whether data was stale (coupling). We moved parsing and freshness checks into `ResponseParser`. The first draft also had `Order` formatting the cancellation message for the customer (cohesion). We moved the wording to Order Status Display.

---

## 5. Design Decisions & Decision Record

### Decision A — Architectural

| | |
|---|---|
| **Context** | REQ-07 depends on an external provider we do not control (Constraint). Its availability and format can change. |
| **Decision** | All communication with the tracking provider goes through Tracking Integration. No other component calls the provider or sees its format. |
| **Rationale** | A provider failure or format change stays contained, and the customer can still see the order status when no estimate exists. |
| **Trade-off accepted** | One extra hop between the customer's screen and the provider, so slightly more latency. We rejected letting the display call the provider directly because every provider change would then reach the user interface. |

### Decision B — Detailed design

| | |
|---|---|
| **Context** | The provider can answer with usable, stale, incomplete data, or not answer at all. Those outcomes were being handled in different places. |
| **Decision** | `TrackingIntegration` always returns an `EtaResult`, and "unavailable" is a normal result with a reason, never an exception that propagates. |
| **Rationale** | Callers handle one result type. The unavailable path (REQ-07 alternative and exception flows) is designed instead of accidental. |
| **Trade-off accepted** | Callers must check `isAvailable()` explicitly, and a forgotten check shows an empty estimate instead of failing loudly. |

### ADR

**ADR file:** `docs/decisions/ADR-001-isolate-tracking-provider.md` — records Decision A.

---

## 6. Traceability

| Requirement ID | Use Case | Component(s) | Class(es) |
|---|---|---|---|
| REQ-07 | Track Order | Order Status Display, Estimate Resolver, Tracking Integration | EstimateResolver, TrackingIntegration, RequestHandler, ResponseParser, FallbackHandler, EtaResult |
| REQ-12 | Cancel Order | Order Status Display, Order Management, Order Records | OrderManagement, Order, OrderItem, OrderRepository |
| REQ-13 | Cancel Order | Order Status Display, Order Management | OrderManagement, Order |

---

## 7. Optional — AI-Assisted Design Reflection

> An AI assistant suggested adding a separate Notification component to tell customers when an estimate changes. We rejected it: no requirement in our set supports it, and it would add a component without a responsibility we can trace.

---

## 8. Consistency Checklist

- [x] Every component in the diagram appears in the responsibilities table, and the other way around.
- [x] Every class belongs to an identifiable component.
- [x] Components are named by responsibility, not technology.
- [x] Nothing inside the system boundary is something we do not control.
- [x] Provided and required capabilities match across components.
- [x] Both decisions name a real cost or limitation, and at least one has an ADR.
- [x] The traceability table has no empty rows and no orphan components or classes.
- [x] Diagrams render correctly on GitHub.
