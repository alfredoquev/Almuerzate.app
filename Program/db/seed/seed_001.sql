-- Almuérzate — INSERT de datos de prueba (requiere que las tablas ya existan)

-- Comedores
INSERT INTO comedor (nombre, capacidad_maxima)
SELECT v.nombre, v.capacidad
FROM (VALUES
    ('Yu Takeuchi',                120),
    ('Comedor de Ciencias',         80),
    ('Comedor Hemeroteca',          60),
    ('Comedor de Ciencias Humanas', 70),
    ('Comedor de Derecho',          50)
) AS v(nombre, capacidad)
WHERE NOT EXISTS (SELECT 1 FROM comedor c WHERE c.nombre = v.nombre);

-- Usuarios administradores
INSERT INTO usuario (nombre, apellido, username, passkey, es_administrador)
SELECT v.nombre, v.apellido, v.username, v.passkey, TRUE
FROM (VALUES
    ('Laura', 'Administradora', 'admin01', 'passkey_prueba_admin01'),
    ('Jorge', 'Supervisor',     'admin02', 'passkey_prueba_admin02')
) AS v(nombre, apellido, username, passkey)
WHERE NOT EXISTS (SELECT 1 FROM usuario u WHERE u.username = v.username);

-- Usuarios estudiantes (30)
INSERT INTO usuario (nombre, apellido, username, passkey, es_administrador)
SELECT
    (ARRAY['Camila','Andrés','Valentina','Santiago','Daniela','Mateo','Sofía','Juan',
           'Mariana','Felipe','Isabela','Sebastián','Laura','Nicolás','Paula'])[1 + (g - 1) % 15],
    (ARRAY['Gómez','Rodríguez','Martínez','López','García','Hernández','Pérez','Ramírez',
           'Torres','Díaz','Vargas','Castro'])[1 + (g * 5) % 12],
    'estudiante' || lpad(g::text, 2, '0'),
    'passkey_prueba_' || lpad(g::text, 2, '0'),
    FALSE
FROM generate_series(1, 30) AS g
WHERE NOT EXISTS (
    SELECT 1 FROM usuario u WHERE u.username = 'estudiante' || lpad(g::text, 2, '0')
);

-- Estudiantes beneficiarios
INSERT INTO estudiante_beneficiario (id_usuario)
SELECT u.id_usuario
FROM usuario u
WHERE u.username LIKE 'estudiante%'
  AND NOT EXISTS (SELECT 1 FROM estudiante_beneficiario e WHERE e.id_usuario = u.id_usuario);

-- Periodos de asignación (anterior y vigente, relativos a la fecha de ejecución)
INSERT INTO periodo_asignacion (fecha_inicio, fecha_fin)
SELECT v.ini, v.fin
FROM (VALUES
    (CURRENT_DATE - 270, CURRENT_DATE - 120),
    (CURRENT_DATE - 90,  CURRENT_DATE + 60)
) AS v(ini, fin)
WHERE NOT EXISTS (
    SELECT 1 FROM periodo_asignacion p
    WHERE daterange(p.fecha_inicio, p.fecha_fin, '[]') && daterange(v.ini, v.fin, '[]')
);

-- Asignaciones del periodo vigente: estudiante01..25 (5 días c/u); 26..30 sin asignaciones
WITH vigente AS (
    SELECT id_periodo FROM periodo_asignacion
    WHERE CURRENT_DATE BETWEEN fecha_inicio AND fecha_fin
),
est AS (
    SELECT e.id_estudiante,
           ROW_NUMBER() OVER (ORDER BY e.id_estudiante) AS k
    FROM estudiante_beneficiario e
    JOIN usuario u ON u.id_usuario = e.id_usuario
    WHERE u.username LIKE 'estudiante%'
),
dias AS (
    SELECT d.orden, d.dia::dia_semana AS dia
    FROM (VALUES (1,'Lunes'), (2,'Martes'), (3,'Miercoles'), (4,'Jueves'), (5,'Viernes'))
         AS d(orden, dia)
),
com AS (
    SELECT id_comedor, ROW_NUMBER() OVER (ORDER BY id_comedor) AS c
    FROM comedor
    WHERE nombre IN ('Yu Takeuchi','Comedor de Ciencias','Comedor Hemeroteca',
                     'Comedor de Ciencias Humanas','Comedor de Derecho')
)
INSERT INTO asignacion (id_estudiante, id_comedor, id_periodo, dia_semana)
SELECT est.id_estudiante, com.id_comedor, vigente.id_periodo, dias.dia
FROM est
CROSS JOIN dias
CROSS JOIN vigente
JOIN com ON com.c = CASE WHEN dias.orden % 2 = 1
                         THEN (est.k % 5) + 1
                         ELSE ((est.k + 2) % 5) + 1 END
WHERE est.k <= 25
  AND NOT EXISTS (
      SELECT 1 FROM asignacion a
      WHERE a.id_estudiante = est.id_estudiante
        AND a.id_periodo    = vigente.id_periodo
        AND a.dia_semana    = dias.dia
  );

-- Asignaciones del periodo anterior: estudiante01 y estudiante02
WITH anterior AS (
    SELECT id_periodo FROM periodo_asignacion
    WHERE fecha_fin < CURRENT_DATE - 90
    ORDER BY fecha_fin DESC LIMIT 1
),
est AS (
    SELECT e.id_estudiante,
           ROW_NUMBER() OVER (ORDER BY e.id_estudiante) AS k
    FROM estudiante_beneficiario e
    JOIN usuario u ON u.id_usuario = e.id_usuario
    WHERE u.username LIKE 'estudiante%'
),
dias AS (
    SELECT d.orden, d.dia::dia_semana AS dia
    FROM (VALUES (1,'Lunes'), (2,'Martes'), (3,'Miercoles'), (4,'Jueves'), (5,'Viernes'))
         AS d(orden, dia)
),
com AS (
    SELECT id_comedor, ROW_NUMBER() OVER (ORDER BY id_comedor) AS c
    FROM comedor
    WHERE nombre IN ('Yu Takeuchi','Comedor de Ciencias','Comedor Hemeroteca',
                     'Comedor de Ciencias Humanas','Comedor de Derecho')
)
INSERT INTO asignacion (id_estudiante, id_comedor, id_periodo, dia_semana)
SELECT est.id_estudiante, com.id_comedor, anterior.id_periodo, dias.dia
FROM est
CROSS JOIN dias
CROSS JOIN anterior
JOIN com ON com.c = ((est.k + dias.orden) % 5) + 1
WHERE est.k <= 2
  AND NOT EXISTS (
      SELECT 1 FROM asignacion a
      WHERE a.id_estudiante = est.id_estudiante
        AND a.id_periodo    = anterior.id_periodo
        AND a.dia_semana    = dias.dia
  );
