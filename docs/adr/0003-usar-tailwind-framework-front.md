# ADR-0003: Uso de Tailwind como Framework para el Font-End
 
<!--
Nombra el archivo con el número consecutivo y un slug corto, por ejemplo:
docs/adr/0001-usar-postgresql-como-base-de-datos-principal.md
Los ADR se numeran en orden y NUNCA se editan después de aceptados para
cambiar la decisión en sí (sí puedes corregir errores de redacción). Si la
decisión cambia, escribes un ADR nuevo y marcas este como reemplazado
(ver "Estado"). Así el repositorio queda como una bitácora histórica de
por qué el sistema es como es.
-->
 
Autores:
 - @amadodev07
 - @juanfpg1
 - @xedroc422
 - @alfredoquev
 - @julondono07
Fecha: 2026-09-12 
## Estado
 
<!--
Uno de: Propuesto | Aceptado | Rechazado | Reemplazado por ADR-000Y | Obsoleto
Un ADR "Propuesto" está en discusión. Una vez el equipo decide, pasa a
"Aceptado" (o "Rechazado" si se descarta la propuesta) y ya no se
modifica su contenido de fondo.
-->
 
Aceptado
 
## Contexto
 
<!--
¿Qué problema técnico, restricción o fuerza nos obliga a tomar esta
decisión ahora? Describe la situación de forma neutral y objetiva —
todavía no es el lugar para argumentar a favor de una opción.
Ejemplos de fuerzas en juego: requisitos no funcionales (rendimiento,
seguridad, escalabilidad), restricciones de equipo o de tiempo,
deuda técnica existente, compatibilidad con sistemas ya construidos.
-->
Para el desarrollo del proyecto se requiere construir una interfaz de usuario visualmente atractiva, responsiva y funcional que permita maquetar componentes de forma rápida sin comprometer el rendimiento. 

Entre las restricciones y necesidades clave del proyecto se encuentran:
1. Visualización de datos: Necesidad de integrar paneles estadísticos y gráficos interactuando con la lógica de procesada en el backend en C++.
2. Velocidad de iteración: El equipo requiere una curva de aprendizaje baja y una alta velocidad de maquetación para evitar construir componentes visuales complejos desde cero.
3. Ecosistema y soporte: Contar con el respaldo de una comunidad activa y un amplio ecosistema de librerías complementarias de UI.
## Decisión
 
<!--
Qué vamos a hacer, en una o dos frases claras y en tiempo presente
("Vamos a usar X para Y"). Esta es la sección más corta del documento:
un ADR no es un RFC, no necesita convencer a nadie aquí — la
justificación ya quedó en "Contexto" y las alternativas descartadas
van abajo.
-->
Vamos a utilizar Tailwind CSS como la solución principal para la maquetación e implementación de la interfaz de usuario del proyecto.
## Alternativas consideradas
 
<!--
Qué otras opciones se evaluaron y por qué se descartaron. No hace
falta un análisis exhaustivo, basta con dejar constancia de que se
consideraron y el motivo del descarte (costo, madurez, curva de
aprendizaje, no cumple un requisito, etc.).
-->
* Slint: Se evaluó como alternativa nativa para C++. Se descartó debido a su ecosistema limitado para la visualización de datos estadísticas/gráficos complejos, la necesidad de aprender un lenguaje de marcado distinto y la escasez de componentes UI preconstruidos en comparación con el ecosistema web.
* Dear ImGui: Se consideró por su alto rendimiento en C++ nativo. Se descartó porque su enfoque visual en modo inmediato dificulta la creación de una interfaz comercial moderna y requiere trabajo manual.
## Consecuencias
 ### Positivas (+)
* Integración sencilla de estadísticas: Permite utilizar directamente librerías de gráficos en JS (como Chart.js, Recharts o Tremor) alimentadas por los datos procesados por el backend C++.
* Cero impacto en rendimiento: El CSS final es ultraligero y solo incluye las clases utilizadas.
* Ecosistema robusto: Facilita la integración futura de bibliotecas de componentes modernos y sistemas de diseño reutilizables.

### Negativas / Desafíos (-)
* Acoplamiento al entorno Web: Requiere mantener una capa de comunicación (IPC, WebSockets o bindings) entre JavaScript y la lógica nativa en C++.
<!--
¿Qué se vuelve más fácil o más difícil después de esta decisión?
Incluye efectos positivos y negativos por igual — un ADR honesto
también documenta el costo que se está aceptando (deuda técnica
introducida, dependencia nueva, curva de aprendizaje del equipo).
-->
   
