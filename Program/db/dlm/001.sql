-- Migración 001: lleva la base de datos al modelo de docs/requirements/02_Interaction_and_Data_Design.md (sección 2.2)
-- Es idempotente para tablas nuevas. Sobre `comedor` (ya existente) solo agrega restricciones.
-- Si hay datos que las violan (nombres repetidos o capacidad <= 0), la migración falla y se revierte completa.
BEGIN;

-- Días hábiles (valores de DiaSemana en D1; el orden del enum es el orden Lunes a Viernes)
DO $$ BEGIN
  CREATE TYPE dia_semana AS ENUM ('Lunes','Martes','Miercoles','Jueves','Viernes');
EXCEPTION WHEN duplicate_object THEN NULL; END $$;

CREATE TABLE IF NOT EXISTS usuario (
  id_usuario       integer GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  nombre           text    NOT NULL,
  apellido         text    NOT NULL,
  username         text    NOT NULL UNIQUE,
  passkey          text    NOT NULL,            -- almacenar solo un hash (OQ-4); nunca se devuelve en consultas
  es_administrador boolean NOT NULL DEFAULT false
);

CREATE TABLE IF NOT EXISTS comedor (
  id_comedor       integer GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  nombre           text    NOT NULL,
  capacidad_maxima integer NOT NULL
);

-- Si `comedor` ya existía, se le agregan las restricciones del diseño
DO $$ BEGIN
  IF NOT EXISTS (SELECT 1 FROM pg_constraint WHERE conname = 'comedor_nombre_uk') THEN
    ALTER TABLE comedor ADD CONSTRAINT comedor_nombre_uk UNIQUE (nombre);
  END IF;
  IF NOT EXISTS (SELECT 1 FROM pg_constraint WHERE conname = 'comedor_capacidad_ck') THEN
    ALTER TABLE comedor ADD CONSTRAINT comedor_capacidad_ck CHECK (capacidad_maxima > 0);
  END IF;
END $$;

CREATE TABLE IF NOT EXISTS estudiante_beneficiario (
  id_estudiante integer GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  id_usuario    integer NOT NULL UNIQUE REFERENCES usuario(id_usuario)   -- a lo sumo un estudiante por usuario (OQ-9)
);

CREATE TABLE IF NOT EXISTS periodo_asignacion (
  id_periodo   integer GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  fecha_inicio date NOT NULL,
  fecha_fin    date NOT NULL,
  CONSTRAINT periodo_fechas_ck CHECK (fecha_inicio <= fecha_fin),
  -- Los periodos no se solapan (extremos incluidos). El índice GiST que crea esto sirve a buscarPeriodoVigente.
  CONSTRAINT periodo_sin_solape EXCLUDE USING gist (daterange(fecha_inicio, fecha_fin, '[]') WITH &&)
);

CREATE TABLE IF NOT EXISTS asignacion (
  id_asignacion integer GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  id_estudiante integer    NOT NULL REFERENCES estudiante_beneficiario(id_estudiante),
  id_comedor    integer    NOT NULL REFERENCES comedor(id_comedor),
  id_periodo    integer    NOT NULL REFERENCES periodo_asignacion(id_periodo),
  dia_semana    dia_semana NOT NULL,
  -- Un día por estudiante y periodo. Su índice sirve a buscarComedoresAsignados.
  CONSTRAINT asignacion_dia_uk UNIQUE (id_estudiante, id_periodo, dia_semana)
);
-- Búsquedas por comedor (conteo de aforo de REQ-01, joins de nombre) y soporte de la FK
CREATE INDEX IF NOT EXISTS asignacion_comedor_idx ON asignacion (id_comedor);
CREATE INDEX IF NOT EXISTS asignacion_periodo_idx ON asignacion (id_periodo);

-- Invariante de D1: un estudiante tiene 0 o 5 asignaciones por periodo. Se verifica al confirmar la transacción.
CREATE OR REPLACE FUNCTION verificar_cinco_dias() RETURNS trigger AS $$
DECLARE est integer; per integer; n integer;
BEGIN
  IF TG_OP = 'DELETE' THEN est := OLD.id_estudiante; per := OLD.id_periodo;
  ELSE est := NEW.id_estudiante; per := NEW.id_periodo; END IF;
  SELECT count(*) INTO n FROM asignacion WHERE id_estudiante = est AND id_periodo = per;
  IF n NOT IN (0, 5) THEN
    RAISE EXCEPTION 'El estudiante % debe tener 0 o 5 asignaciones en el periodo % (tiene %)', est, per, n;
  END IF;
  RETURN NULL;
END $$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS asignacion_cinco_dias ON asignacion;
CREATE CONSTRAINT TRIGGER asignacion_cinco_dias
  AFTER INSERT OR UPDATE OR DELETE ON asignacion
  DEFERRABLE INITIALLY DEFERRED
  FOR EACH ROW EXECUTE FUNCTION verificar_cinco_dias();

-- guardarComedoresAsignados (REQ-01): guarda los 5 días de un estudiante de una vez o ninguno.
-- p_comedores: 5 id_comedor en orden Lunes..Viernes. NO verifica el aforo (OQ-6 sin definir).
CREATE OR REPLACE FUNCTION guardar_comedores_asignados(p_id_estudiante integer, p_id_periodo integer, p_comedores integer[])
RETURNS void AS $$
BEGIN
  IF p_comedores IS NULL OR cardinality(p_comedores) <> 5 THEN
    RAISE EXCEPTION 'Se requieren exactamente 5 comedores (Lunes a Viernes)';
  END IF;
  INSERT INTO asignacion (id_estudiante, id_comedor, id_periodo, dia_semana)
  SELECT p_id_estudiante, c.id_comedor, p_id_periodo, (enum_range(NULL::dia_semana))[c.ord]
  FROM unnest(p_comedores) WITH ORDINALITY AS c(id_comedor, ord);
END $$ LANGUAGE plpgsql;

COMMIT;
