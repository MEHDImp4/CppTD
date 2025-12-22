import initSqlJs from 'sql.js';
import { createRequire } from 'module';
import fs from 'fs';
import path from 'path';
import http from 'http';

const require = createRequire(import.meta.url);
const sqlJsPkgDir = path.dirname(require.resolve('sql.js/package.json'));

const dataDir = path.resolve(path.join('..', 'data'));
const dbPath = path.join(dataDir, 'weather.db');

async function loadDb() {
  const SQL = await initSqlJs({ locateFile: file => path.join(sqlJsPkgDir, 'dist', file) });
  if (!fs.existsSync(dbPath)) {
    // Create empty DB
    const db = new SQL.Database();
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
    const binary = db.export();
    fs.mkdirSync(dataDir, { recursive: true });
    fs.writeFileSync(dbPath, Buffer.from(binary));
    db.close();
  }
  const filebuffer = fs.readFileSync(dbPath);
  return new SQL.Database(filebuffer);
}

function dbToJsonRows(db, sql, params = []) {
  const stmt = db.prepare(sql);
  const rows = [];
  while (stmt.step()) {
    const row = stmt.getAsObject();
    rows.push(row);
  }
  stmt.free();
  return rows;
}

async function start() {
  const db = await loadDb();

  const server = http.createServer(async (req, res) => {
    const url = new URL(req.url, `http://${req.headers.host}`);
    const pathParts = url.pathname.replace('/weather-api', '').split('/').filter(Boolean);

    // CORS
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, DELETE, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

    if (req.method === 'OPTIONS') {
      res.writeHead(200);
      res.end();
      return;
    }

    try {
      // GET /measurements
      if (url.pathname === '/weather-api/measurements' && req.method === 'GET') {
        const rows = dbToJsonRows(db, "SELECT id, temperature, humidity, wind_speed, wind_direction, date_time, created_at FROM measurements ORDER BY date_time DESC");
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify(rows));
        return;
      }

      // POST /measurements
      if (url.pathname === '/weather-api/measurements' && req.method === 'POST') {
        let body = '';
        for await (const chunk of req) body += chunk;
        const obj = JSON.parse(body);
        const id = cryptoRandomUUID();
        const insert = db.prepare("INSERT INTO measurements (id, temperature, humidity, wind_speed, wind_direction, date_time) VALUES (?, ?, ?, ?, ?, ?)");
        insert.run([id, obj.temperature, obj.humidity, obj.wind_speed, obj.wind_direction, obj.date_time]);
        insert.free();
        // save DB
        fs.writeFileSync(dbPath, Buffer.from(db.export()));
        res.writeHead(201, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ id, ...obj }));
        return;
      }

      // DELETE /measurements/:id
      if (url.pathname.startsWith('/weather-api/measurements/') && req.method === 'DELETE') {
        const id = pathParts[1];
        const del = db.prepare('DELETE FROM measurements WHERE id = ?');
        del.run([id]);
        del.free();
        fs.writeFileSync(dbPath, Buffer.from(db.export()));
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify({ success: true }));
        return;
      }

      // GET /analysis
      if (url.pathname === '/weather-api/analysis' && req.method === 'GET') {
        const rows = dbToJsonRows(db, "SELECT temperature, humidity, wind_speed, date_time FROM measurements ORDER BY date_time DESC");
        if (!rows || rows.length === 0) {
          res.setHeader('Content-Type', 'application/json');
          res.end(JSON.stringify({ avgTemp:0, minTemp:0, maxTemp:0, avgHumidity:0, avgWindSpeed:0, trend: 'données insuffisantes'}));
          return;
        }
        const temperatures = rows.map(r => r.temperature);
        const avgTemp = temperatures.reduce((a,b)=>a+b,0)/temperatures.length;
        const minTemp = Math.min(...temperatures);
        const maxTemp = Math.max(...temperatures);
        const humidities = rows.map(r=>r.humidity);
        const avgHumidity = humidities.reduce((a,b)=>a+b,0)/humidities.length;
        const windSpeeds = rows.map(r=>r.wind_speed);
        const avgWindSpeed = windSpeeds.reduce((a,b)=>a+b,0)/windSpeeds.length;
        let trend = 'données insuffisantes';
        if (rows.length >= 3) {
          const temp1 = rows[2].temperature;
          const temp2 = rows[1].temperature;
          const temp3 = rows[0].temperature;
          const change1 = temp2 - temp1;
          const change2 = temp3 - temp2;
          const epsilon = 0.5;
          if (change1 > epsilon && change2 > epsilon) trend = 'hausse';
          else if (change1 < -epsilon && change2 < -epsilon) trend = 'baisse';
          else trend = 'stable';
        }
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify({ avgTemp: parseFloat(avgTemp.toFixed(2)), minTemp: parseFloat(minTemp.toFixed(2)), maxTemp: parseFloat(maxTemp.toFixed(2)), avgHumidity: parseFloat(avgHumidity.toFixed(2)), avgWindSpeed: parseFloat(avgWindSpeed.toFixed(2)), trend }));
        return;
      }

      // Not found
      res.writeHead(404, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: 'Route non trouvée' }));
    } catch (err) {
      res.writeHead(500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ error: err.message ?? String(err) }));
    }
  });

  server.listen(8000, () => console.log('Mock API listening on http://localhost:8000'));
}

function cryptoRandomUUID() {
  // Node 14+ has crypto.randomUUID, fallback if absent
  try {
    return (globalThis.crypto && globalThis.crypto.randomUUID && globalThis.crypto.randomUUID()) || require('crypto').randomUUID();
  } catch (e) {
    // simple fallback
    return 'id-' + Math.random().toString(36).slice(2, 10);
  }
}

start();
