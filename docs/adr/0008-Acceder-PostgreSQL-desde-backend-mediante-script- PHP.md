# ADR-0008: Acceder a PostgreSQL desde el backend mediante un script PHP

Autores:
 - @julondono07

Fecha: 2026-11-22

## Estado

Propuesto

## Contexto

El ADR-0001 define PostgreSQL como base de datos principal, y el ADR-0004
ubica el backend en C++ ejecutado en el servidor. El backend necesita
obtener datos de la base de datos para el árbol decisional. El equipo
requiere un mecanismo de acceso a la base de datos que sea sencillo de
aprender y de entender.

## Decisión

Vamos a ejecutar desde el backend en C++ el script que realiza las consultas a PostgreSQL y devuelve el resultado en JSON.

## Alternativas consideradas

- **Conexión directa desde C++ con `libpqxx`:** Se descartó porque su
  implementación resultaba más compleja en términos de aprendizaje y
  entendimiento.

## Consecuencias

### Positivas (+)
- El acceso a la base de datos es más sencillo de aprender y de entender
  que una conexión directa con `libpqxx`.

### Negativas / Desafíos (-)
- Cada consulta lanza un proceso `php` desde C++.
- El flujo pasa por PHP dos veces (PHP → C++ → PHP, ver ADR-0007).
- El backend depende de que `php` esté disponible en el servidor.
- Se deja de aprovechar la compatibilidad con `libpqxx` citada en el ADR-0001.
