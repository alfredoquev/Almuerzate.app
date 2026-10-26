<!--
HOW TO USE THIS TEMPLATE
------------------------
1. Copy this file to docs/design/02_Interaction_and_Data_Design.md in your team repository.
2. This is v1.0 of work you started in Class 13 ("Interaction & Contracts Design v0.1").
   It builds on Deliverable 1 and must stay consistent with it. If making behavior and data
   concrete reveals a problem in Deliverable 1, update Deliverable 1 and say so in Section 4.
3. Use YOUR project. Generic examples are reference material only.
4. Sequence diagrams are derived from the Use Cases in your requirements. Do not invent
   business behavior. If something is not specified, mark it as an assumption or an Open Question.
5. If nothing in your system needs to persist, say so in Section 2.1 with your reasons. Do not
   invent a database.
6. Diagrams: Mermaid is preferred (renders on GitHub). PlantUML or another tool is accepted if
   the exported image is committed and embedded in this file.
   Optionally, also add a JPG of each diagram in docs/design/diagrams/ (the Markdown prevails if they differ).
7. Read Design_Deliverable_2_Example.md for the expected level of detail. Do not copy it.
-->

# Almuérzate — Interaction & Data Design v1.0

| | |
|---|---|
| **Team** | Equipo Almuérzate |
| **Members** | Brayan Santiago Amado Mejia, Diego Alejandro Gaviria Sabogal, Julian Camilo Londoño Zarate, Juan Felipe Pineda Gonzalez, Jose Alfredo Quevedo Suarez. |
| **Status** | Draft |
| **Date** | 2026-10-24 |
| **Inputs** | `docs/design/01_Architectural_and_Detailed_Design.md`, Casos de `docs/requirements/REQ-01-Sistema-de-manejo-de-casos-especiales.md`, Interaction & Contracts Design v0.1 (Class 13) |

---

## 1. Behavior — Selected Use Cases

Se seleccionan los casos UC-2 y UC-5. UC-2 representa nuestro caso base (capstone) de REQ-01 debido a que aquí es donde el sistema de casos especiales modifica la asignacion base de R1 ligado a la regla de dominio R-4, recorre 6 de 7 componentes con cambios de escrituras y sustenta 3 de las 4 filas de design drivers (Architectural & Detailed Design v1.0) y es Must Have en US-02, US-04.
Por otro lado UC-5 lo complementa, origina el caso, usa 'Interfaz de Solicitudes del Estudiante' (unico componente que no concierne UC-2) y además cubre el fallo de envio con guardado local AC-10.

### 1.1 UC-2 (Se acepta un caso pendiente) — from requirement REQ-01

```mermaid
sequenceDiagram
    autonumber
    actor Admin as Administrador
    participant PCA as Panel de Casos Atípicos
    participant SC as Sugerencia de Comedores
    participant GC as Gestión de Casos
    participant GA as Gestión de Asignaciones
    participant NE as Notificaciones al Estudiante
    participant REG as Registros

    Admin->>PCA: clic en Aceptar(idCaso)
    PCA->>SC: obtener comedores sugeridos(dia)
    SC->>GA: obtener carga asignada por comedor(dia)
    GA->>REG: buscar asignaciones(dia)
    alt Registros devuelve los datos de ocupación
        REG-->>GA: asignaciones del día
        GA->>REG: obtener capacidades()
        REG-->>GA: capacidad por comedor
        GA-->>SC: estudiantes asignados y capacidad por comedor
        SC-->>PCA: comedores ordenados de mejor a peor
        PCA-->>Admin: muestra la lista sugerida, el caso sigue pendiente
        Admin->>PCA: elige un comedor y confirma(idCaso, idComedor)
        PCA->>GC: aceptar caso(idCaso, idComedor)
        GC->>REG: buscar caso(idCaso)
        REG-->>GC: caso con estado, estudiante y día
        alt el caso sigue pendiente
            GC->>GA: reasignar estudiante(idEstudiante, dia, idComedor)
            GA->>REG: guardar asignación con origen manual
            REG-->>GA: guardada
            GA-->>GC: reasignación aplicada
            GC->>REG: guardar caso aceptado y decisión
            REG-->>GC: guardado
            GC->>NE: notificar decisión(idCaso, aceptado, idComedor)
            NE->>REG: guardar notificación
            REG-->>NE: guardada
            NE-->>GC: notificación disponible
            GC-->>PCA: caso reasignado
            PCA-->>Admin: confirma la reasignación
        else el caso ya fue resuelto
            GC-->>PCA: error 409, caso ya resuelto
            PCA-->>Admin: muestra el error
        end
    else Registros no responde o faltan datos
        REG-->>GA: error
        GA-->>SC: error
        SC-->>PCA: lista no disponible con código y mensaje
        PCA-->>Admin: muestra código y mensaje, el caso sigue pendiente
    end
```

