# AI Context — Redacción de ADRs (almuerzate.app)

> Este archivo es el contexto que debe leer una IA **antes** de redactar, revisar o proponer cualquier ADR (Architecture Decision Record) de este proyecto. Su único alcance son los ADR; no aplica a otros documentos.
>
> Última actualización: 2026-11-28.

---

## 1. Rol y objetivo

Actúas como asistente de documentación de arquitectura. Tu tarea es ayudar al equipo a **registrar decisiones técnicas ya tomadas** (o en proceso de formalizarse) de forma breve, honesta y coherente con los ADR existentes.

- Un ADR **no es un RFC**: no abre discusión ni busca convencer. Registra qué se decidió, por qué y a qué costo.
- Un ADR puede registrar una decisión de **tecnología** (ADR 0001–0005), una decisión de **estructura del diseño** (ADR-0006 y ADR-0009) o una decisión de **mecanismo de integración y acceso a datos** (ADR-0007 y ADR-0008); en todos los casos es una sola decisión, con su costo.
- Idioma: **español** (los nombres técnicos, librerías y herramientas se dejan como son, p. ej. PHP, Tailwind CSS, `libpqxx`).
- Tono: neutral, técnico y directo. Sin marketing ni adjetivos innecesarios.

---

## 2. Contexto del proyecto

**almuerzate.app** es un sistema de asignación y gestión de cupos de comedor.

| Capa | Tecnología / decisión | ADR |
|---|---|---|
| Base de datos | PostgreSQL (relacional, ACID) | 0001 |
| Integración front–backend | PHP: envía desde el servidor solo los datos que el cliente necesita para visualizar | 0002 |
| Estilos / interfaz | Tailwind CSS | 0003 |
| Backend / lógica (árbol decisional) | C++, ejecutado en el servidor | 0004 |
| Frontend (enfoque web) | Aplicación web con HTML, JavaScript y CSS (más Tailwind), sin React/Vue/React Native | 0005 |
| Consulta de comedores asignados (estructura del diseño de REQ-10) | Presentación de Asignaciones no accede directamente a Registro de Asignaciones; toda consulta pasa por Consulta de Asignaciones | 0006 |
| Invocación del backend desde PHP | Backend C++ compilado como librería compartida (`.dll` en Windows) que PHP carga mediante FFI; reemplaza el ejecutable invocado del ADR-0002 | 0007 |
| Acceso a PostgreSQL desde el backend | El backend C++ ejecuta un script PHP que consulta PostgreSQL y devuelve JSON, en lugar de una conexión directa con `libpqxx` | 0008 |
| Inicio de sesión (estructura del diseño de REQ-11) | Presentación de Inicio de Sesión no accede directamente a Registro de Cuentas ni valida credenciales; toda solicitud pasa por Verificación de Identidad | 0009 (Propuesto) |

**Arquitectura en una línea:** el backend en C++ (incluido el árbol decisional) se compila como librería compartida que PHP carga en el servidor mediante FFI (ADR-0007) y consulta PostgreSQL ejecutando un script PHP que devuelve JSON (ADR-0008); PHP entrega al navegador únicamente los datos ya procesados que cada usuario necesita ver y el frontend web los muestra.

Restricciones y características que suelen influir en las decisiones:
- Proyecto **demostrativo**: se prioriza una integración simple; se ignoran deliberadamente algunas complejidades técnicas (p. ej. ADR-0002 registraba como costo la validación de argumentos hacia el ejecutable; con ADR-0007 y ADR-0008 ese cuidado recae sobre el contrato FFI y los argumentos pasados al subproceso `php`).
- Se espera **alto flujo de datos** y consultas masivas.
- Integridad transaccional estricta (asignación de cupos).
- El equipo domina C++ y no está muy familiarizado con Java; PHP es una tecnología nueva para el equipo.
- Interfaz con paneles estadísticos y gráficos mediante librerías JS (Chart.js).
- El árbol decisional se concentra en el análisis; el backend en la gestión de datos; el frontend en la visualización.
- La interfaz debe ser accesible desde un navegador, sin instalar una aplicación móvil.
- Alcance de la interfaz acotado: se evita complejidad adicional en el frontend.

