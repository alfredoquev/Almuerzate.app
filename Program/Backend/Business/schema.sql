-- =========================================================
-- almuerzate.app: esquema de asignación de comedores
-- =========================================================

CREATE TABLE estudiante (
    id_estudiante   INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    es_beneficiario BOOLEAN NOT NULL DEFAULT FALSE
);

CREATE TABLE comedor (
    id_comedor       INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    nombre           VARCHAR(100) NOT NULL UNIQUE,
    capacidad_maxima INTEGER NOT NULL CHECK (capacidad_maxima > 0)
);

CREATE TABLE periodo_asignacion (
    id_periodo   INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    fecha_inicio DATE    NOT NULL,
    fecha_fin    DATE    NOT NULL,
    es_vigente   BOOLEAN NOT NULL DEFAULT FALSE,
    CONSTRAINT ck_periodo_fechas CHECK (fecha_fin >= fecha_inicio)
);

-- Solo puede haber un periodo vigente a la vez
CREATE UNIQUE INDEX ux_periodo_unico_vigente
    ON periodo_asignacion (es_vigente)
    WHERE es_vigente;

CREATE TABLE asignacion (
    id_asignacion INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    id_estudiante INTEGER NOT NULL,
    id_comedor    INTEGER NOT NULL,
    id_periodo    INTEGER NOT NULL,
    dia_semana    VARCHAR(10) NOT NULL,

    CONSTRAINT fk_asignacion_estudiante
        FOREIGN KEY (id_estudiante) REFERENCES estudiante (id_estudiante),
    CONSTRAINT fk_asignacion_comedor
        FOREIGN KEY (id_comedor) REFERENCES comedor (id_comedor),
    CONSTRAINT fk_asignacion_periodo
        FOREIGN KEY (id_periodo) REFERENCES periodo_asignacion (id_periodo),

    -- Lunes a viernes (REQ-01)
    CONSTRAINT ck_asignacion_dia
        CHECK (dia_semana IN ('LUNES','MARTES','MIERCOLES','JUEVES','VIERNES')),

    -- Un solo comedor por estudiante, periodo y día
    CONSTRAINT uq_asignacion_estudiante_periodo_dia
        UNIQUE (id_estudiante, id_periodo, dia_semana)
);

-- Índices para las consultas más frecuentes (REQ-10 y conteo de aforo)
CREATE INDEX ix_asignacion_estudiante_periodo ON asignacion (id_estudiante, id_periodo);
CREATE INDEX ix_asignacion_comedor_periodo_dia ON asignacion (id_comedor, id_periodo, dia_semana);

-- 1. Comedores (capacidades pequeñas para que el aforo sea verificable)
INSERT INTO comedor (nombre, capacidad_maxima) VALUES
    ('Comedor Central',       3),
    ('Comedor Ciencias',      3),
    ('Comedor Ingeniería',    2),
    ('Comedor Humanidades',   2);

-- 2. Estudiantes: 7 beneficiarios y 1 que no lo es
INSERT INTO estudiante (es_beneficiario) VALUES
    (TRUE), (TRUE), (TRUE), (TRUE), (TRUE), (TRUE), (TRUE),
    (FALSE);

-- 3. Periodo de asignación vigente (semana en curso: 16 al 20 de noviembre de 2026)
INSERT INTO periodo_asignacion (fecha_inicio, fecha_fin, es_vigente) VALUES
    ('2026-11-16', '2026-11-20', TRUE);

-- 4. Asignaciones: una por beneficiario y día (7 estudiantes x 5 días = 35 filas)
--    Cada cadena es la semana de un estudiante, de lunes a viernes:
--    C = Central, S = Ciencias, I = Ingeniería, H = Humanidades
WITH plan (id_estudiante, semana) AS (
    VALUES (1, 'CCCCC'),
           (2, 'CSCSC'),
           (3, 'CSSII'),
           (4, 'SSSHH'),
           (5, 'SIICS'),
           (6, 'IIHHH'),
           (7, 'HHIIC')
),
dias (n, dia_semana) AS (
    VALUES (1, 'LUNES'), (2, 'MARTES'), (3, 'MIERCOLES'),
           (4, 'JUEVES'), (5, 'VIERNES')
),
codigos (codigo, nombre) AS (
    VALUES ('C', 'Comedor Central'),
           ('S', 'Comedor Ciencias'),
           ('I', 'Comedor Ingeniería'),
           ('H', 'Comedor Humanidades')
)
INSERT INTO asignacion (id_estudiante, id_comedor, id_periodo, dia_semana)
SELECT p.id_estudiante,
       c.id_comedor,
       per.id_periodo,
       d.dia_semana
FROM plan p
JOIN dias d        ON TRUE
JOIN codigos k     ON k.codigo = SUBSTRING(p.semana, d.n, 1)
JOIN comedor c     ON c.nombre = k.nombre
JOIN periodo_asignacion per ON per.es_vigente;