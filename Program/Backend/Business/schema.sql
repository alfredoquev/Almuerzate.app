-- Esquema SUPUESTO para probar RepositorioAsignacionesPostgres.
-- Reemplazar por el esquema real del proyecto.

CREATE TABLE periodo_asignacion (
    fecha_inicio DATE NOT NULL PRIMARY KEY,
    fecha_fin    DATE NOT NULL,
    CHECK (fecha_fin >= fecha_inicio)
);

CREATE TABLE asignacion (
    id_estudiante  TEXT NOT NULL,
    dia            INT  NOT NULL CHECK (dia BETWEEN 1 AND 5),  -- 1 = lunes ... 5 = viernes
    nombre_comedor TEXT NOT NULL,
    periodo_inicio DATE NOT NULL REFERENCES periodo_asignacion (fecha_inicio),
    PRIMARY KEY (id_estudiante, periodo_inicio, dia)
);

-- Datos de prueba (el periodo debe contener la fecha actual para ser "vigente")
INSERT INTO periodo_asignacion (fecha_inicio, fecha_fin)
VALUES (DATE '2026-08-01', DATE '2026-12-15');

INSERT INTO asignacion (id_estudiante, dia, nombre_comedor, periodo_inicio) VALUES
    ('estudiante1', 1, 'Comedor Central', DATE '2026-08-01'),
    ('estudiante1', 2, 'Comedor Central', DATE '2026-08-01'),
    ('estudiante1', 3, 'Comedor Norte',   DATE '2026-08-01'),
    ('estudiante1', 4, 'Comedor Norte',   DATE '2026-08-01'),
    ('estudiante1', 5, 'Comedor Central', DATE '2026-08-01');