| Use Case step or flow | Message(s) in the diagram | Information exchanged | Operation defined in (Deliverable 1) |
|---|---|---|---|
| Main flow, paso 1 | 1 | idCaso, tomado de la lista de UC-1 | Panel de Casos Atípicos: «Mostrar y resolver casos pendientes» (2.2) |
| Main flow, paso 2 | 2 a 9 | Va el día del caso. Vuelven estudiantes asignados y capacidad por comedor, y la lista ordenada de mejor a peor | Sugerencia de Comedores: «obtener comedores sugeridos». Gestión de Asignaciones: «obtener carga asignada por comedor». Registros: «buscar / guardar asignaciones» y «obtener capacidades» (2.2). Sin clase en 3.2 |
| Main flow, paso 3 | 10 | Lista sugerida. No se escribe nada: el caso sigue pendiente | Panel de Casos Atípicos (2.2) |
| Main flow, pasos 4 y 5 | 11 y 12 | idCaso e idComedor | «aceptar caso» (2.2) = ManejoSolicitudes.responderSolicitud(idSolicitud, decision, mensaje, idAdministrador) (3.2). Falta el comedor destino |
| Main flow, paso 6 (validar y reasignar) | 13 a 18 | Caso con estado, estudiante y día. Luego estudiante, día y comedor, y la asignación guardada con origen manual (R-4) | RepositorioSolicitudes.buscarPorId (3.2). «reasignar estudiante» (2.2), sin clase |
| Main flow, paso 6 (registrar y notificar) | 19 a 24 | Caso aceptado con la decisión (quién, cuándo, resultado, comedor; R-9). Luego idCaso, resultado y comedor | Solicitud.aprobar (se ejecuta dentro de Gestión de Casos antes del mensaje 19) y RepositorioSolicitudes.guardar (3.2). «notificar decisión» (2.2), sin clase |
| Main flow, paso 6 (respuesta) | 25 y 26 | Caso reasignado y confirmación al administrador | Resultado (3.2) |
| Alternative flow (propuesta, no está en UC-2) | 25 y 26 | Caso reasignado y confirmación al administrador | Solicitud controla las transiciones válidas (3.3). Falta YA_RESUELTA en ErrorSolicitud |
| Exception (AC-07) | 29 a 32 | El error sube de Registros al administrador con código y mensaje | Sugerencia de Comedores «responde explícitamente que no pudo» (2.2). Resultado con error (Decision A) |


### 1.2 UC-5 (Se solicita reasignar un comedor) — from requirement REQ-01

