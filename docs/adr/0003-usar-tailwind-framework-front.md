# ADR-0003: Usar Tailwind CSS como framework de estilos para el frontend

Autores:
- @amadodev07
- @juanfpg1

Fecha: 2026-10-30

## Estado

Aceptado

## Contexto

Para el desarrollo del proyecto se requiere construir una interfaz de
usuario visualmente atractiva, responsiva y funcional que permita maquetar
componentes de forma rápida sin comprometer el rendimiento.

Entre las restricciones y necesidades clave del proyecto se encuentran:
1. **Visualización de datos:** necesidad de integrar paneles estadísticos y
   gráficos que muestren la lógica procesada por el backend en C++.
2. **Velocidad de iteración:** el equipo requiere una curva de aprendizaje
   baja y una alta velocidad de maquetación para evitar construir
   componentes visuales complejos desde cero.
3. **Ecosistema y soporte:** contar con el respaldo de una comunidad activa
   y un amplio ecosistema de librerías complementarias de UI.

## Decisión

Vamos a utilizar Tailwind CSS como la solución principal para la
maquetación e implementación de la interfaz de usuario del proyecto.

## Alternativas consideradas

- **Slint:** se evaluó como alternativa nativa para C++. Se descartó por su
  ecosistema limitado para la visualización de datos estadísticos y
  gráficos complejos, la necesidad de aprender un lenguaje de marcado
  distinto y la escasez de componentes UI preconstruidos en comparación
  con el ecosistema web.
- **Dear ImGui:** se consideró por su alto rendimiento en C++ nativo. Se
  descartó porque su enfoque visual en modo inmediato dificulta la creación
  de una interfaz moderna y requiere trabajo manual.

## Consecuencias

### Positivas (+)
- **Integración sencilla de estadísticas:** permite utilizar directamente
  librerías de gráficos en JS (como Chart.js) alimentadas por los datos
  procesados por el backend C++.
- **Bajo impacto en rendimiento:** Tailwind genera en la compilación solo
  las clases utilizadas y no requiere JavaScript en el navegador, por lo
  que el CSS resultante es pequeño y estático.
- **Ecosistema robusto:** facilita la integración futura de bibliotecas de
  componentes modernos y sistemas de diseño reutilizables.

### Negativas / Desafíos (-)
- **Acoplamiento al entorno web:** requiere mantener una capa de
  comunicación entre JavaScript y la lógica en C++, que se implementa con
  PHP según lo establecido en el ADR-0002.