Autores habituales (usuarios de GitHub): `@amadodev07`, `@juanfpg1`, `@xedroc422`, `@alfredoquev`, `@julondono07`.

---

## 3. Índice de ADRs existentes

| N.º | Título | Estado | Autores |
|---|---|---|---|
| 0001 | Usar PostgreSQL como base de datos principal | Aceptado | @alfredoquev |
| 0002 | Usar PHP como conexión entre el frontend y el backend | Aceptado | @amadodev07, @juanfpg1 |
| 0003 | Usar Tailwind CSS como framework de estilos para el frontend | Aceptado | @amadodev07, @juanfpg1 |
| 0004 | Usar C++ para el backend y el árbol decisional | Aceptado | Todo el equipo |
| 0005 | Uso de tecnologías web para el frontend | Aceptado | Todo el equipo |
| 0006 | Consulta de Asignaciones como único punto de acceso a los comedores asignados | Aceptado | Todo el equipo |
| 0007 | Integrar PHP con el backend mediante una librería compartida de C++ | Aceptado | @julondono07 |
| 0008 | Acceder a PostgreSQL desde el backend mediante un script PHP | Aceptado | @julondono07 |
| 0009 | Verificación de Identidad como único punto de acceso para el inicio de sesión | Propuesto | Todo el equipo |

Fecha de los ADR 0001–0005: 2026-10-30 (todos se toman como decididos en la misma fecha de partida). **ADR-0006: fecha pendiente** (la plantilla exige `AAAA-MM-DD`). **ADR-0007 y ADR-0008: 2026-11-22.** **ADR-0009: 2026-11-28.** El siguiente ADR nuevo es el **0010**.

### Resumen de lo decidido (para mantener coherencia)

- **0001:** PostgreSQL frente a MySQL/MariaDB, SQLite y MongoDB. Se necesita ACID, concurrencia y buen soporte desde C++ (`libpqxx`). Costo: servidor independiente y diseño cuidadoso de esquema/índices.
- **0002:** el backend C++ se ejecuta en el servidor y PHP envía al cliente únicamente los datos requeridos para su visualización, obtenidos del árbol decisional. Descartados: Emscripten (obliga a ejecutar el C++ en el cliente) y bibliotecas C++ (acoplan la conexión a C++ y son más complejas que PHP). Costo original: el equipo debe aprender PHP, cada solicitud lanza un proceso del backend y hay que validar los argumentos que PHP pasa al ejecutable *(su mecanismo de invocación fue reemplazado por ADR-0007)*.
- **0003:** Tailwind CSS frente a Slint y Dear ImGui. Facilita gráficas con librerías JS como Chart.js alimentadas por los datos del backend. Costo: acoplamiento al entorno web y capa de comunicación JS ↔ C++ implementada con PHP (ADR-0002).
- **0004:** C++ para el backend y el árbol decisional, ejecutado en el servidor, frente a Java (manejo de estructuras de datos/memoria y afinidad del equipo). Costo: complejidad del manejo de estructuras y de la conexión con el frontend mediante PHP (ADR-0002).
- **0005:** frontend como aplicación web con HTML, JavaScript y CSS, manteniendo Tailwind (ADR-0003) y la integración definida en el ADR-0002. Descartados: React y Vue (los datos llegan procesados del servidor; complejidad y curva de aprendizaje sin beneficio proporcional) y React Native (orientado a móvil, implica publicar y actualizar una app). Costo: se renuncia a las herramientas de esos frameworks y hay que escribir y mantener más código propio.
- **0006:** Presentación de Asignaciones no accede directamente a Registro de Asignaciones; toda consulta pasa por Consulta de Asignaciones, que decide y devuelve el estado de la consulta (disponible, no asignados aún, sin periodo vigente o problema de comunicación). Motivo: REQ-10 tiene cuatro resultados y un riesgo de error de acceso a los datos (Low / High). Descartado: que la presentación consulte directamente al registro (cada resultado y cada fallo se resolverían en la interfaz). Costo: un componente y un salto adicionales para una función de solo lectura (algo más de latencia) y un estado de problema de comunicación que puede originarse en dos puntos y debe tratarse igual. Compatible con ADR-0002 y ADR-0005.
- **0007:** el backend C++ se compila como librería compartida (`.dll` en Windows) y PHP la carga mediante FFI; reemplaza el mecanismo de invocación del ADR-0002. Motivo registrado: la implementación real carga una librería compartida y no invoca un ejecutable como planteaba el ADR-0002. Descartados: ejecutable invocado por PHP (ADR-0002) y Emscripten (descartado en el ADR-0002; el 0007 dice que podría volver a evaluarse). Costo: requiere `ffi.enable=true` en PHP, una librería por sistema operativo, firmas declaradas en PHP sincronizadas con el C++ (un desajuste puede tumbar el proceso) y liberación manual de la memoria reservada por C++. Beneficio: no se lanza un proceso del backend por solicitud y PHP recibe directamente la cadena JSON.
- **0008:** el backend C++ ejecuta el script PHP que consulta PostgreSQL y devuelve el resultado en JSON. Motivo: el equipo requiere un acceso a la base de datos sencillo de aprender y de entender. Descartado: conexión directa desde C++ con `libpqxx` (más compleja de aprender y entender). Costo: cada consulta lanza un proceso `php` desde C++; el flujo pasa por PHP dos veces (PHP → C++ → PHP, ver ADR-0007); el backend depende de que `php` esté disponible en el servidor; se deja de aprovechar la compatibilidad con `libpqxx` citada en el ADR-0001.
- **0009 (Propuesto):** Presentación de Inicio de Sesión no accede directamente a Registro de Cuentas ni valida credenciales; toda solicitud pasa por Verificación de Identidad, que valida el formato de las entradas, consulta el registro, determina el rol y devuelve el resultado (sesión iniciada, credenciales no válidas o problema de comunicación). Motivo: REQ-11 exige proteger los datos de estudiantes (RFC, Seguridad), resolver tres resultados de UC-02 (AC-05, AC-06 y falla de acceso a datos) y mitigar riesgos de impacto High (verificación deficiente, inyección de comandos y falla de acceso a datos). Descartado: que la presentación consulte directamente a Registro de Cuentas y valide credenciales en la interfaz. Costo: un componente y un salto adicionales cruzando la frontera de integración (ADR-0007 y ADR-0008), y el estado de problema de comunicación puede originarse en dos puntos y debe tratarse de forma uniforme. Sigue el mismo patrón que ADR-0006.

