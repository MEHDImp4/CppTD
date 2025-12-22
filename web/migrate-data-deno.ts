import { DB } from 'https://deno.land/x/sqlite/mod.ts';
import * as path from 'https://deno.land/std@0.203.0/path/mod.ts';

const measurementsUrl = new URL('../data/measurements.txt', import.meta.url);
const dataDir = path.resolve(path.fromFileUrl(new URL('../data', import.meta.url)));
const dbPath = path.join(dataDir, 'weather.db');


try {
  const content = await Deno.readTextFile(measurementsUrl);
  const lines = content.split('\n').map(l => l.trim()).filter(l => l && !l.startsWith('#'));

  console.log(`📁 Lecture de ${lines.length} ligne(s) depuis ${measurementsUrl.pathname}`);

  const measurements = [];
  for (const line of lines) {
    const parts = line.split('|');
    if (parts.length !== 5) {
      console.warn(`⚠️  Ligne ignorée (format invalide): ${line}`);
      continue;
    }
    const [temperature, humidity, windSpeed, windDirection, dateTime] = parts;
    measurements.push({
      temperature: parseFloat(temperature),
      humidity: parseFloat(humidity),
      wind_speed: parseFloat(windSpeed),
      wind_direction: windDirection.trim(),
      date_time: new Date(dateTime.trim()).toISOString(),
    });
  }

  if (measurements.length === 0) {
    console.log('Aucune donnée à migrer.');
    Deno.exit(0);
  }

  // Ensure directory exists
  await Deno.mkdir(dataDir, { recursive: true });

  const db = new DB(dbPath);
  db.query(`
    CREATE TABLE IF NOT EXISTS measurements (
      id TEXT PRIMARY KEY,
      temperature REAL NOT NULL,
      humidity REAL NOT NULL,
      wind_speed REAL NOT NULL,
      wind_direction TEXT NOT NULL,
      date_time TEXT NOT NULL,
      created_at TEXT DEFAULT (datetime('now'))
    )
  `);

  const insert = db.prepare("INSERT INTO measurements (id, temperature, humidity, wind_speed, wind_direction, date_time) VALUES (?, ?, ?, ?, ?, ?)");

  db.transaction(() => {
    for (const m of measurements) {
      insert.run([crypto.randomUUID(), m.temperature, m.humidity, m.wind_speed, m.wind_direction, m.date_time]);
    }
  })();

  console.log(`🎉 Migration réussie! ${measurements.length} mesure(s) ajoutée(s) à SQLite (${dbPath})`);
  db.close();
} catch (err) {
  console.error('❌ Erreur:', err.message ?? err);
  Deno.exit(1);
}
