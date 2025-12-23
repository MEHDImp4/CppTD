# Documentation Complète - Station Météorologique en C++

## Vue d'ensemble du projet

Ce projet est une application C++ qui gère une **station météorologique**. Elle permet de:
- Ajouter des mesures météo (température, humidité, vent)
- Afficher et supprimer des mesures
- Analyser les données (moyennes, min, max, tendances)
- Sauvegarder et charger les données dans un fichier

L'architecture utilise **3 couches** :

```
USER (Interface) → main.cpp (Affichage & Menu)
                   ↓
LOGIC (Métier) → WeatherStation (Gestion des données)
                   ↓
DATA → Measurement (Une seule mesure) + Analyzer (Statistiques)
```

---

## CLASSE 1 : Measurement (Fichier: include/Measurement.h & src/Measurement.cpp)

### Rôle
Représente **UNE SEULE mesure météo** avec 5 propriétés.

### Les Propriétés (Variables Privées)
```cpp
float temperature;              // La température en °C
float humidity;                 // L'humidité en % (0-100)
float windSpeed;                // La vitesse du vent en km/h
std::string windDirection;      // Direction: N, S, E, O, NE, NO, SE, SO
std::string dateTime;           // Date et heure: "2025-01-15 14:30"
```

### Constructeur
```cpp
Measurement(float temp = 0.0f, float hum = 0.0f, float wind = 0.0f,
            const std::string& dir = "N", const std::string& time = "")
    : temperature(temp), humidity(hum), windSpeed(wind),
      windDirection(dir), dateTime(time) {}
```
**Explication :**
- Initialise tous les paramètres avec des valeurs par défaut
- La syntaxe avec `:` s'appelle "liste d'initialisation" (plus efficace que de les assigner après)
- Si je crée `Measurement()` sans arguments, tous les paramètres prennent leurs valeurs par défaut (0.0f, "N", "")

### Les Getters (Lecteurs)
```cpp
float getTemperature() const;   // Retourne la température
float getHumidity() const;      // Retourne l'humidité
float getWindSpeed() const;     // Retourne la vitesse du vent
std::string getWindDirection() const;  // Retourne la direction
std::string getDateTime() const; // Retourne la date/heure
```
**Explication :**
- `const` = **ne modifie pas** l'objet (lecture seule)
- Permette à d'autres classes d'accéder aux données sans les changer

### Les Setters (Modificateurs)
```cpp
void setTemperature(float temp);
void setHumidity(float hum);
// ... etc
```
**Explication :**
- Permettent de **modifier** les données après création
- Utile si on veut corriger une mesure

### Méthode toString() - Affichage Lisible
```cpp
std::string Measurement::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1)  // 1 décimale pour l'affichage
        << "  Date/Heure: " << dateTime << "\n"
        << "  Température: " << temperature << "°C\n"
        << "  Humidité: " << humidity << "%\n"
        << "  Vitesse du vent: " << windSpeed << " km/h\n"
        << "  Direction du vent: " << windDirection;
    return oss.str();
}
```
**Explication :**
- Formate la mesure pour affichage à l'écran
- `std::setprecision(1)` = affiche 1 chiffre après la virgule
- Exemple de résultat:
  ```
  Date/Heure: 2025-01-15 14:30
  Température: 25.3°C
  Humidité: 65.0%
  Vitesse du vent: 12.5 km/h
  Direction du vent: NE
  ```