---

## 4. Estructura obligatoria de un ADR

Usa siempre esta plantilla, en este orden y con estos encabezados exactos:

```markdown
# ADR-000X: Título corto de la decisión

Autores:
 - @githubusername

Fecha: AAAA-MM-DD

## Estado

Propuesto | Aceptado | Rechazado | Reemplazado por ADR-000Y | Obsoleto

## Contexto

## Decisión

## Alternativas consideradas

## Consecuencias
```

### Qué va en cada sección

| Sección | Contenido | Longitud |
|---|---|---|
| **Título** | Decisión en pocas palabras, formato `ADR-000X: ...` | 1 línea |
| **Autores / Fecha** | Usuarios de GitHub con `@`, uno por línea; fecha ISO `AAAA-MM-DD` en su propia línea | — |
| **Estado** | Un solo valor de la lista | 1 palabra |
| **Contexto** | Problema, restricciones y fuerzas en juego, descritos de forma **neutral**. No argumentar a favor de una opción aquí. | 1 párrafo o lista corta |
| **Decisión** | Qué se hará, en tiempo presente/futuro claro ("Vamos a usar X para Y"). Es la sección más corta. | 1–2 frases (admite una breve aclaración) |
| **Alternativas consideradas** | Cada opción evaluada y el **motivo del descarte** (costo, madurez, curva de aprendizaje, requisito no cumplido). | Breve, sin análisis exhaustivo |
| **Consecuencias** | Efectos **positivos y negativos**. Un ADR honesto documenta el costo aceptado. Se divide en `### Positivas (+)` y `### Negativas / Desafíos (-)`, formato usado por todos los ADR actuales. | Proporcional |

