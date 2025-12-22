# Interface Web - Station Météorologique

Interface web moderne pour gérer et visualiser les données de la station météorologique.

## Architecture

```
┌─────────────────┐
│  Interface Web  │ (Vite + Vanilla JS)
│   (Browser)     │
└────────┬────────┘
         │ HTTPS
         ▼
┌─────────────────┐
│  Edge Function  │ (weather-api)
│   (Supabase)    │
└────────┬────────┘
         │ SQL
         ▼
┌─────────────────┐
│   PostgreSQL    │
│   (Supabase)    │
└─────────────────┘
```

## Fonctionnalités

- ✅ **Affichage des mesures** - Liste toutes les mesures météo
- ✅ **Ajout de mesures** - Formulaire pour ajouter de nouvelles mesures
- ✅ **Suppression de mesures** - Supprimer des mesures individuelles
- ✅ **Analyses statistiques** - Températures min/max/moyenne, humidité, vent, tendances
- ✅ **Interface responsive** - Fonctionne sur mobile et desktop
- ✅ **Design moderne** - Interface épurée et professionnelle

## Installation

```bash
# Depuis le dossier web/
npm install
```

## Configuration

Les variables d'environnement sont déjà configurées dans `.env`:

```
VITE_SUPABASE_URL=https://iopxyonobcjgmfodumnl.supabase.co
VITE_SUPABASE_ANON_KEY=eyJhbGci...
```

## Démarrage

```bash
# Démarrer le serveur de développement
npm run dev

# L'application sera accessible sur http://localhost:3000
```

## Migration des données

Pour migrer les données du fichier `data/measurements.txt` vers Supabase:

```bash
node migrate-data.js
```

## Build pour production

```bash
npm run build

# Les fichiers seront générés dans dist/
# Pour prévisualiser:
npm run preview
```

## API Endpoints

L'Edge Function `weather-api` expose les endpoints suivants:

- **GET** `/measurements` - Récupérer toutes les mesures
- **POST** `/measurements` - Ajouter une mesure
- **DELETE** `/measurements/:id` - Supprimer une mesure
- **GET** `/analysis` - Récupérer les statistiques

## Structure des fichiers

```
web/
├── index.html          # Page HTML principale
├── style.css           # Styles CSS
├── main.js             # Logique JavaScript
├── vite.config.js      # Configuration Vite
├── package.json        # Dépendances
├── migrate-data.js     # Script de migration
└── .env                # Variables d'environnement
```

## Technologies utilisées

- **Vite** - Build tool moderne et rapide
- **Supabase** - Backend as a Service (base de données + API)
- **Vanilla JavaScript** - Pas de framework, simplicité maximale
- **CSS moderne** - Variables CSS, Grid, Flexbox

## Notes

- L'interface utilise la même logique métier que le programme C++
- Les calculs statistiques sont effectués côté serveur (Edge Function)
- La base de données Supabase stocke toutes les mesures
- L'accès est public pour cette démo (pas d'authentification)
