// db/db.server.ts
import { drizzle } from 'drizzle-orm/postgres-js';
import postgres from 'postgres';
import { DATABASE_URL } from '$env/static/private';
import { dev } from '$app/environment';
import * as schema from './schema'

// const client = dev ? postgres(DATABASE_URL) : postgres(DATABASE_URL, { ssl: 'require' });
const client =  postgres(DATABASE_URL) 
export const db = drizzle(client, { schema });

export default db