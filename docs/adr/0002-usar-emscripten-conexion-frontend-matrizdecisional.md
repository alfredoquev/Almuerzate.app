# ADR-0002: Usar Emscripten como integración y conexión del frontend con el backend

Autores:
- @amadodev07
- @juanfpg1

Fecha: 2026-09-12

## Estado

Aceptado


## Contexto

El árbol decisional estará desarrollado en C++ y debe integrarse con un frontend web desarrollado con Tailwind CSS. El proyecto es demostrativo y requiere una integración simple, preparada para un alto flujo de datos, dejando de lado deliberadamente algunas complejidades técnicas como la seguridad.

Las fuerzas y restricciones clave que impulsan esta decisión son:
1. **Reutilización de la lógica existente:** El árbol decisional ya está implementado en C++ y se busca reutilizarlo sin reescribirlo ni convertirlo en un servicio independiente.
2. **Simplicidad de la integración:** Al ser un proyecto demostrativo, la conexión entre el frontend y el árbol decisional debe ser sencilla y proporcional al alcance del proyecto.
3. **Separación de responsabilidades:** El árbol decisional debe concentrarse únicamente en el análisis de los datos recibidos, mientras que la gestión de los datos y la comunicación con la base de datos corresponde al backend.


## Decisión

Vamos a utilizar Emscripten como único mecanismo de integración entre el frontend y el backend.


## Alternativas consideradas

- **APIs C++:** Se descartaron en favor del uso exclusivo de Emscripten, ya que requerirían un servicio adicional y una complejidad mayor a la necesaria para esta implementación.
- **Ejecutable C++:** Se descartó por ser menos práctico para aplicaciones web.
- **Biblioteca C++ en el backend:** Se descartó porque acopla el backend a C++ y requiere una integración más compleja.


## Consecuencias

### Positivas (+)
- **Reutilización del árbol decisional:** Se aprovecha la lógica ya desarrollada en C++ sin convertirla en un servicio independiente.
- **Integración simplificada:** El frontend ejecuta directamente el árbol decisional mediante JavaScript, sin necesidad de mantener un servicio adicional.
- **Separación de responsabilidades:** El árbol decisional se concentra en el análisis de los datos y el backend en su gestión.

### Negativas / Desafíos (-)
- **Curva de aprendizaje:** El equipo debe aprender a utilizar Emscripten, una herramienta nueva para el proyecto.
- **Adaptación del código:** Es necesario adaptar parte del código C++ para que pueda compilarse a WebAssembly.
