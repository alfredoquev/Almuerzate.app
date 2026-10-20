# ADR-0005: Uso de clase genérica para devolver resultados de queries
 
<!--
Nombra el archivo con el número consecutivo y un slug corto, por ejemplo:
docs/adr/0001-usar-postgresql-como-base-de-datos-principal.md
Los ADR se numeran en orden y NUNCA se editan después de aceptados para
cambiar la decisión en sí (sí puedes corregir errores de redacción). Si la
decisión cambia, escribes un ADR nuevo y marcas este como reemplazado
(ver "Estado"). Así el repositorio queda como una bitácora histórica de
por qué el sistema es como es.
-->

Se decidió usar una clase con dos tipos genéricos para encapsular la información devuelta por una query y sus posibles códigos de error.
 
Autores:
 - @amadodev07, @juanfpg1, @xedroc422, @alfredoquev y @julondono07
Fecha: 2026-09-12
 
## Estado
 
<!--
Uno de: Propuesto | Aceptado | Rechazado | Reemplazado por ADR-000Y | Obsoleto
Un ADR "Propuesto" está en discusión. Una vez el equipo decide, pasa a
"Aceptado" (o "Rechazado" si se descarta la propuesta) y ya no se
modifica su contenido de fondo.
-->
Aceptado
 
## Contexto
 
<!--
¿Qué problema técnico, restricción o fuerza nos obliga a tomar esta
decisión ahora? Describe la situación de forma neutral y objetiva —
todavía no es el lugar para argumentar a favor de una opción.
Ejemplos de fuerzas en juego: requisitos no funcionales (rendimiento,
seguridad, escalabilidad), restricciones de equipo o de tiempo,
deuda técnica existente, compatibilidad con sistemas ya construidos.
-->
Al obtener información requerida para una operación en la aplicación a través de una query, el patrón de recibir una respuesta con dicha información o un código de error es un patrón que se puede repetir a lo largo del desarrollo de la aplicación. Además, la firma de función de los métodos que devuelvan el resultado de una query podrán devolver solo un tipo (e.g. Resultado o Error) pero no dos al tiempo, por lo cual al tener una clase que encapsule tanto datos de queries como tipos de errores se maneja esta limitación.
 
## Decisión
 
<!--
Qué vamos a hacer, en una o dos frases claras y en tiempo presente
("Vamos a usar X para Y"). Esta es la sección más corta del documento:
un ADR no es un RFC, no necesita convencer a nadie aquí — la
justificación ya quedó en "Contexto" y las alternativas descartadas
van abajo.
-->
Se usará una clase genérica (Resultado<U,V>) para encapsular la información de una query o su código de error, en la cual los tipos corresponderán a la información devuelta y el tipo de error devuelto.

## Alternativas consideradas
 
<!--
Qué otras opciones se evaluaron y por qué se descartaron. No hace
falta un análisis exhaustivo, basta con dejar constancia de que se
consideraron y el motivo del descarte (costo, madurez, curva de
aprendizaje, no cumple un requisito, etc.).
-->
Manejar el retorno de tipos de error directamente en las funciones que requieran información de una query, y que las funciones que realizen una query directamente devuelvan NULL al no poder consultar información en una base de datos
 
## Consecuencias
 
<!--
¿Qué se vuelve más fácil o más difícil después de esta decisión?
Incluye efectos positivos y negativos por igual — un ADR honesto
también documenta el costo que se está aceptando (deuda técnica
introducida, dependencia nueva, curva de aprendizaje del equipo).
-->
Se hace necesario hacer validación adicional según el tipo de respuesta que se espera (e.g Al buscar solicitudes, la información esperada es la de una solicitud, y a su vez este tipo de query tendrá sus tipos de error que pueden ser distintos a los de otras queries).