```mermaid
sequenceDiagram
    autonumber
    actor Est as Estudiante beneficiario
    participant IES as Interfaz de Solicitudes del Estudiante
    participant GC as Gestión de Casos
    participant GA as Gestión de Asignaciones
    participant REG as Registros

    Est->>IES: clic en Solicitar reasignación de comedor
    IES->>GC: obtener opciones de solicitud()
    GC->>GA: obtener asignación del estudiante()
    GA->>REG: buscar asignaciones del estudiante
    REG-->>GA: asignaciones por día
    GA-->>GC: día y comedor actual de cada asignación
    GC-->>IES: opciones de solicitud
    IES-->>Est: muestra el formulario
    Est->>IES: elige día y comedor, escribe la justificación y envía
    IES->>GC: enviar solicitud(dia, idComedorPreferido, justificacion, idEnvio)
    alt el envío llega y Gestión de Casos responde
        GC->>REG: buscar solicitud pendiente del estudiante
        REG-->>GC: solicitudes del estudiante
        alt cumple R-5 y R-6
            GC->>REG: guardar solicitud en estado pendiente
            REG-->>GC: guardada con código de caso
            GC-->>IES: solicitud registrada con código de caso
            IES-->>Est: mensaje de envío exitoso
        else incumple R-5 o R-6
            GC-->>IES: rechazo 4XX con el motivo
            IES-->>Est: muestra el motivo
        end
    else sin respuesta o error 5XX
        IES->>IES: guardar la solicitud localmente
        IES-->>Est: código y mensaje de error
    end
```
| Use Case step or flow | Message(s) in the diagram | Information exchanged | Operation defined in (Deliverable 1) |
|---|---|---|---|
| Main flow, paso 1 | 1 | Clic del estudiante con sesión abierta. | Interfaz de Solicitudes del Estudiante: «Reportar un caso» (2.2) |
| Main flow, paso 2 | 2 a 8 | Se piden las opciones. Vuelven día y comedor actual de cada asignación, y se muestra el formulario. | «obtener opciones de solicitud», «obtener asignación del estudiante», «buscar / guardar asignaciones» (2.2). Sin clase en 3.2 |
| Main flow, paso 3 a 5 (captura) | 9 | Día, comedor preferido y justificación (R-5). | «Reportar un caso» (2.2) |
| Main flow, paso 5 (envio) | 10 | dia, idComedorPreferido, justificacion, idEnvio | «enviar solicitud» (2.2) = ManejoSolicitudes.realizarSolicitud(motivo, idEstudiante) (3.2). Faltan día, comedor e idEnvio |
| Main flow, poscondición | 11 a 16 | Solicitudes del estudiante. Solicitud en estado pendiente. Código del caso y mensaje de éxito. | RepositorioSolicitudes.buscarPorEstudiante y guardar (3.2), EstadoSolicitud |
| Alternative flow (propuesta, no está en UC-5) | 17 y 18 | 4XX con el motivo (R-5 o R-6) | Resultado con ErrorSolicitud (3.2). Faltan JUSTIFICACION_VACIA y YA_HAY_PENDIENTE |
| Exception (AC-10) | 19 y 20 | Solicitud completa con idEnvio, guardada en el cliente. Código y mensaje de error | No existe como capacidad de la interfaz en 2.2 (driver de la cola de sincronización) |


---

## 2. Data Design

### 2.1 Principal information the system needs

| Information | Needed by (use cases) | Persisted? | Why (or why not) |
|---|---|---|---|
| Usuario y clave de usuario | UC-1; UC-4 | Yes | Las credenciales de usuario deben persistir para poder realizar autenticación varias veces. |
| Estado y Justificación de solicitudes de reasignación | UC-1; UC-4; UC-5 | Yes | Una o varias solicitudes pueden ser consultadas varias veces en un periodo arbitrario de tiempo por un administrador o estudiante. Además, al crearse una solicitud, un administrador debe poder revisarla después. |
| Asignaciones de comedores | UC-2; UC-4 | Yes | Un usuario administrativo puede reasignar un comedor y el sistema luego notificará al estudiante; estas notificaciones deben ser guardadas en un historial |
| Capacidad y ubicación de comedores | UC-2 | Yes | Este dato no se puede inferir a partir de otros, está relacionado a los comedores y es utilizado al recomendar comedores |
| Distancia del comedor al estudiante | UC-2 | No | Este dato se calcula a partir de la capacidad del comedor y depende también del horario del estudiante. Al haber tantos escenarios posibles que resulten en distintas distancias, es necesario calcular este dato dinámicamente en vez de guardarlo en un registro |


*(Data owned by an external system, or recomputable on demand, is often better not persisted. If that is your decision, say so.)*

### 2.2 Data model
**Justificación del modelo de datos elegido:** Acorde a [ADR-0001](docs/adr/0001-Usar-postgresql-como-base-de-datos-principal.md), se acordó usar un tipo de almacenamiento relacional, además las relaciones son el centro del dominio (solicitudes enviadas por estudiantes y evaluadas por un administrativo). Por esto, conviene usar un diagrama ER.

