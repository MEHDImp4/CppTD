import { createClient } from 'jsr:@supabase/supabase-js@2';

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

Deno.serve(async (req: Request) => {
  // Handle CORS preflight
  if (req.method === 'OPTIONS') {
    return new Response(null, {
      status: 200,
      headers: corsHeaders,
    });
  }

  try {
    const supabaseUrl = Deno.env.get('SUPABASE_URL')!;
    const supabaseKey = Deno.env.get('SUPABASE_ANON_KEY')!;
    const supabase = createClient(supabaseUrl, supabaseKey);

    const url = new URL(req.url);
    const path = url.pathname.replace('/weather-api', '');

    // GET /measurements - Récupérer toutes les mesures
    if (path === '/measurements' && req.method === 'GET') {
      const { data, error } = await supabase
        .from('measurements')
        .select('*')
        .order('date_time', { ascending: false });

      if (error) {
        throw error;
      }

      return new Response(JSON.stringify(data), {
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      });
    }

    // POST /measurements - Ajouter une mesure
    if (path === '/measurements' && req.method === 'POST') {
      const body: Measurement = await req.json();

      const { data, error } = await supabase
        .from('measurements')
        .insert({
          temperature: body.temperature,
          humidity: body.humidity,
          wind_speed: body.wind_speed,
          wind_direction: body.wind_direction,
          date_time: body.date_time,
        })
        .select()
        .single();

      if (error) {
        throw error;
      }

      return new Response(JSON.stringify(data), {
        status: 201,
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      });
    }

    // DELETE /measurements/:id - Supprimer une mesure
    if (path.startsWith('/measurements/') && req.method === 'DELETE') {
      const id = path.split('/')[2];

      const { error } = await supabase
        .from('measurements')
        .delete()
        .eq('id', id);

      if (error) {
        throw error;
      }

      return new Response(JSON.stringify({ success: true }), {
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      });
    }

    // GET /analysis - Récupérer les analyses statistiques
    if (path === '/analysis' && req.method === 'GET') {
      const { data, error } = await supabase
        .from('measurements')
        .select('*')
        .order('date_time', { ascending: false });

      if (error) {
        throw error;
      }

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
      JSON.stringify({ error: error.message }),
      {
        status: 500,
        headers: { ...corsHeaders, 'Content-Type': 'application/json' },
      }
    );
  }
});