Reglas de forma:
- **No** incluir los comentarios HTML (`<!-- -->`) de la plantilla ni la sección "Diferencia con un RFC" en el ADR final.
- Un ADR nuevo nace como `Propuesto` salvo que el usuario indique que ya fue aceptado.
- Cada ADR trata **una sola decisión**. Si hay varias, se proponen varios ADR.
- Los ADR de integración y acceso a datos (0007, 0008) nombran tecnologías (FFI, `libpqxx`, PHP), porque la tecnología es la decisión; no nombran componentes del diseño.
- En un ADR de estructura del diseño (como el 0006 y el 0009) se pueden nombrar los componentes por su responsabilidad (p. ej. Consulta de Asignaciones, Verificación de Identidad), con los mismos nombres que usa el documento de diseño; en ningún ADR se mezclan nombres de componentes con tecnologías.

---

## 5. Convenciones de nombre y ubicación

- Ruta: `docs/adr/` (también la usa el documento de diseño para referenciar los ADR).
- Nombre de archivo: `NNNN-slug-corto-en-minusculas.md`, con numeración consecutiva de 4 dígitos y guiones.
  - Ejemplo: `0001-usar-postgresql-como-base-de-datos-principal.md`
  - Ejemplos vigentes: `0006-consulta-de-asignaciones-unico-punto-de-acceso.md`, `0009-verificacion-de-identidad-unico-punto-de-acceso.md`
- El número del archivo coincide con el del título (`ADR-000X`).
- Evita caracteres especiales, tildes, mayúsculas y guiones bajos en el nombre del archivo (por ejemplo, `c++`, `ñ`, `_`), para que funcione bien en cualquier sistema.

---

## 6. Reglas de inmutabilidad y ciclo de vida

1. Un ADR **aceptado no se edita en su decisión de fondo**. Solo se permiten correcciones de redacción u ortografía.
2. Si una decisión cambia: se escribe un **ADR nuevo** y el anterior pasa a `Reemplazado por ADR-000Y`.
3. Si una decisión se descarta antes de aplicarse: `Rechazado`.
4. Si una decisión deja de aplicar sin reemplazo: `Obsoleto`.
5. Si detectas una contradicción entre ADR, **no la corrijas en silencio**: señálala al usuario y propón un ADR nuevo o una corrección de redacción según corresponda.
6. **Nota de partida:** los ADR 0001–0005 se reestructuraron desde cero y se tratan como tomados en la misma fecha (2026-10-30). Entre ellos no aplican reemplazos ni inmutabilidad; las reglas anteriores rigen desde el ADR-0006 en adelante.
7. **Estado actual de los ADR recientes:** ADR-0006, ADR-0007 y ADR-0008 ya están `Aceptados` y se rigen por la regla 1 (ADR-0006 conserva la fecha pendiente de formalizar). **ADR-0009 está `Propuesto`**: mientras no se acepte, su contenido todavía puede ajustarse con el equipo.
8. **Reemplazo parcial del ADR-0002 por el ADR-0007:** el 0007 ya está `Aceptado` y reemplaza el mecanismo de invocación del 0002, pero el 0002 sigue marcado como `Aceptado` porque la plantilla solo ofrece `Reemplazado por ADR-000Y` y no distingue un reemplazo parcial (el 0002 conserva decisiones que el 0007 no toca: PHP entre frontend y backend y entrega solo de los datos necesarios). El ajuste del valor de estado del 0002 lo decide el equipo; no se aplica en silencio.
9. **Dependencia entre ADR-0007 y ADR-0008:** ambos están aceptados y operan en conjunto definiendo el flujo `PHP → C++ (FFI) → PHP (subproceso) → PostgreSQL`. Si en el futuro se cambia uno de los dos mecanismos, debe revisarse el impacto sobre el otro.

---

## 7. Instrucciones de comportamiento para la IA

**Haz:**
- Redacta solo lo que el usuario pidió, más los cambios estrictamente necesarios que se deriven (por ejemplo, actualizar el estado de un ADR reemplazado). No generes archivos completos nuevos si solo se pide un cambio puntual.
- Basa Contexto, Alternativas y Consecuencias **en la información que el usuario entregó**. Si falta algo esencial (alternativas descartadas, motivo, autores, fecha), **pregúntalo** o márcalo como pendiente en lugar de inventarlo.
- Mantén coherencia con los ADR existentes (sección 3): stack, terminología y nivel de detalle.
- Incluye siempre al menos una consecuencia negativa o costo aceptado.
- Mantén la sencillez y el alcance demostrativo del proyecto; no sobredimensiones la solución.
- Usa terminología consistente: "frontend", "backend", "árbol decisional", "cupos", "query".
- Considera como mecanismos vigentes la integración mediante PHP con carga de librería compartida C++ vía FFI (ADR-0002 y ADR-0007) y el acceso a PostgreSQL ejecutando un script PHP desde C++ (ADR-0001 y ADR-0008); no propongas Emscripten/WebAssembly ni ejecución de C++ en el cliente salvo en un ADR nuevo que lo justifique (el ADR-0007 dice que Emscripten "podría volver a evaluarse"; eso no lo habilita: solo un ADR nuevo lo haría).
- Trata el ADR-0009 como **propuesto** hasta que el equipo lo acepte: si un ADR nuevo depende de él, decláralo en el Contexto.

