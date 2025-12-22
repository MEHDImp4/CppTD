import { readFileSync } from 'fs';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

function migrateMeasurements() {
  try {
    const dataPath = join(__dirname, '..', 'data', 'measurements.txt');
    const content = readFileSync(dataPath, 'utf-8');
    const lines = content.trim().split('\n');

    console.log(`📁 Lecture de ${lines.length} ligne(s) depuis ${dataPath}`);

    const measurements = [];

    for (const line of lines) {
      if (!line.trim() || line.startsWith('#')) continue;

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

    console.log(`✅ ${measurements.length} mesure(s) prête(s) pour la migration`);

    if (measurements.length === 0) {
      console.log('Aucune donnée à migrer.');
      return;
    }

    // Try to use native better-sqlite3 if available; otherwise suggest Deno alternative
    let Database;
    try {
      Database = require('better-sqlite3');
    } catch (e) {
      console.error('⚠️  Module "better-sqlite3" introuvable ou installation impossible.');
      console.error('   Options:');
      console.error('   1) Installer Visual Studio C++ (Desktop development with C++) puis réexécuter `npm install`');
      console.error('   2) Utiliser le script Deno (recommandé) :');
      console.error('      deno run --allow-read --allow-write ../web/migrate-data-deno.ts');
      return;
    }

    const dbPath = join(__dirname, '..', 'data', 'weather.db');
    const db = new Database(dbPath);

    db.exec(`
      CREATE TABLE IF NOT EXISTS measurements (
        id TEXT PRIMARY KEY,
        temperature REAL NOT NULL,
        humidity REAL NOT NULL,
        wind_speed REAL NOT NULL,
        wind_direction TEXT NOT NULL,
        date_time TEXT NOT NULL,
        created_at TEXT DEFAULT (datetime('now'))
      );
    `);

    const insert = db.prepare("INSERT INTO measurements (id, temperature, humidity, wind_speed, wind_direction, date_time) VALUES (?, ?, ?, ?, ?, ?)");
    const insertMany = db.transaction((rows) => {
      for (const r of rows) {
        insert.run([require('crypto').randomUUID(), r.temperature, r.humidity, r.wind_speed, r.wind_direction, r.date_time]);
      }
    });

    insertMany(measurements);

    console.log(`🎉 Migration réussie! ${measurements.length} mesure(s) ajoutée(s) à SQLite (${dbPath})`);
    db.close();
  } catch (error) {
    console.error('❌ Erreur:', error.message);
  }
}

migrateMeasurements();
