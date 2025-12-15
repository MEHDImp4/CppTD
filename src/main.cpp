#include "../include/WeatherStation.h"
#include "../include/Analyzer.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <filesystem>
#include <sstream>
#include <clocale>

#ifdef _WIN32
#include <windows.h>
#endif
#include "../include/ConsoleUtf8.h"

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

#ifdef _WIN32
    // Activer UTF-8 pour la console Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // Utiliser la locale système (peut aider pour l'affichage des accents)
    std::setlocale(LC_ALL, "");

    ConsoleUtf8::write("\n===========================================\n   STATION METEOROLOGIQUE\n===========================================\n");

    // Charger les donnees au demarrage
    station.loadFromFile(dataFile);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        ConsoleUtf8::write("\n  Entrez votre choix (1-8): ");
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
                ConsoleUtf8::writeLine("  Chargement en cours...");
                if (station.loadFromFile(dataFile)) {
                    ConsoleUtf8::writeLine("  Chargement reussi.");
                }
                break;
            }
            case 5: {
                ConsoleUtf8::writeLine("  Sauvegarde en cours...");
                if (station.saveToFile(dataFile)) {
                    ConsoleUtf8::writeLine("  Sauvegarde reussie.");
                }
                break;
            }
            case 6: {
                displayAnalysisMenu(station);
                break;
            }
            case 7: {
                char confirm;
                ConsoleUtf8::write("  Etes-vous sur? (o/n): ");
                cin >> confirm;
                cin.ignore();
                if (confirm == 'o' || confirm == 'O') {
                    station.clear();
                    ConsoleUtf8::writeLine("  Toutes les donnees ont ete effacees.");
                }
                break;
            }
            case 8: {
                ConsoleUtf8::writeLine("  Au revoir!");
                running = false;
                break;
            }
            default: {
                ConsoleUtf8::writeLine("  Choix invalide.");
            }
        }

        if (running && choice >= 1 && choice <= 7) {
            ConsoleUtf8::write("\n  Appuyez sur Entrée pour continuer...");
            cin.get();
        }
    }

    // Sauvegarder les données avant de quitter
    station.saveToFile(dataFile);
    return 0;
}

void displayMenu() {
    ConsoleUtf8::write("\n");
    ConsoleUtf8::write(std::string(60, '=') + "\n");
    ConsoleUtf8::write("  MENU PRINCIPAL\n");
    ConsoleUtf8::write(std::string(60, '=') + "\n");
    ConsoleUtf8::write(
        "  1. Ajouter une nouvelle mesure\n"
        "  2. Afficher toutes les mesures\n"
        "  3. Supprimer une mesure\n"
        "  4. Charger les mesures depuis fichier\n"
        "  5. Sauvegarder les mesures\n"
        "  6. Afficher les analyses\n"
        "  7. Effacer toutes les donnees\n"
        "  8. Quitter\n"
    );
    ConsoleUtf8::write(std::string(60, '='));
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

    ConsoleUtf8::write("  Temperature (°C): ");
    cin >> temp;
    cin.ignore();

    ConsoleUtf8::write("  Humidite (%): ");
    cin >> humidity;
    cin.ignore();

    ConsoleUtf8::write("  Vitesse du vent (km/h): ");
    cin >> windSpeed;
    cin.ignore();

    ConsoleUtf8::write("  Direction du vent (N/S/E/O/NE/NO/SE/SO): ");
    getline(cin, windDir);

    ConsoleUtf8::write("  Date/Heure (format: 2025-01-01 14:30) [Entree pour heure actuelle]: ");
    getline(cin, dateTime);
    if (dateTime.empty()) {
        dateTime = getCurrentDateTime();
    }

    Measurement m(temp, humidity, windSpeed, windDir, dateTime);
    station.addMeasurement(m);

    ConsoleUtf8::writeLine("\n  Mesure ajoutee avec succes!");
    ConsoleUtf8::writeLine("  Total: " + std::to_string(station.getCount()) + " mesure(s).");
}

void removeMeasurementInteractive(WeatherStation& station) {
    if (station.getCount() == 0) {
        ConsoleUtf8::writeLine("  Aucune mesure a supprimer.");
        return;
    }

    ConsoleUtf8::write(std::string(60, '-') + "\n");
    ConsoleUtf8::writeLine("  SUPPRIMER UNE MESURE");
    ConsoleUtf8::write(std::string(60, '-') + "\n");

    station.listAll();

    int index;
    ConsoleUtf8::write("\n  Entrez le numero de la mesure a supprimer (1 a " + std::to_string(station.getCount()) + "): ");
    cin >> index;
    cin.ignore();

    index--; // Convertir en index 0-based

    if (station.removeMeasurement(index)) {
        ConsoleUtf8::writeLine("\n  Mesure supprimee.");
        ConsoleUtf8::writeLine("  Total: " + std::to_string(station.getCount()) + " mesure(s) restante(s).");
    } else {
        ConsoleUtf8::writeLine("\n  Numero invalide.");
    }
}

void displayAnalysisMenu(WeatherStation& station) {
    const auto& measurements = station.getMeasurements();

        if (measurements.empty()) {
           ConsoleUtf8::writeLine("  Aucune mesure disponible pour l'analyse.");
           return;
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);

        float avgTemp = Analyzer::averageTemperature(measurements);
        float minTemp = Analyzer::minTemperature(measurements);
        float maxTemp = Analyzer::maxTemperature(measurements);
        float avgHumidity = Analyzer::averageHumidity(measurements);
        float avgWind = Analyzer::averageWindSpeed(measurements);
        std::string trend = Analyzer::detectTemperatureTrend(measurements);

        oss << "\n" << std::string(60, '=') << "\n";
        oss << "  ANALYSES STATISTIQUES\n";
        oss << std::string(60, '=') << "\n";

        oss << "\n  TEMPERATURE:\n";
        oss << "      - Moyenne: " << avgTemp << "°C\n";
        oss << "      - Minimum: " << minTemp << "°C\n";
        oss << "      - Maximum: " << maxTemp << "°C\n";
        oss << "      - Tendance (3 dernieres): " << trend << "\n";

        oss << "\n  HUMIDITE:\n";
        oss << "      - Moyenne: " << avgHumidity << "%\n";

        oss << "\n  VENT:\n";
        oss << "      - Vitesse moyenne: " << avgWind << " km/h\n";

        oss << "\n" << std::string(60, '=') << "\n";

        ConsoleUtf8::write(oss.str());
}