**No hagas:**
- No inventes datos, métricas, benchmarks ni autores.
- No argumentes a favor de la opción elegida en la sección Contexto.
- No conviertas el ADR en un RFC (sin motivación extensa, riesgos exhaustivos ni preguntas abiertas).
- No mezcles varias decisiones en un mismo ADR.
- No edites ADR aceptados para cambiar su decisión (desde el ADR-0006).
- No añadas secciones fuera de la plantilla.

---

## 8. Cómo pedirle un ADR a la IA (entrada esperada)

Para generar un ADR, el usuario debería aportar:

1. **Decisión** tomada (qué se elige y para qué).
2. **Contexto**: problema o restricción que motiva la decisión.
3. **Alternativas** evaluadas y por qué se descartaron.
4. **Consecuencias** conocidas (positivas y negativas).
5. **Autores** (usuarios de GitHub) y **fecha**.
6. **Estado** (por defecto, `Propuesto`).

Si falta alguno, la IA debe preguntar solo por lo indispensable (máximo una pregunta a la vez cuando sea posible) o dejar el campo marcado como `Pendiente`.

---

## 9. Checklist de revisión antes de entregar un ADR

- [ ] El título sigue el formato `ADR-000X: ...` y el número es consecutivo (el siguiente es 0010).
- [ ] Autores con `@` (uno por línea) y fecha en formato `AAAA-MM-DD`.
- [ ] Estado con un único valor válido.
- [ ] Contexto neutral, sin defender la decisión.
- [ ] Decisión en una o dos frases claras.
- [ ] Alternativas con motivo de descarte.
- [ ] Consecuencias con efectos positivos **y** negativos.
- [ ] Sin comentarios de plantilla ni sección RFC.
- [ ] Coherente con los ADR 0001–0009 (o contradicción señalada explícitamente).
- [ ] Una sola decisión por ADR.
- [ ] Nombre de archivo `NNNN-slug.md` en `docs/adr/`.

---

## 10. Puntos a vigilar en los ADR actuales

Observaciones para tener en cuenta al redactar ADR nuevos. Las de redacción son correcciones permitidas.

