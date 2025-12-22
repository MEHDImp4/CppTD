import initSqlJs from 'sql.js';
import fs from 'fs';
import path from 'path';
import { randomUUID } from 'crypto';
import { fileURLToPath } from 'url';
import { createRequire } from 'module';

const require = createRequire(import.meta.url);

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

async function migrateMeasurements() {
  try {
    const dataPath = path.join(__dirname, '..', 'data', 'measurements.txt');
    if (!fs.existsSync(dataPath)) {
      console.error('❌ Fichier de données introuvable:', dataPath);
      process.exit(1);
    }

    const content = fs.readFileSync(dataPath, 'utf8');
    const lines = content.split('\n').map(l => l.trim()).filter(l => l && !l.startsWith('#'));
    console.log(`📁 Lecture de ${lines.length} ligne(s) depuis ${dataPath}`);

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
      return;
    }

    const dbPath = path.join(__dirname, '..', 'data', 'weather.db');
    await fs.promises.mkdir(path.dirname(dbPath), { recursive: true });

    // Ensure sql-wasm.wasm is loaded from the installed sql.js package
    const sqlJsPkgDir = path.dirname(require.resolve('sql.js/package.json'));
    const SQL = await initSqlJs({ locateFile: file => path.join(sqlJsPkgDir, 'dist', file) });

    let db;
    if (fs.existsSync(dbPath)) {
      const filebuffer = fs.readFileSync(dbPath);
      db = new SQL.Database(filebuffer);
    } else {
      db = new SQL.Database();
    }

    db.run(`
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

    db.run('BEGIN TRANSACTION');
    for (const m of measurements) {
      insert.run([randomUUID(), m.temperature, m.humidity, m.wind_speed, m.wind_direction, m.date_time]);
    }
    db.run('COMMIT');
    insert.free();

    const binaryArray = db.export();
    const buffer = Buffer.from(binaryArray);
    fs.writeFileSync(dbPath, buffer);
    db.close();

    console.log(`🎉 Migration réussie! ${measurements.length} mesure(s) ajoutée(s) à SQLite (${dbPath})`);
  } catch (err) {
    console.error('❌ Erreur:', err.message ?? err);
    process.exit(1);
  }
}

migrateMeasurements();
