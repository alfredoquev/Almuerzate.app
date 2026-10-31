# ADR-0002: Usar PHP como conexión entre el frontend y el backend

Autores:
- @amadodev07
- @juanfpg1
 
Fecha: 2026-10-30

## Estado

Aceptado

## Contexto

Vamos a integrar el front-end con el backend mediante PHP, que se ejecuta en
el servidor, invoca el ejecutable en C++ (árbol decisional) y envía al
cliente únicamente los datos requeridos para su visualización.

Las fuerzas y restricciones clave que impulsan esta decisión son:
1. **Reutilización de la lógica existente:** El árbol decisional ya está implementado en C++ y se busca reutilizarlo sin reescribirlo.
2. **Simplicidad de la integración:** La conexión entre el front-end y el backend debe ser sencilla y proporcional al alcance del proyecto.
3. **Separación de responsabilidades:** El backend se encarga del análisis y la gestión de los datos, y el front-end de su visualización.

## Decisión

Vamos a integrar el front-end con el backend enviando desde el servidor, mediante PHP, únicamente los datos requeridos por el cliente para su visualización obtenidos del arbol desicional en c++.

## Alternativas consideradas

- **Emscripten:** Se descartó porque requiere ejecutar el código C++ dentro del cliente, y el backend se ejecutará en el servidor.
- **Bibliotecas en C++:** Se descartaron porque acoplan la conexión con el front-end a C++, requieren una integración más compleja que PHP.

## Consecuencias

### Positivas (+)
- **Mejor manejo de la conexión cliente-servidor:** PHP provee un mecanismo definido para enviar la información del servidor al cliente.
- **Sistema más común y mejor documentado:** PHP es una tecnología ampliamente utilizada y cuenta con abundante documentación.
- **Reutilización directa del C++:** el árbol decisional se usa tal como
  está, sin reescribirlo ni exponerlo como servicio.

### Negativas / Desafíos (-)
- **Curva de aprendizaje:** El equipo debe aprender PHP, una tecnología nueva para el proyecto.
- **Costo por ejecución:** cada solicitud lanza un proceso del backend,
  lo que puede penalizar el rendimiento con alto flujo de datos.
- **Validación de argumentos:** los parámetros que PHP pasa al ejecutable
  deben validarse para evitar inyección de comandos.
