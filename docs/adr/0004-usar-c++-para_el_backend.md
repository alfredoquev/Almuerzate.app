# ADR-0004: Implementación de árbol de decisiones en C++
 
Se decidio usar el lenguaje de c++ para el manejo de backend
 
Autores:
 - @amadodev07, @juanfpg1, @xedroc422, @alfredoquev y @julondono07
Fecha: 2026-09-12
 
## Estado
 
Aceptado
 
## Contexto
 
La asignación de los cupos en el comedor es parte fundamental del programa, por lo que se busca manejar un lenguaje que sea capaz de manejar grandes cantidades de datos por medio de distintas estructuras de datos y que pueda ser conectado tanto con las bases de datos como con el frontend; además, dado que el sistema a implementar puede considerar una complejidad considerable, es pertinente hacer uso de un lenguaje conocido.
 
## Decisión
 
En base a lo anterior, se definió el uso de C++, ya que es un lenguaje conocido por su capacidad de manejo de grandes cantidades de datos junto con una mejor implementación de estructuras de datos como resultado del manejo de memoria que este permite. Además, es una lenguaje al cual el equipo de trabajo es afín lo que facilita el desarrollo de la aplicación.
 
## Alternativas consideradas
 
Se consideró la implementación de Java, el cual es un lenguaje que también permite un buen manejo de estructuras de datos y puede garantizar una buena implementación con el frontend debido a la existencia de librerías dedicadas a la creación de interfaces de usuario; sin embargo, dado que el equipo no esta muy familiarizado con este y su sintaxis se descartó esta opción.
 
## Consecuencias

Una vez considerada la decisión de C++, es importante hacer hincapié en la implementación adecuada de las estructuras de datos que se van a utilizar ya que C++ es un lenguaje que tiene cierta complejidad en el manejo de estas. Por otro lado, también es necesario implementar una correcta conexión con el frontend por medio de APIs. Además C++ es un lenguaje comúnmente utilizado en otras aplicaciones de navegador junto con cadenas de herramientas para compilación, debido a que permite la escritura de código de más bajo nivel e intensivo en rendimiento sin necesidad de interactuar directamente con WebAssembly.
 
