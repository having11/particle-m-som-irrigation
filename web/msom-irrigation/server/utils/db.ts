import pg from 'pg';
import { Valve } from '~/models/Valve';

const postgresClient = new pg.Client({
  user: process.env.PG_USERNAME,
  host: process.env.PG_HOST,
  database: process.env.PG_DATABASE,
  password: process.env.PG_PASSWORD,
  port: Number.parseInt(process.env.PG_PORT || '5432'),
  });
await postgresClient.connect();

export async function saveValveStates(states: {index: number, state: boolean}[]): Promise<void> {
  for await (const state of states) {
    const valveResult = await postgresClient.query<{id: number}>(`SELECT id FROM valve WHERE index = $1`,
      [state.index]);

    await postgresClient.query(`INSERT INTO valve_history(valve_id, active)
      VALUES ($1, $2)`, [valveResult.rows[0].id, state.state]);
  }
}

export async function saveSoilValues(values: {index: number, value: number}[]): Promise<void> {
  for await (const value of values) {
    const soilResult = await postgresClient.query<{id: number}>(`SELECT id FROM soil_sensor WHERE index = $1`,
      [value.index]);

    await postgresClient.query(`INSERT INTO soil_history(value, soil_sensor_id)
      VALUES ($1, $2)`, [value.value, soilResult.rows[0].id]);
  }
}

export async function getLatestValveStates(): Promise<Valve[]> {
  const result = await postgresClient.query<Valve>(`
    SELECT DISTINCT ON (t1.valve_id) t1.valve_id AS id, t4.index, t4.name, t1.active AS state, t1.timestamp, t3.value AS soil_value
      FROM public.valve_history t1
    LEFT JOIN soil_sensor t2
      ON t1.valve_id = t2.valve_id
    LEFT JOIN soil_history t3
      ON t2.id = t3.soil_sensor_id
    INNER JOIN valve t4
      ON t1.valve_id = t4.id
    ORDER BY t1.valve_id, t1.timestamp DESC
    `);

  return result.rows;
}

export async function getLatestSoilValues(): Promise<{index: number, value: number}[]> {
  const result = await postgresClient.query<{index: number, value: number}>(`
    SELECT DISTINCT ON (t2.index) t2.index, value, timestamp
      FROM soil_history t1
    INNER JOIN soil_sensor t2
      ON t1.soil_sensor_id = t2.id
    ORDER BY t2.index, timestamp DESC
    `);
    
  return result.rows;
}