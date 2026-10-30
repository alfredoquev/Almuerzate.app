# ADR-0006: Uso de tecnologías web para el frontend

Autores: - @amadodev07 - @juanfpg1 - @xedroc422 - @alfredoquev -
@julondono07 Fecha: 2026-10-29

## Estado

Propuesto

## Contexto

El proyecto requiere una interfaz mediante la cual los usuarios puedan
consultar información e interactuar con el sistema. Su desarrollo debe
considerar la integración con el backend y las condiciones de uso de la
aplicación:

-   **Integración con el backend:** el frontend debe permitir la
    comunicación con el backend desarrollado en C++, cuya integración
    con JavaScript se realiza mediante Emscripten y WebAssembly, según
    lo establecido en el ADR-0002.
-   **Accesibilidad:** la interfaz debe estar disponible para los
    usuarios sin exigir la instalación de una aplicación móvil.
-   **Complejidad de desarrollo:** el alcance de la interfaz es acotado,
    por lo que se busca evitar complejidad adicional en su
    implementación.

## Decisión

Vamos a desarrollar el frontend como una aplicación web utilizando HTML,
JavaScript y CSS. Se mantiene Tailwind CSS como framework de estilos, de
acuerdo con la decisión registrada en el ADR-0003.

## Alternativas consideradas

-   **React y Vue:** se descartaron por la complejidad adicional y la
    curva de aprendizaje que supondría incorporarlos al desarrollo del
    frontend.
-   **React Native:** se descartó porque está orientado al desarrollo de
    aplicaciones móviles, mientras que el proyecto requiere una interfaz
    web accesible desde un navegador. Además, su distribución y
    actualización implicarían gestionar una aplicación móvil y sus
    mecanismos de publicación.

## Consecuencias

### Positivas (+)

-   La interfaz podrá utilizarse desde un navegador web, sin requerir la
    instalación de una aplicación móvil.
-   El equipo tendrá control directo sobre la estructura, el
    comportamiento y los estilos del frontend.
-   Se utilizarán tecnologías conocidas por el equipo de desarrollo.
-   La integración con el backend podrá realizarse mediante JavaScript y
    el mecanismo definido en el ADR-0002.

### Negativas / Desafíos (-)

-   Se descartan frameworks como React y Vue, que podrían aportar
    herramientas y componentes para acelerar ciertas tareas de
    desarrollo.
-   La implementación directa de la estructura, las interacciones y los
    componentes de la interfaz requerirá escribir y mantener más código
    propio.
