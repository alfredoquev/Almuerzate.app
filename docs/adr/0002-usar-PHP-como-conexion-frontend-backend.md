# ADR-0002: Usar PHP como conexión entre el frontend y el backend

Autores:
- @amadodev07
- @juanfpg1
 
Fecha: 2026-10-30

## Estado

Propuesto

## Contexto

El backend y el árbol decisional están desarrollados en C++ y se ejecutan en el servidor. El frontend es una página web que debe recibir la información requerida para mostrarla a cada estudiante. El proyecto es demostrativo y requiere una integración simple, dejando de lado deliberadamente algunas complejidades técnicas.

Las fuerzas y restricciones clave que impulsan esta decisión son:
1. **Reutilización de la lógica existente:** El árbol decisional ya está implementado en C++ y se busca reutilizarlo sin reescribirlo.
2. **Simplicidad de la integración:** La conexión entre el frontend y el backend debe ser sencilla y proporcional al alcance del proyecto.
3. **Separación de responsabilidades:** El backend se encarga del análisis y la gestión de los datos, y el frontend de su visualización.

## Decisión

Vamos a ejecutar el backend en C++ en el servidor y a utilizar PHP para enviar al cliente la información requerida, la cual el frontend presenta en una página web.

## Alternativas consideradas

- **Emscripten:** Se descartó porque requiere ejecutar el código C++ dentro del cliente, y el backend debe ejecutarse en el servidor.
- **Bibliotecas en C++:** Se descartaron porque acoplan la conexión con el frontend a C++, requieren una integración más compleja que PHP.

## Consecuencias

### Positivas (+)
- **Mejor manejo de la conexión cliente-servidor:** PHP provee un mecanismo definido para enviar la información del servidor al cliente.
- **Sistema más común y mejor documentado:** PHP es una tecnología ampliamente utilizada y cuenta con abundante documentación.

### Negativas / Desafíos (-)
- **Curva de aprendizaje:** El equipo debe aprender PHP, una tecnología nueva para el proyecto.
- **Vulnerabilidades de seguridad:** La conexión con PHP puede estar sujeta a vulnerabilidades de seguridad.