```mermaid
erDiagram
    Administrativo |o--o{ Usuario : "tiene rol"
    Estudiante |o--o{ Usuario : "tiene rol"
    Estudiante ||--o{ Solicitud: "realiza"
    Administrativo ||--o{ Asignacion: "toma"
    Solicitud ||--|| Comedor: "sugiere"
    Solicitud o{--|| EstadoSolicitud: "tiene"
    Estudiante ||--o{ Notificacion: "recibe"
    Usuario{
        int id_usuario PK
        varchar(32) correo UK
        varchar(32) clave UK
        int id_rol FK
    }
    Administrativo{
        int id_administrativo PK
        int id_usuario FK
    }
    Estudiante{
        int id_estudiante PK
        int id_usuario FK
        int[] id_comedores FK
    }
    Solicitud{
        int id_solicitud PK
        varchar(4000) justificacion
        varchar(500) evidencia
        DATE fecha_de_envio
        int id_estudiante FK
        int id_comedor FK
        int id_estado FK
    }
    Asignacion{
        int id_decision PK
        int id_solicitud FK
        int id_administrativo FK
        DATE fecha
        varchar(4000) motivo
        bool aprovado
        int id_comedor FK
    }
    Comedor{
        SERIAL id_comedor PK
        varchar(32) nombre UK
        int capacidad_maxima
        GEOMETRY ubicacion
    }
    EstadoSolicitud{
        int id_estuado PK
        varchar(32) nombre UK
    }
    Notificacion{
        int id_notificacion PK
        DATE fecha
        int id_asignacion FK
        int id_solicitud FK
        int id_estado FK
    }
```

### 2.3 Data Ownership

| Information | Authoritative owner (component responsible for maintaining it) | Consumed by (uses it without owning it) | Access through |
|---|---|---|---|
| **Casos Atípicos (Solicitudes, justificación, estado y decisiones)** | `Gestión de Casos` *(Nota: delega el almacenamiento físico a `Registros`)* | `Interfaz de Solicitudes del Estudiante`, `Panel de Casos Atípicos` | Métodos de clase (Sección 3.2):<br>`ManejoSolicitudes.consultarSolicitudes()`<br>`ManejoSolicitudes.responderSolicitud()`<br>`ManejoSolicitudes.realizarSolicitud()` |
| **Asignaciones individuales vigentes** (incluye flag `origen: manual \| automatica` según R-4) | `Gestión de Asignaciones` *(Nota: delega el almacenamiento físico a `Registros`)* | `Gestión de Casos` | Capacidades de componente (Sección 2.2):<br>`Asignación`<br>`Administrativo` |
| **Carga asignada por comedor** (métrica calculada de estudiantes por día) | `Gestión de Asignaciones` | `Sugerencia de Comedores` | Capacidad de componente (Sección 2.2):<br>Derivada de `Asignacion` agrupada por `id_comedor` y día; no es una tabla.|
| **Capacidades máximas estáticas de los comedores** | `Registros` (Datos de configuración administrativa) | `Gestión de Asignaciones` | Capacidad de componente (Sección 2.2):<br>`Comedor` |
| **Historial de Notificaciones** (Estado de visualización pull in-app, L9) | `Notificaciones al Estudiante` (Lee la decisión maestra desde `Registros`) | `Interfaz de Solicitudes del Estudiante` | Capacidad de componente (Sección 2.2):<br>`Notificacion`|
| **Borradores y solicitudes locales encoladas** (Offline / AC-10) | `Interfaz de Solicitudes del Estudiante` / `Panel de Casos Atípicos` | Ninguno (uso interno del cliente) | Almacenamiento local de la PWA (`LocalStorage` / `IndexedDB`) |
| **Cuentas, Usuarios y Roles** | Sistema de usuarios de la universidad (Externo) | `Interfaz de Solicitudes del Estudiante`, `Panel de Casos Atípicos` | API externa (simulada) de verificación |

## 2.4 Business rules and invariants enforced in data

