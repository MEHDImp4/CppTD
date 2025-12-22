import { createClient } from '@supabase/supabase-js';
import { readFileSync } from 'fs';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const supabaseUrl = 'https://iopxyonobcjgmfodumnl.supabase.co';
const supabaseKey = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImlvcHh5b25vYmNqZ21mb2R1bW5sIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NjY0MzU4OTAsImV4cCI6MjA4MjAxMTg5MH0.OEu24YKhC1N0nnH0snTPy_q81C2l7dPq6vFTVn8PaSQ';

const supabase = createClient(supabaseUrl, supabaseKey);

async function migrateMeasurements() {
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

    const { data, error } = await supabase
      .from('measurements')
      .insert(measurements)
      .select();

    if (error) {
      console.error('❌ Erreur lors de la migration:', error);
      return;
    }

    console.log(`🎉 Migration réussie! ${data.length} mesure(s) ajoutée(s) à Supabase`);
  } catch (error) {
    console.error('❌ Erreur:', error.message);
  }
}

migrateMeasurements();
