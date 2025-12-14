#include "../include/WeatherStation.h"
#include "../include/Analyzer.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <filesystem>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

// Declarations des fonctions
void displayMenu();
string getCurrentDateTime();
string locateDataFile(int argc, char* argv[]);
void addMeasurementInteractive(WeatherStation& station);
void removeMeasurementInteractive(WeatherStation& station);
void displayAnalysisMenu(WeatherStation& station);

string locateDataFile(int argc, char* argv[]) {
    vector<fs::path> candidates;

    // Current working directory candidates
    candidates.push_back(fs::current_path() / "data/measurements.txt");
    candidates.push_back(fs::current_path() / "src" / "data" / "measurements.txt");

    // Try to use argv[0] (executable path) to locate repository root
    if (argc > 0 && argv[0] != nullptr) {
        try {
            fs::path exe = fs::absolute(argv[0]);
            fs::path exeDir = exe.parent_path();
            candidates.push_back(exeDir / "data" / "measurements.txt");
            candidates.push_back(exeDir.parent_path() / "data" / "measurements.txt");
        } catch (...) {
            // ignore filesystem errors and continue with other candidates
        }
    }

    // A guess: repository root is parent of cwd
    candidates.push_back(fs::current_path().parent_path() / "data" / "measurements.txt");

    for (auto &p : candidates) {
        if (!p.empty() && fs::exists(p)) {
            return p.string();
        }
    }

    // Fallback to relative path (will produce the original error if not found)
    return string("data/measurements.txt");
}

int main(int argc, char* argv[]) {
    WeatherStation station;
    string dataFile = locateDataFile(argc, argv);

    cout << "\n===========================================\n"
         << "   STATION METEOROLOGIQUE\n"
         << "===========================================\n";

    // Charger les donnees au demarrage
    station.loadFromFile(dataFile);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cout << "\n  Entrez votre choix (1-8): ";
        cin >> choice;
        cin.ignore();  // Nettoie le buffer

        cout << "\n";

        switch (choice) {
            case 1: {
                addMeasurementInteractive(station);
                break;
            }
            case 2: {
                station.listAll();
                break;
            }
            case 3: {
                removeMeasurementInteractive(station);
                break;
            }
            case 4: {
                cout << "  Chargement en cours...\n";
                if (station.loadFromFile(dataFile)) {
                    cout << "  Chargement reussi.\n";
                }
                break;
            }
            case 5: {
                cout << "  Sauvegarde en cours...\n";
                if (station.saveToFile(dataFile)) {
                    cout << "  Sauvegarde reussie.\n";
                }
                break;
            }
            case 6: {
                displayAnalysisMenu(station);
                break;
            }
            case 7: {
                char confirm;
                cout << "  Etes-vous sur? (o/n): ";
                cin >> confirm;
                cin.ignore();
                if (confirm == 'o' || confirm == 'O') {
                    station.clear();
                    cout << "  Toutes les donnees ont ete effacees.\n";
                }
                break;
            }
            case 8: {
                cout << "  Au revoir!\n";
                running = false;
                break;
            }
            default: {
                cout << "  Choix invalide.\n";
            }
        }

        if (running && choice >= 1 && choice <= 7) {
            cout << "\n  Appuyez sur Entrée pour continuer...";
            cin.get();
        }
    }

    // Sauvegarder les données avant de quitter
    station.saveToFile(dataFile);
    return 0;
}

void displayMenu() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "  MENU PRINCIPAL\n";
    cout << string(60, '=') << "\n";
    cout << "  1. Ajouter une nouvelle mesure\n"
         << "  2. Afficher toutes les mesures\n"
         << "  3. Supprimer une mesure\n"
         << "  4. Charger les mesures depuis fichier\n"
         << "  5. Sauvegarder les mesures\n"
         << "  6. Afficher les analyses\n"
         << "  7. Effacer toutes les donnees\n"
         << "  8. Quitter\n"
         << string(60, '=');
}

string getCurrentDateTime() {
    auto now = time(nullptr);
    auto tm = *localtime(&now);
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

void addMeasurementInteractive(WeatherStation& station) {
    cout << string(60, '-') << "\n"
         << "  AJOUTER UNE NOUVELLE MESURE\n"
         << string(60, '-') << "\n";

    float temp, humidity, windSpeed;
    string windDir, dateTime;

    cout << "  Temperature (°C): ";
    cin >> temp;
    cin.ignore();

    cout << "  Humidite (%): ";
    cin >> humidity;
    cin.ignore();

    cout << "  Vitesse du vent (km/h): ";
    cin >> windSpeed;
    cin.ignore();

    cout << "  Direction du vent (N/S/E/O/NE/NO/SE/SO): ";
    getline(cin, windDir);

    cout << "  Date/Heure (format: 2025-01-01 14:30) [Entree pour heure actuelle]: ";
    getline(cin, dateTime);
    if (dateTime.empty()) {
        dateTime = getCurrentDateTime();
    }

    Measurement m(temp, humidity, windSpeed, windDir, dateTime);
    station.addMeasurement(m);

    cout << "\n  Mesure ajoutee avec succes!\n"
         << "  Total: " << station.getCount() << " mesure(s).\n";
}

void removeMeasurementInteractive(WeatherStation& station) {
    if (station.getCount() == 0) {
        cout << "  Aucune mesure a supprimer.\n";
        return;
    }

    cout << string(60, '-') << "\n"
         << "  SUPPRIMER UNE MESURE\n"
         << string(60, '-') << "\n";

    station.listAll();

    int index;
    cout << "\n  Entrez le numero de la mesure a supprimer (1 a "
         << station.getCount() << "): ";
    cin >> index;
    cin.ignore();

    index--; // Convertir en index 0-based

    if (station.removeMeasurement(index)) {
        cout << "\n  Mesure supprimee.\n"
             << "  Total: " << station.getCount() << " mesure(s) restante(s).\n";
    } else {
        cout << "\n  Numero invalide.\n";
    }
}

void displayAnalysisMenu(WeatherStation& station) {
    const auto& measurements = station.getMeasurements();

    if (measurements.empty()) {
        cout << "  Aucune mesure disponible pour l'analyse.\n";
        return;
    }

    cout << string(60, '=') << "\n"
         << "  ANALYSES STATISTIQUES\n"
         << string(60, '=') << "\n";

    cout << fixed << setprecision(2);

    float avgTemp = Analyzer::averageTemperature(measurements);
    float minTemp = Analyzer::minTemperature(measurements);
    float maxTemp = Analyzer::maxTemperature(measurements);
    float avgHumidity = Analyzer::averageHumidity(measurements);
    float avgWind = Analyzer::averageWindSpeed(measurements);
    string trend = Analyzer::detectTemperatureTrend(measurements);

    cout << "\n  TEMPERATURE:\n"
         << "      - Moyenne: " << avgTemp << "°C\n"
         << "      - Minimum: " << minTemp << "°C\n"
         << "      - Maximum: " << maxTemp << "°C\n"
         << "      - Tendance (3 dernieres): " << trend << "\n";

    cout << "\n  HUMIDITE:\n"
         << "      - Moyenne: " << avgHumidity << "%\n";

    cout << "\n  VENT:\n"
         << "      - Vitesse moyenne: " << avgWind << " km/h\n";

    cout << "\n" << string(60, '=') << "\n";
}
