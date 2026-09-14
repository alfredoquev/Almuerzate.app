# ADR-0001: Usar PostgreSQL como base de datos principal

Autores:
- @alfredoquev

Fecha: 2026-09-12 

## Estado

Aceptado


## Contexto

El proyecto requiere almacenar y gestionar de forma persistente grandes volúmenes de datos estructurados (tales como información de usuarios, registros y la lógica de asignación de cupos de comedor). La arquitectura del sistema está compuesta por un front-end web desarrollado con HTML, CSS y Tailwind CSS, conectado a un núcleo de procesamiento y lógica de negocio implementado en C++. 

Las fuerzas y restricciones clave que impulsan esta decisión son:
1. **Rendimiento y Procesamiento:** La base de datos debe interactuar de forma eficiente con el backend en C++, soportando operaciones complejas y consultas masivas de datos sin convertirse en un cuello de botella.
2. **Integridad Transaccional:** Al tratarse de un sistema de asignación y gestión, es un requisito indispensable garantizar la consistencia estricta de los datos mediante un modelo relacional robusto (ACID).
3. **Ecosistema y Conectividad:** Se necesita un motor de base de datos maduro que cuente con soporte nativo o librerías estables y de alto rendimiento para su integración con lenguajes compilados como C++.


## Decisión

Vamos a utilizar PostgreSQL como el sistema de gestión de base de datos relacional principal de la aplicación.


## Alternativas consideradas

- **MySQL / MariaDB:** Se evaluó por su alta popularidad; sin embargo, se descartó frente a PostgreSQL debido a que este último ofrece capacidades avanzadas superiores para el manejo de consultas complejas, extensiones robustas y un soporte más estricto de los estándares SQL, lo cual beneficia la lógica intensiva operada desde C++.
- **SQLite:** Se consideró por su simplicidad al no requerir un servidor dedicado; se descartó porque no está optimizado para soportar la concurrencia de escrituras y lecturas concurrentes que la aplicación podría exigir en entornos de producción.
- **MongoDB (NoSQL):** Se valoró por su flexibilidad en los esquemas; se descartó debido a que la naturaleza de la información (registros institucionales, usuarios y asignaciones) demanda relaciones altamente estructuradas y transacciones atómicas rigurosas (ACID).


## Consecuencias

### Positivas (+)
- **Integridad y Seguridad de los Datos:** Garantía absoluta de consistencia transaccional (ACID) para las operaciones críticas del sistema (como la asignación de cupos).
- **Sinergia con C++:** Excelente compatibilidad y disponibilidad de librerías maduras y eficientes para conectar la base de datos directamente con el backend en C++ (`libpqxx`, etc.).
- **Preparado para Analítica y Paneles:** Facilita la extracción eficiente de datos agregados que posteriormente se reflejarán en las interfaces construidas con Tailwind CSS y librerías de gráficos en el front-end.

### Negativas / Desafíos (-)
- **Infraestructura y Despliegue:** Requiere aprovisionar y administrar un servidor de base de datos independiente en comparación con soluciones embebidas (como SQLite).
- **Curva de Aprendizaje:** Requiere un diseño de esquema relacional cuidadoso y una correcta indexación para evitar problemas de rendimiento a medida que el volumen de datos crezca.
 
