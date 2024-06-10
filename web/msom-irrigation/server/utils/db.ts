import pg from 'pg';

const postgresClient = new pg.Client({
  user: process.env.PG_USERNAME,
  host: process.env.PG_HOST,
  database: process.env.PG_DATABASE,
  password: process.env.PG_PASSWORD,
  port: Number.parseInt(process.env.PG_PORT || '5432'),
  });
await postgresClient.connect();