| Business rule or invariant | Where it is enforced (component logic / data constraint / both) | How |
|---|---|---|
| **R-6: Un estudiante puede tener una sola solicitud pendiente a la vez** | Ambos | **Logica:** `Gestión de Casos` verifica la existencia de solicitudes pendientes antes de crear una nueva. <br>**Data constraint:** Índice único parcial en la base de datos sobre `(id_estudiante)` donde el `estado = 'PENDIENTE'`. |
| **R-5: Toda solicitud debe incluir una justificación escrita** | Ambos | **Logica:** `ManejoSolicitudes.realizarSolicitud()` rechaza cadenas vacías o compuestas solo de espacios.<br>**Data constraint:** La columna `justificacion` tiene una restricción `NOT NULL` y un `CHECK (length(trim(justificacion)) > 0)`. |
| **R-4: Un cambio aprobado es permanente y sobrevive al recálculo** | Ambos | **Logica:** El script de recálculo (Entrega 2) excluye o salta los registros marcados como manuales.<br>**Data constraint:** La entidad de asignación tiene el atributo obligatorio `origen` con valores permitidos `(AUTOMATICA, MANUAL)`. `Gestión de Asignaciones` fija este valor en `MANUAL` al reasignar. |
| **R-9: Cada decisión queda registrada (quién, cuándo, resultado y motivo)** | Data constraint | La tabla/documento de resolución de la solicitud exige restricciones `NOT NULL` para `id_administrador`, `fecha_decision`, y `resultado`. Si el resultado es `RECHAZADO`, la columna `motivo_rechazo` debe ser obligatoria mediante un constraint condicional (`CHECK`). |
| **AC-10: Idempotencia en el envío de solicitudes frente a fallos de red** | Ambos | **Logica:** El cliente genera un UUID `idEnvio` antes de intentar la petición. Si falla y reintenta, envía el mismo ID.<br>**Data constraint:** La columna `id_envio` en la tabla de solicitudes tiene una restricción `UNIQUE`. Un reintento duplicado genera un error de conflicto (409) manejado sin crear registros extra. |
| **R-8: El administrador debe justificar explícitamente si la asignación supera el aforo del 80%** | Component logic | `Gestión de Casos` evalúa si la `carga asignada` resultante supera el `aforo operativo (80%)` usando `GA.obtenerCargaAsignada()`. Si es así, la operación exige que el administrador ingrese un texto de anulación antes de permitir el registro. No se fuerza estrictamente en DB para permitir la contingencia administrativa. |
---

## 3. Interface Contract

Select **one** important interaction from your sequence diagrams and specify the agreement between the two components. Technology (REST, JSON, OpenAPI) is not required.

**Interaction chosen** (from diagram 1.X, message `...`):
>

| Element | Specification |
|---|---|
| **Operation and purpose** | |
| **Input** (parameters and types) | |
| **Output** (successful result and data structure) | |
| **Preconditions** (what the caller must satisfy before invoking) | |
| **Postconditions** (what is guaranteed on successful completion) | |
| **Failure outcomes** (each failure, and what state is left after it) | |
| **Invariants and side effects** (what must stay true, what may change) | |

**Testable examples** *(optional but recommended: concrete values make guarantees checkable)*

| Case | Before | Request | Expected |
|---|---|---|---|
| | | | |

---

## 4. Cross-Consistency & Changes to Deliverable 1

| Check | Result |
|---|---|
| Every participant in a sequence diagram is an actor, a component, a class or an external system from Deliverable 1 | |
| Every message corresponds to a capability, operation or contract | |
| Every entity or document type is reflected in the class diagrams, or the reason it is not is stated | |
| Every persisted information element has exactly one authoritative owner | |
| The contract agrees with the sequence diagram and with the data model (same behavior in all three views) | |

**Changes made to Deliverable 1 because of this deliverable** *(real designs change when behavior and data become concrete. List what you updated, or write "None" and why.)*
>

---

## 5. Open Questions

> Anything unresolved about behavior or data. List it here instead of guessing.

---

## 6. Optional — AI-Assisted Design Reflection

> One design recommendation generated by AI that the team accepted, modified or rejected, and why.

---

## 7. Consistency Checklist

- [ ] Two use cases from the project, each traceable to a Use Case written in the requirements.
- [ ] Each sequence diagram shows the main flow and at least one alternative or exception flow.
- [ ] No business behavior is invented: anything unspecified is marked as an assumption or Open Question.
- [ ] Section 2.1 explains what is NOT persisted as well as what is.
- [ ] The data model has identifiers, relationships and constraints, not only boxes.
- [ ] Every piece of principal information has one authoritative owner, and consumers are listed (Section 2.3).
- [ ] The contract defines input, output, preconditions, postconditions and failure outcomes.
- [ ] Sequence, data model and contract describe the same behavior.
- [ ] Diagrams render correctly on GitHub (or the exported images are embedded).