### Méthode serialize() - Sauvegarde dans un Fichier
```cpp
std::string Measurement::serialize() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2)  // 2 décimales pour la sauvegarde
        << temperature << "|" << humidity << "|" << windSpeed << "|"
        << windDirection << "|" << dateTime;
    return oss.str();
}
```
**Explication :**
- Convertit la mesure en **texte pour fichier**
- Format: `25.50|65.00|12.50|NE|2025-01-15 14:30`
- Le `|` sépare les données (format pipe-délimité)
- `std::setprecision(2)` = 2 décimales (plus de précision que l'affichage)

### Méthode deserialize() - Chargement depuis Fichier
```cpp
Measurement Measurement::deserialize(const std::string& line) {
    // Ignorer les lignes vides ou les commentaires (#)
    if (line.empty() || line[0] == '#') {
        return Measurement();  // Retourne une mesure par défaut (vide)
    }

    std::istringstream iss(line);
    float temp = 0.0f, hum = 0.0f, wind = 0.0f;
    std::string dir = "N", time = "";
    char pipe;

    // Lire: temp|hum|wind|dir|time
    if (iss >> temp >> pipe >> hum >> pipe >> wind >> pipe) {
        std::getline(iss, dir, '|');    // Lit jusqu'au prochain |
        std::getline(iss, time);        // Lit le reste
    }

    return Measurement(temp, hum, wind, dir, time);
}
```
**Explication :**
- **Méthode statique** (pas besoin d'un objet, on peut l'appeler sur la classe)
- Prend une ligne du fichier: `"25.50|65.00|12.50|NE|2025-01-15 14:30"`
- La **parse** (découpe) en utilisant `|` comme séparateur
- Crée et retourne un objet `Measurement` avec ces données

---

## CLASSE 2 : WeatherStation (Fichier: include/WeatherStation.h & src/WeatherStation.cpp)

### Rôle
Gère une **collection de mesures** (les stocke, ajoute, supprime, sauvegarde).

### Les Propriétés
```cpp
std::vector<Measurement> measurements;  // Liste de toutes les mesures
```
**Explication :**
- `std::vector` = tableau dynamique (peut grandir)
- Chaque élément est un objet `Measurement`

### Constructeur
```cpp
WeatherStation::WeatherStation() {}
```
**Explication :**
- **Vide** - crée juste un vecteur vide au départ
- Le vecteur sera rempli avec des mesures plus tard

### Méthode addMeasurement() - Ajouter une Mesure
```cpp
void WeatherStation::addMeasurement(const Measurement& measurement) {
    measurements.push_back(measurement);
}
```
**Explication :**
- `push_back()` = ajoute une mesure à la fin du vecteur
- `const Measurement&` = on passe une **référence** (pas une copie) pour plus d'efficacité
- Exemple:
  ```cpp
  WeatherStation station;
  Measurement m1(25.5, 65, 12.5, "NE", "2025-01-15 14:30");
  station.addMeasurement(m1);  // Ajoute m1 à station
  ```

### Méthode removeMeasurement() - Supprimer une Mesure
```cpp
bool WeatherStation::removeMeasurement(int index) {
    // Vérifier que l'index est valide (entre 0 et size-1)
    if (index < 0 || index >= static_cast<int>(measurements.size())) {
        return false;  // Index invalide
    }
    measurements.erase(measurements.begin() + index);  // Supprime l'élément
    return true;   // Succès
}
```
**Explication :**
- Supprime la mesure à position `index`
- Retourne `true` si succès, `false` si l'index est invalide
- `erase()` = enlève un élément du vecteur
- Exemple:
  ```cpp
  if (station.removeMeasurement(0)) {
      cout << "Première mesure supprimée";
  } else {
      cout << "Index invalide";
  }
  ```

### Méthode getMeasurements() - Accéder à toutes les Mesures
```cpp
const std::vector<Measurement>& WeatherStation::getMeasurements() const {
    return measurements;
}
```
**Explication :**
- Retourne une **référence** à tout le vecteur (pas une copie)
- `const` = promesse de ne pas modifier l'objet
- Utilisé par `Analyzer` pour les statistiques

### Méthode getCount() - Nombre de Mesures
```cpp
int WeatherStation::getCount() const {
    return measurements.size();
}
```
**Explication :**
- Retourne combien de mesures sont stockées

### Méthode listAll() - Afficher toutes les Mesures
```cpp
void WeatherStation::listAll() const {
    if (measurements.empty()) {
        std::cout << "\n  Aucune mesure stockee.\n";
        return;
    }

    std::cout << "\n" << std::string(60, '=') << "\n";  // Ligne de séparation
    std::cout << "  LISTE DE TOUTES LES MESURES (" << measurements.size() << " total)\n";
    std::cout << std::string(60, '=') << "\n";

    for (size_t i = 0; i < measurements.size(); ++i) {
        std::cout << "\n  [Mesure #" << (i + 1) << "]\n";
        std::cout << measurements[i].toString() << "\n";  // Affiche chaque mesure
    }

    std::cout << "\n" << std::string(60, '=') << "\n";
}
```
**Explication :**
- Affiche toutes les mesures de façon lisible
- `std::string(60, '=')` = crée une ligne de 60 caractères `=`
- Boucle sur chaque mesure et appelle `toString()` pour l'afficher

### Méthode saveToFile() - Sauvegarder dans un Fichier
```cpp
bool WeatherStation::saveToFile(const std::string& path) {
    std::ofstream file(path);  // Ouvre le fichier en écriture
    if (!file.is_open()) {
        std::cerr << "  Erreur: impossible d'ouvrir " << path << " en ecriture.\n";
        return false;  // Échoue si le fichier ne peut pas s'ouvrir
    }

    for (const auto& measurement : measurements) {
        file << measurement.serialize() << "\n";  // Écrit chaque mesure
    }

    file.close();
    std::cout << "  " << measurements.size() << " mesure(s) sauvegardee(s) dans "
              << path << "\n";
    return true;
}
```
**Explication :**
- Crée/ouvre le fichier `data/measurements.txt`
- Boucle sur chaque mesure et appelle `serialize()` pour convertir en texte
- Écrit chaque ligne dans le fichier
- Ferme le fichier

### Méthode loadFromFile() - Charger depuis un Fichier
```cpp
bool WeatherStation::loadFromFile(const std::string& path) {
    std::ifstream file(path);  // Ouvre le fichier en lecture
    if (!file.is_open()) {
        std::cerr << "  Fichier " << path << " introuvable.\n";
        return false;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {  // Lit ligne par ligne
        if (!line.empty()) {  // Ignore les lignes vides
            measurements.push_back(Measurement::deserialize(line));  // Convertit et ajoute
            count++;
        }
    }

    file.close();
    std::cout << "  " << count << " mesure(s) chargee(s) depuis " << path << "\n";
    return true;
}
```
**Explication :**
- Lit le fichier `data/measurements.txt`
- Pour chaque ligne non-vide:
  - Appelle `Measurement::deserialize()` pour convertir en objet
  - L'ajoute au vecteur
- Affiche combien de mesures ont été chargées

### Méthode clear() - Effacer Toutes les Mesures
```cpp
void WeatherStation::clear() {
    measurements.clear();  // Vide le vecteur
}
```
**Explication :**
- Supprime TOUTES les mesures
- Le vecteur reste vide mais valide

---

## CLASSE 3 : Analyzer (Fichier: include/Analyzer.h & src/Analyzer.cpp)

### Rôle
Effectue des **analyses statistiques** sur les mesures (moyennes, min, max, tendances).

### Caractéristique Spéciale
- **Toutes les méthodes sont `static`** = pas besoin de créer un objet
- On les appelle directement sur la classe: `Analyzer::averageTemperature(...)`

### Méthode averageTemperature() - Température Moyenne
```cpp
float Analyzer::averageTemperature(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;  // Si aucune mesure, retourner 0
    }

    float sum = 0.0f;
    for (const auto& m : measurements) {
        sum += m.getTemperature();  // Additionner toutes les températures
    }
    return sum / measurements.size();  // Diviser par le nombre de mesures
}
```
**Explication :**
- Calcule la moyenne en additionnant et en divisant par le nombre
- Exemple: (20 + 25 + 30) / 3 = 25°C

### Méthode minTemperature() - Température Minimale
```cpp
float Analyzer::minTemperature(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float minTemp = measurements[0].getTemperature();  // Commence avec la première
    for (const auto& m : measurements) {
        if (m.getTemperature() < minTemp) {
            minTemp = m.getTemperature();  // Met à jour si une plus petite valeur
        }
    }
    return minTemp;
}
```
**Explication :**
- Compare chaque température pour trouver la plus petite
- Exemple: min(20, 25, 30) = 20°C

### Méthode maxTemperature() - Température Maximale
```cpp
float Analyzer::maxTemperature(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float maxTemp = measurements[0].getTemperature();
    for (const auto& m : measurements) {
        if (m.getTemperature() > maxTemp) {
            maxTemp = m.getTemperature();  // Met à jour si une plus grande valeur
        }
    }
    return maxTemp;
}
```
**Explication :**
- Trouve la température la plus élevée
- Exemple: max(20, 25, 30) = 30°C

### Méthode averageHumidity() - Humidité Moyenne
```cpp
float Analyzer::averageHumidity(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (const auto& m : measurements) {
        sum += m.getHumidity();
    }
    return sum / measurements.size();
}
```
**Explication :**
- Même logique que `averageTemperature()` mais pour l'humidité

### Méthode averageWindSpeed() - Vitesse du Vent Moyenne
```cpp
float Analyzer::averageWindSpeed(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (const auto& m : measurements) {
        sum += m.getWindSpeed();
    }
    return sum / measurements.size();
}
```
**Explication :**
- Calcule la vitesse du vent moyenne

### Méthode detectTemperatureTrend() - Détection de Tendance
```cpp
std::string Analyzer::detectTemperatureTrend(const std::vector<Measurement>& measurements) {
    // Besoin d'au moins 3 mesures
    if (measurements.size() < 3) {
        return "données insuffisantes";
    }

    // Récupère les 3 dernières mesures
    size_t size = measurements.size();
    float temp1 = measurements[size - 3].getTemperature();  // Avant-dernière
    float temp2 = measurements[size - 2].getTemperature();  // Médiane
    float temp3 = measurements[size - 1].getTemperature();  // Dernière

    // Compare les évolutions
    float change1 = temp2 - temp1;  // Variation entre mesure 1 et 2
    float change2 = temp3 - temp2;  // Variation entre mesure 2 et 3

    const float epsilon = 0.5f;  // Seuil: si variation < 0.5, c'est stable

    // Si les deux changements vont dans le même sens
    if (change1 > epsilon && change2 > epsilon) {
        return "hausse";  // Les 2 variations sont positives = hausse
    } else if (change1 < -epsilon && change2 < -epsilon) {
        return "baisse";  // Les 2 variations sont négatives = baisse
    } else {
        return "stable";  // Pas de tendance claire
    }
}
```
**Explication :**
- Analyse les 3 **dernières** mesures pour voir la tendance
- Exemple 1:
  ```
  Temp1 = 20°C
  Temp2 = 22°C (change1 = +2°C)
  Temp3 = 24°C (change2 = +2°C)
  Résultat: "hausse" (les 2 variations sont positives)
  ```
- Exemple 2:
  ```
  Temp1 = 25°C
  Temp2 = 23°C (change1 = -2°C)
  Temp3 = 21°C (change2 = -2°C)
  Résultat: "baisse" (les 2 variations sont négatives)
  ```
- Exemple 3:
  ```
  Temp1 = 22°C
  Temp2 = 25°C (change1 = +3°C)
  Temp3 = 23°C (change2 = -2°C)
  Résultat: "stable" (une augmente, une diminue)
  ```

---

## FONCTION PRINCIPALE : main()

### Flux Global

1. **Création de la station**
   ```cpp
   WeatherStation station;
   std::string dataFile = "data/measurements.txt";
   ```

2. **Chargement des données au démarrage**
   ```cpp
   station.loadFromFile(dataFile);  // Lit le fichier
   ```

3. **Boucle principale (Menu)**
   ```cpp
   while (running) {
       displayMenu();           // Affiche les options
       // ... Lit le choix de l'utilisateur
       // ... Exécute l'action correspondante
   }
   ```

4. **Sauvegarde avant quitter**
   ```cpp
   station.saveToFile(dataFile);  // Sauvegarde avant de terminer
   ```

### Les Fonctions Utilitaires

#### displayMenu()
```cpp
void displayMenu() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  MENU PRINCIPAL\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "  1. Ajouter une nouvelle mesure\n"
              << "  2. Afficher toutes les mesures\n"
              // ... etc
}
```
**Explication :**
- Affiche le menu avec les 8 options

#### getCurrentDateTime()
```cpp
std::string getCurrentDateTime() {
    auto now = std::time(nullptr);  // Heure système actuelle
    auto tm = *std::localtime(&now);  // Convertit en structure temps
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");  // Format: 2025-01-15 14:30
    return oss.str();
}
```
**Explication :**
- Récupère la date/heure actuelle du système
- La formate en `"2025-01-15 14:30"`

#### addMeasurementInteractive()
```cpp
void addMeasurementInteractive(WeatherStation& station) {
    // ... Affiche des prompts
    std::cout << "  Temperature (°C): ";
    std::cin >> temp;
    // ... etc pour humidity, windSpeed, windDir

    // Crée un objet Measurement
    Measurement m(temp, humidity, windSpeed, windDir, dateTime);

    // L'ajoute à la station
    station.addMeasurement(m);
}
```
**Explication :**
- Dialogue interactif avec l'utilisateur
- Demande la température, l'humidité, le vent, etc.
- Crée un `Measurement` avec ces données
- L'ajoute à `WeatherStation`

#### removeMeasurementInteractive()
```cpp
void removeMeasurementInteractive(WeatherStation& station) {
    // Affiche toutes les mesures
    station.listAll();

    // Demande l'index à supprimer
    int index;
    std::cout << "\n  Entrez le numero de la mesure a supprimer: ";
    std::cin >> index;

    index--;  // Convertit de 1-based en 0-based

    // Supprime
    if (station.removeMeasurement(index)) {
        std::cout << "\n  Mesure supprimee.\n";
    }
}
```
**Explication :**
- L'utilisateur rentre le numéro (1, 2, 3, ...)
- On soustrait 1 pour convertir en index interne (0, 1, 2, ...)
- Appelle `removeMeasurement()`

#### displayAnalysisMenu()
```cpp
void displayAnalysisMenu(WeatherStation& station) {
    const auto& measurements = station.getMeasurements();

    // Calcule les statistiques
    float avgTemp = Analyzer::averageTemperature(measurements);
    float minTemp = Analyzer::minTemperature(measurements);
    float maxTemp = Analyzer::maxTemperature(measurements);

    // Affiche les résultats
    std::cout << "  TEMPERATURE:\n"
              << "      - Moyenne: " << avgTemp << "°C\n"
              << "      - Minimum: " << minTemp << "°C\n"
              << "      - Maximum: " << maxTemp << "°C\n";
}
```
**Explication :**
- Utilise la classe `Analyzer` pour calculer les statistiques
- Affiche les résultats de manière lisible

---

## Les 8 Choix du Menu

```
Case 1: Ajouter une mesure       → appel addMeasurementInteractive()
Case 2: Afficher les mesures     → appel station.listAll()
Case 3: Supprimer une mesure     → appel removeMeasurementInteractive()
Case 4: Charger depuis fichier   → appel station.loadFromFile()
Case 5: Sauvegarder dans fichier → appel station.saveToFile()
Case 6: Afficher les analyses    → appel displayAnalysisMenu()
Case 7: Effacer toutes données   → appel station.clear()
Case 8: Quitter                  → running = false
```

---

## Format du Fichier data/measurements.txt

**Exemple de fichier:**
```
25.50|65.00|12.50|NE|2025-01-15 14:30
18.20|72.00|8.30|N|2025-01-15 15:00
22.10|68.50|10.20|NO|2025-01-15 16:00
```

**Format:** `température|humidité|vitesseVent|direction|dateTime`

---

## Résumé des Patterns Utilisés

### 1. Encapsulation (Classes)
- Données privées (`private:`)
- Accès via getters/setters
- Protège les données

### 2. Sérialisation
- `serialize()` : Objet → Texte pour fichier
- `deserialize()` : Texte du fichier → Objet

### 3. Statique
- `Analyzer` utilise que des méthodes statiques
- Pas besoin de créer un objet `Analyzer`
- Appel direct: `Analyzer::averageTemperature(...)`

### 4. Références
- Éviter les copies inutiles
- Exemple: `const std::vector<Measurement>&` dans Analyzer

### 5. Validation
- Vérifie les vecteurs vides
- Valide les index

