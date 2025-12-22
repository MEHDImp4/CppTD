import { DB } from 'https://deno.land/x/sqlite/mod.ts';

const corsHeaders = {
  'Access-Control-Allow-Origin': '*',
  'Access-Control-Allow-Methods': 'GET, POST, DELETE, OPTIONS',
  'Access-Control-Allow-Headers': 'Content-Type, Authorization, X-Client-Info, Apikey',
};

interface Measurement {
  id?: string;
  temperature: number;
  humidity: number;
  wind_speed: number;
  wind_direction: string;
  date_time: string;
}

// Ensure data directory exists and DB is initialized
try {
  await Deno.mkdir('./data', { recursive: true });
} catch (_e) {}

const dbPath = './data/weather.db';
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

db.query(`CREATE INDEX IF NOT EXISTS idx_measurements_date_time ON measurements(date_time DESC)`);

Deno.serve(async (req: Request) => {
  // Handle CORS preflight
  if (req.method === 'OPTIONS') {
    return new Response(null, {
      status: 200,
      headers: corsHeaders,
    });
  }

  try {
    const url = new URL(req.url);
    const path = url.pathname.replace('/weather-api', '');

    // GET /measurements - Récupérer toutes les mesures
    if (path === '/measurements' && req.method === 'GET') {
      const rows = [...db.query("SELECT id, temperature, humidity, wind_speed, wind_direction, date_time, created_at FROM measurements ORDER BY date_time DESC")];
      const data = rows.map((r: any[]) => ({
        id: r[0],
        temperature: r[1],
        humidity: r[2],
        wind_speed: r[3],
        wind_direction: r[4],
        date_time: r[5],
        created_at: r[6],
      }));

      return new Response(JSON.stringify(data), {
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      });
    }

    // POST /measurements - Ajouter une mesure
    if (path === '/measurements' && req.method === 'POST') {
      const body: Measurement = await req.json();
      const id = crypto.randomUUID();

      db.query(
        "INSERT INTO measurements (id, temperature, humidity, wind_speed, wind_direction, date_time) VALUES (?, ?, ?, ?, ?, ?)",
        [id, body.temperature, body.humidity, body.wind_speed, body.wind_direction, body.date_time]
      );

      const row = [...db.query("SELECT id, temperature, humidity, wind_speed, wind_direction, date_time, created_at FROM measurements WHERE id = ?", [id])][0];
      const data = {
        id: row[0],
        temperature: row[1],
        humidity: row[2],
        wind_speed: row[3],
        wind_direction: row[4],
        date_time: row[5],
        created_at: row[6],
      };

      return new Response(JSON.stringify(data), {
        status: 201,
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      });
    }

    // DELETE /measurements/:id - Supprimer une mesure
    if (path.startsWith('/measurements/') && req.method === 'DELETE') {
      const id = path.split('/')[2];

      db.query("DELETE FROM measurements WHERE id = ?", [id]);

      return new Response(JSON.stringify({ success: true }), {
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      });
    }

    // GET /analysis - Récupérer les analyses statistiques
    if (path === '/analysis' && req.method === 'GET') {
      const rows = [...db.query("SELECT temperature, humidity, wind_speed, date_time FROM measurements ORDER BY date_time DESC")];
      const data = rows.map((r: any[]) => ({
        temperature: r[0],
        humidity: r[1],
        wind_speed: r[2],
        date_time: r[3],
      }));

      if (!data || data.length === 0) {
        return new Response(
          JSON.stringify({
            avgTemp: 0,
            minTemp: 0,
            maxTemp: 0,
            avgHumidity: 0,
            avgWindSpeed: 0,
            trend: 'données insuffisantes',
          }),
          {
            headers: { ...corsHeaders, 'Content-Type': 'application/json' },
          }
        );
      }

      // Calculs statistiques
      const temperatures = data.map((m: any) => m.temperature);
      const avgTemp = temperatures.reduce((a: number, b: number) => a + b, 0) / temperatures.length;
      const minTemp = Math.min(...temperatures);
      const maxTemp = Math.max(...temperatures);

      const humidities = data.map((m: any) => m.humidity);
      const avgHumidity = humidities.reduce((a: number, b: number) => a + b, 0) / humidities.length;

      const windSpeeds = data.map((m: any) => m.wind_speed);
      const avgWindSpeed = windSpeeds.reduce((a: number, b: number) => a + b, 0) / windSpeeds.length;

      // Détection de tendance (3 dernières mesures)
      let trend = 'données insuffisantes';
      if (data.length >= 3) {
        const temp1 = data[2].temperature;
        const temp2 = data[1].temperature;
        const temp3 = data[0].temperature;
        const change1 = temp2 - temp1;
        const change2 = temp3 - temp2;
        const epsilon = 0.5;

        if (change1 > epsilon && change2 > epsilon) {
          trend = 'hausse';
        } else if (change1 < -epsilon && change2 < -epsilon) {
          trend = 'baisse';
        } else {
          trend = 'stable';
        }
      }

      return new Response(
        JSON.stringify({
          avgTemp: parseFloat(avgTemp.toFixed(2)),
          minTemp: parseFloat(minTemp.toFixed(2)),
          maxTemp: parseFloat(maxTemp.toFixed(2)),
          avgHumidity: parseFloat(avgHumidity.toFixed(2)),
          avgWindSpeed: parseFloat(avgWindSpeed.toFixed(2)),
          trend,
        }),
        {
          headers: { ...corsHeaders, 'Content-Type': 'application/json' },
        }
      );
    }

    // Route non trouvée
    return new Response(JSON.stringify({ error: 'Route non trouvée' }), {
      status: 404,
      headers: { ...corsHeaders, 'Content-Type': 'application/json' },
    });
  } catch (error) {
    return new Response(
      JSON.stringify({ error: (error as Error).message }),
      {
        status: 500,
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      }
    );
  }
});
