# ADR-0004: Usar C++ para el backend y el árbol decisional

Autores:
- @amadodev07
- @juanfpg1
- @xedroc422
- @alfredoquev
- @julondono07

Fecha: 2026-10-30

## Estado

Aceptado


## Contexto

La asignación de los cupos en el comedor es parte fundamental del programa. El sistema puede tener una complejidad considerable y debe conectarse tanto con la base de datos como con el front-end.

Las fuerzas y restricciones clave que impulsan esta decisión son:
1. **Manejo de datos:** Se requiere un lenguaje capaz de manejar grandes cantidades de datos por medio de distintas estructuras de datos.
2. **Conectividad:** El lenguaje debe poder conectarse con la base de datos y con el front-end.
3. **Conocimiento del equipo:** Dada la complejidad del sistema, es pertinente utilizar un lenguaje que el equipo ya conozca.


## Decisión

Vamos a utilizar C++ como lenguaje del backend, incluido el árbol decisional, que se ejecutará en el servidor.


## Alternativas consideradas

- **Java:** Se evaluó porque también permite un buen manejo de estructuras de datos y una buena integración con el front-end, gracias a las librerías dedicadas a la creación de interfaces de usuario. Se descartó porque el equipo no está muy familiarizado con el lenguaje ni con su sintaxis.


## Consecuencias

### Positivas (+)
- **Manejo de datos y estructuras:** C++ permite una mejor implementación de estructuras de datos y el manejo de grandes cantidades de datos, como resultado del control de memoria que ofrece.
- **Afinidad del equipo:** El equipo de trabajo conoce el lenguaje, lo que facilita el desarrollo de la aplicación.

### Negativas / Desafíos (-)
- **Complejidad en las estructuras de datos:** C++ tiene cierta complejidad en el manejo de estructuras de datos, por lo que es necesario implementarlas adecuadamente.
- **Conexión con el front-end:** Es necesario implementar una correcta conexión con el front-end mediante PHP, de acuerdo con lo establecido en el ADR-0002.
 
