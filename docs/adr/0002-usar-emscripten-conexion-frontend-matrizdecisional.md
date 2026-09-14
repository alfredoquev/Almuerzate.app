# ADR-0002: Empscripten como integracion y conexion de frontend con backend
 
 
Autores:
 - @amadodev07, @juanfpg1
Fecha: 2026-09-12
 
## Estado

Aceptado
 
## Contexto
 
El árbol decisional estára desarrollado en C++ y debe integrarse con un frontend web en Tailwind. El proyecto es demostrativo y requiere una integración simple preparada para alto flujo de datos, sin embargo dejando ignorando algunas complejidades tecnicas como la seguridad.
 
## Decisión
 
Vamos a utilizar Emscripten para compilar el arbol C++ a WebAssembly y ejecutarla desde el frontend mediante JavaScript. Esto nos permitirá a posterior, reutilizar la lógica existente en C++ sin convertirla en un servicio independiente. Por otro lado, la comunicación necesaria con la base de datos se realizará mediante una implementacion en el backend con un modulo en principio sencillo que funcione como intermediario, obteniendo y enviando los datos necesarios hacia el frontend. Así el arbol se concentra puramente en el analisis dados los datos y el backend en cuestión, la gestión de datos manteniendo una integración sencilla para el alcance del proyecto.
 
## Alternativas consideradas
 
APIs C++: descartadas por requerir un servicio adicional y mayor complejidad de la necesaria para nuestra implementacion.
Ejecutable C++: descartado por ser menos práctico para en aplicaciones web.
Biblioteca C++ en backend: descartada porque acopla el backend a C++ y requiere mayor integración.
 
## Consecuencias
 
Se reutiliza el arbol existente y se simplifica su integración con el frontend. Como costo, el equipo debe aprender un sistema nuevo Emscripten y adaptar parte del código C++ para WebAssembly.
