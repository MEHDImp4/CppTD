/*
  # Création de la table des mesures météorologiques

  1. Nouvelle Table
    - `measurements`
      - `id` (uuid, clé primaire) - Identifiant unique de la mesure
      - `temperature` (float) - Température en °C
      - `humidity` (float) - Humidité en %
      - `wind_speed` (float) - Vitesse du vent en km/h
      - `wind_direction` (text) - Direction du vent (N, S, E, O, NE, NO, SE, SO)
      - `date_time` (timestamptz) - Date et heure de la mesure
      - `created_at` (timestamptz) - Date de création de l'enregistrement
  
  2. Sécurité
    - Activer RLS sur la table `measurements`
    - Ajouter une politique pour permettre la lecture publique
    - Ajouter une politique pour permettre l'insertion publique
    - Ajouter une politique pour permettre la suppression publique
  
  Note: Pour cette application de démonstration, l'accès est public.
  Dans un environnement de production, vous devriez restreindre l'accès.
*/

-- Créer la table des mesures
CREATE TABLE IF NOT EXISTS measurements (
  id uuid PRIMARY KEY DEFAULT gen_random_uuid(),
  temperature float NOT NULL,
  humidity float NOT NULL,
  wind_speed float NOT NULL,
  wind_direction text NOT NULL,
  date_time timestamptz NOT NULL,
  created_at timestamptz DEFAULT now()
);

-- Activer RLS
ALTER TABLE measurements ENABLE ROW LEVEL SECURITY;

-- Politique pour la lecture (SELECT)
CREATE POLICY "Allow public read access"
  ON measurements
  FOR SELECT
  TO anon
  USING (true);

-- Politique pour l'insertion (INSERT)
CREATE POLICY "Allow public insert access"
  ON measurements
  FOR INSERT
  TO anon
  WITH CHECK (true);

-- Politique pour la suppression (DELETE)
CREATE POLICY "Allow public delete access"
  ON measurements
  FOR DELETE
  TO anon
  USING (true);

-- Créer un index sur date_time pour améliorer les performances des requêtes
CREATE INDEX IF NOT EXISTS idx_measurements_date_time ON measurements(date_time DESC);