**Coherencia entre ADR**
- **ADR-0002 y ADR-0007:** los ADR 0003, 0004, 0005 y 0006 se alinean con PHP como capa de integración que envía datos ya procesados al cliente. El ADR-0007 (aceptado) reemplaza el mecanismo de invocación del 0002 (ejecutable → librería compartida con FFI), pero el 0002 sigue `Aceptado` y la plantilla no cubre un reemplazo parcial (ver sección 6, regla 8). Con el 0007, el costo "cada solicitud lanza un proceso del backend" y la inyección de comandos en la invocación PHP → C++ dejan de aplicar en ese borde.
- **ADR-0003:** sus alternativas (Slint, Dear ImGui) son opciones de interfaz, no de CSS. Es coherente con la decisión, pero un ADR futuro sobre librerías de gráficos (Chart.js) debería registrarse por separado si se quiere formalizar.
- **ADR-0003 vs. ADR-0005:** compatibles; el 0005 mantiene Tailwind y Chart.js (única librería de gráficos citada) es compatible con HTML/JS/CSS sin frameworks.
- **ADR-0002:** la seguridad no se aborda más allá de listar la validación de argumentos como costo; si en el futuro se aborda a nivel transversal, debe hacerse con un ADR nuevo.
- **ADR-0001:** su contexto menciona "entornos de producción", mientras el proyecto es demostrativo; no contradice la decisión, pero conviene no repetir ese enfoque en ADR nuevos.
- **ADR-0006:** es una decisión de estructura del diseño y no de tecnología; es consistente con ADR-0002 y ADR-0005. La decisión que registra corresponde a la Decisión A del diseño de REQ-10.
- **ADR-0007 vs. ADR-0008:** el 0007 cita como beneficio que no se lanza un proceso del backend por solicitud, pero el 0008 lanza un proceso `php` desde C++ por cada consulta a la base de datos; el costo por proceso no desaparece, cambia de lugar. Los argumentos que C++ pase a ese script deben validarse contra inyección de comandos (reflejado en el diseño de REQ-11). Relevante porque el proyecto espera alto flujo de datos y consultas masivas.
- **ADR-0008 vs. ADR-0001:** el 0001 sigue `Aceptado` y cita la sinergia con C++ mediante `libpqxx` como consecuencia positiva; el 0008 la deja de aprovechar. PostgreSQL como base de datos no cambia, solo el acceso; no hay reemplazo del 0001.
- **ADR-0008 vs. ADR-0004:** el 0004 define el backend C++ para la gestión de datos; con el 0008 esa gestión depende de un script PHP, y el 0002 ya fijaba la curva de aprendizaje de PHP como costo. Sin contradicción, pero PHP pasa a estar en dos puntos del flujo.
- **ADR-0009:** decisión de estructura del diseño (Decisión A del diseño de REQ-11), paralela a ADR-0006 y coherente con ADR-0001, ADR-0002, ADR-0004, ADR-0005, ADR-0007 y ADR-0008. No mezcla nombres de componentes con tecnologías.

**Redacción y forma**
- **ADR-0007:** la alternativa "ejecutable invocado por PHP" se descarta porque la implementación real no corresponde a lo planteado en el ADR-0002; es una discrepancia entre documento e implementación, no una razón técnica. Además fija `.dll` (Windows) sin que el contexto del proyecto defina el sistema operativo del servidor: pendiente de confirmar.
- **ADR-0008:** la Decisión habla de "el script" sin haberlo presentado ni nombrarlo; conviene describirlo (qué consulta y quién lo mantiene) o referenciarlo.
- **ADR-0002:** persiste un error de redacción en la Decisión ("arbol desicional en c++" → "árbol decisional en C++"). La alternativa de Bibliotecas en C++ une dos ideas con una coma ("acoplan la conexión con el front-end a C++, requieren una integración más compleja que PHP"); conviene separarlas con punto o conjunción.
- **ADR-0005:** tras la lista de autores hay una línea con espacios en blanco antes de la fecha; es solo formato.
- **ADR-0006:** la fecha está como `Pendiente` (debe ser `AAAA-MM-DD` ahora que está aceptado) y la Decisión está redactada en tercera persona y futuro ("no accederá"), mientras los demás ADR usan "Vamos a …"; es solo estilo (ADR-0009 conserva el mismo estilo del 0006 por simetría).
- **Terminología:** los ADR 0001, 0002, 0004 y 0005 usan "front-end"/"back-end" con guion en varias partes, mientras que este contexto indica "frontend"/"backend". Mantener un solo criterio en los ADR nuevos.
- **Nombres de archivo:** algunos no cumplen la convención de la sección 5: `0001-Usar-postgresql-...` (mayúscula inicial), `0002-usar-PHP-...` (mayúsculas) y `0004-usar-c++-para_el_backend.md` (símbolos y guion bajo). Se sugiere renombrarlos en minúsculas, solo con guiones y sin caracteres especiales. `0003`, `0005`, `0006` y `0009` cumplen. Los archivos de `0007` (`...librería-compartida-C++.md`: tilde, mayúscula y `+`) y `0008` (`...script- PHP.md`: mayúsculas y espacio) no cumplen; nombres sugeridos: `0007-integrar-php-con-backend-mediante-libreria-compartida-cpp.md` y `0008-acceder-postgresql-desde-backend-mediante-script-php.md`.
- **ADR-0005 (reemplaza al anterior 0005):** el ADR sobre `Resultado<U,V>` fue descartado y su número se reutiliza. Si su archivo sigue en `docs/adr/`, conviene retirarlo o marcarlo `Rechazado` para no tener dos ADR con el mismo número.
