# ADR-0003: Uso de Tailwind como Framework para el Font-End
 
 
Autores:
 - @amadodev07
 - @juanfpg1

Fecha: 2026-09-12 
## Estado
 
 
Aceptado
 
## Contexto
 

Para el desarrollo del proyecto se requiere construir una interfaz de usuario visualmente atractiva, responsiva y funcional que permita maquetar componentes de forma rápida sin comprometer el rendimiento. 

Entre las restricciones y necesidades clave del proyecto se encuentran:
1. Visualización de datos: Necesidad de integrar paneles estadísticos y gráficos interactuando con la lógica de procesada en el backend en C++.
2. Velocidad de iteración: El equipo requiere una curva de aprendizaje baja y una alta velocidad de maquetación para evitar construir componentes visuales complejos desde cero.
3. Ecosistema y soporte: Contar con el respaldo de una comunidad activa y un amplio ecosistema de librerías complementarias de UI.
## Decisión

Vamos a utilizar Tailwind CSS como la solución principal para la maquetación e implementación de la interfaz de usuario del proyecto.
## Alternativas consideradas
 

* Slint: Se evaluó como alternativa nativa para C++. Se descartó debido a su ecosistema limitado para la visualización de datos estadísticas/gráficos complejos, la necesidad de aprender un lenguaje de marcado distinto y la escasez de componentes UI preconstruidos en comparación con el ecosistema web.
* Dear ImGui: Se consideró por su alto rendimiento en C++ nativo. Se descartó porque su enfoque visual en modo inmediato dificulta la creación de una interfaz comercial moderna y requiere trabajo manual.
## Consecuencias
 ### Positivas (+)
* Integración sencilla de estadísticas: Permite utilizar directamente librerías de gráficos en JS (como Chart.js, Recharts o Tremor) alimentadas por los datos procesados por el backend C++.
* Cero impacto en rendimiento: El CSS final es ultraligero y solo incluye las clases utilizadas.
* Ecosistema robusto: Facilita la integración futura de bibliotecas de componentes modernos y sistemas de diseño reutilizables.

### Negativas / Desafíos (-)
* Acoplamiento al entorno Web: Requiere mantener una capa de comunicación (IPC, WebSockets o bindings) entre JavaScript y la lógica nativa en C++.

   
