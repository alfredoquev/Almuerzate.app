# Almuérzate

**RFC: Sistema de asignación de comedores para la Universidad Nacional de Colombia**

**Autores:** @amadodev07, @juanfpg1, @xedroc422, @alfredoquev y @julondono07

---

## Contenido

1. [TL;DR](#1-tldr)
2. [Motivación](#2-motivación)
3. [Propuesta de implementación](#3-propuesta-de-implementación)
4. [Métricas](#4-métricas)
5. [Riesgos e inconvenientes](#5-riesgos-e-inconvenientes)
6. [Alternativas](#6-alternativas)
7. [Impacto potencial y dependencias](#7-impacto-potencial-y-dependencias)
8. [Preguntas sin resolver](#8-preguntas-sin-resolver)
9. [Conclusión](#9-conclusión)

---

## 1. TL;DR

Buscamos crear una aplicación que sirva como **sistema de asignación de comedores** en la Universidad Nacional de Colombia. El sistema priorizará seis criterios:

- el **aforo** de cada comedor,
- las **preferencias de comedor** de cada estudiante,
- las **discapacidades** del estudiante (en caso de haber),
- el **horario** del estudiante cada dia,
- las **preferencias alimentarias** de cada estudiante,
- y su **ubicación relativa** a cada comedor.

---

## 2. Motivación

En los comedores de la universidad hay un problema claro: algunos están saturados mientras que otros presentan un aforo mínimo.

Esto genera:

- filas interminables,
- estudiantes llegando tarde a sus clases,
- y en general, un gran descontento por parte de la comunidad estudiantil.

---

## 3. Propuesta de implementación

### Entrega 1

Se planea implementar una aplicación en línea en la que los estudiantes puedan **consultar el comedor que les ha sido asignado**. Esta asignación se realizará de manera inteligente, teniendo en cuenta determinada información personal y las características de cada estudiante.

### Entrega 2

En esta etapa se incorporará a la aplicación una **encuesta** que permitirá identificar de manera más precisa las necesidades y preferencias de los estudiantes. Con base en la información recopilada, se establecerá un **sistema de reasignación periódica** de los comedores. Además, se buscará mejorar el proceso de asignación de cupos para optimizar su distribución y aprovechamiento.

### Entrega final

Se añadirá un **sistema de estadísticas** que permitirá a los administradores consultar y analizar el comportamiento de los comedores, facilitando la toma de decisiones. Asimismo, los administradores podrán realizar **reasignaciones de emergencia**.

Finalmente, se implementará un sistema de **diferenciación en la asignación de cupos según el lapso de tiempo** al que el estudiante opte, selección que se realizará a partir de sus preferencias registradas en la encuesta.

---

## 4. Métricas

| Qué mediremos | Objetivo |
|---|---|
| **Estandarización del aforo** | Que ningún comedor esté saturado ni vacío. Se monitoreará la ocupación por comedor y franja horaria, y la diferencia entre comedores. |
| **Tiempo de fila** | Reducirlo a **menos de 40 minutos**, para que los estudiantes puedan acceder al servicio dentro de la hora de almuerzo establecida por la universidad. |
| **Desempeño del sistema** | Tiempo de respuesta de la asignación y de la consulta del comedor, y usuarios concurrentes en horas pico. |
| **Calidad de la asignación** | Porcentaje de estudiantes que piden cambio o reasignación, como indicador indirecto de asignaciones erróneas. |

---

## 5. Riesgos e inconvenientes

- **Asignación errónea de comedores:** si el árbol de decisión asigna mal, podrían aumentar los tiempos de desplazamiento y de fila en lugar de reducirse.
- **Limitaciones físicas de los comedores:** el aforo real de cada comedor es un tope que el sistema no puede superar, por lo que la distribución óptima depende de una capacidad limitada.
- **Fuga de información privada de los estudiantes:** el sistema maneja datos personales (ubicación, condiciones, preferencias) cuya exposición sería un riesgo importante para los beneficiarios.

---

## 6. Alternativas

### Asignación aleatoria de comedores
Todos los comedores recibirían un porcentaje parecido de estudiantes, pero sin considerar las características de cada uno. Esto podría perjudicar a quienes quedan lejos de su comedor o tienen condiciones particulares.

### Mantener el sistema actual
No ha garantizado un servicio funcional ni eficiente, y los problemas de saturación y filas persisten.

---

## 7. Impacto potencial y dependencias

### ¿A quién impacta?

- **Estudiantes beneficiarios:** ven una mejora en el servicio.
- **Trabajadores de los comedores:** enfrentan menos saturación en horas pico y, por ende, pueden ofrecer un mejor servicio.

### Dependencias

- El estado actual de los comedores (infraestructura y capacidad).
- Las entidades encargadas de los datos de los estudiantes relevantes para la asignación.

### Seguridad

El acceso a los datos de los estudiantes debe estar restringido y protegido, ya que es el punto más sensible del sistema.

---

## 8. Preguntas sin resolver

- ¿Qué ocurre cuando el aforo de un comedor se ve afectado de forma imprevista (cierre temporal, falla en el servicio)? ¿Cómo se redistribuye a los estudiantes asignados?
- ¿Con qué frecuencia deben actualizarse los datos de los estudiantes (ubicación, horario) y quién es responsable de mantenerlos al día?
- ¿Cómo se ponderan los componentes del árbol de decisión (ubicación, preferencia, aforo, condiciones del estudiante) cuando entran en conflicto?
- ¿Cómo se manejará a los estudiantes que no completen la encuesta de preferencias?
- ¿Qué convenios o permisos se necesitan con las entidades que administran los datos de los estudiantes?
- ¿Cómo se medirá de forma confiable el tiempo real de fila para validar la meta de 40 minutos?
- ¿Qué mecanismo tendrá el estudiante para apelar o solicitar el cambio de una asignación?

---

## 9. Conclusión

Se propone un sistema que distribuya mejor a los estudiantes beneficiarios entre los comedores, partiendo de la premisa de que esto **reducirá el tiempo de espera y mejorará el servicio**.

La asignación se realiza mediante un **árbol de decisión** que determina el mejor comedor para cada estudiante según distintos componentes, como el aforo, la ubicación y las preferencias.

Aunque existen riesgos asociados a la calidad de la asignación, las limitaciones físicas y la protección de datos, consideramos que los beneficios superan estos inconvenientes frente a las alternativas de la asignación aleatoria o de mantener el sistema actual.
