#include "WeatherStation.h"
#include "Analyzer.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <sstream>

// Declarations des fonctions
void displayMenu();
std::string getCurrentDateTime();
void addMeasurementInteractive(WeatherStation& station);
void removeMeasurementInteractive(WeatherStation& station);
void displayAnalysisMenu(WeatherStation& station);

int main() {
    WeatherStation station;
    std::string dataFile = "data/measurements.txt";

    std::cout << "\n===========================================\n"
              << "   STATION METEOROLOGIQUE\n"
              << "===========================================\n";

    // Charger les donnees au demarrage
    station.loadFromFile(dataFile);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        std::cout << "\n  Entrez votre choix (1-8): ";
        std::cin >> choice;
        std::cin.ignore();  // Nettoie le buffer

        std::cout << "\n";

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
                std::cout << "  Chargement en cours...\n";
                if (station.loadFromFile(dataFile)) {
                    std::cout << "  Chargement reussi.\n";
                }
                break;
            }
            case 5: {
                std::cout << "  Sauvegarde en cours...\n";
                if (station.saveToFile(dataFile)) {
                    std::cout << "  Sauvegarde reussie.\n";
                }
                break;
            }
            case 6: {
                displayAnalysisMenu(station);
                break;
            }
            case 7: {
                char confirm;
                std::cout << "  Etes-vous sur? (o/n): ";
                std::cin >> confirm;
                std::cin.ignore();
                if (confirm == 'o' || confirm == 'O') {
                    station.clear();
                    std::cout << "  Toutes les donnees ont ete effacees.\n";
                }
                break;
            }
            case 8: {
                std::cout << "  Au revoir!\n";
                running = false;
                break;
            }
            default: {
                std::cout << "  Choix invalide.\n";
            }
        }

        if (running && choice >= 1 && choice <= 7) {
            std::cout << "\n  Appuyez sur Entrée pour continuer...";
            std::cin.get();
        }
    }

    // Sauvegarder les données avant de quitter
    station.saveToFile(dataFile);
    return 0;
}

void displayMenu() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  MENU PRINCIPAL\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "  1. Ajouter une nouvelle mesure\n"
              << "  2. Afficher toutes les mesures\n"
              << "  3. Supprimer une mesure\n"
              << "  4. Charger les mesures depuis fichier\n"
              << "  5. Sauvegarder les mesures\n"
              << "  6. Afficher les analyses\n"
              << "  7. Effacer toutes les donnees\n"
              << "  8. Quitter\n"
              << std::string(60, '=');
}

std::string getCurrentDateTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

void addMeasurementInteractive(WeatherStation& station) {
    std::cout << std::string(60, '-') << "\n"
              << "  AJOUTER UNE NOUVELLE MESURE\n"
              << std::string(60, '-') << "\n";

    float temp, humidity, windSpeed;
    std::string windDir, dateTime;

    std::cout << "  Temperature (°C): ";
    std::cin >> temp;
    std::cin.ignore();

    std::cout << "  Humidite (%): ";
    std::cin >> humidity;
    std::cin.ignore();

    std::cout << "  Vitesse du vent (km/h): ";
    std::cin >> windSpeed;
    std::cin.ignore();

    std::cout << "  Direction du vent (N/S/E/O/NE/NO/SE/SO): ";
    std::getline(std::cin, windDir);

    std::cout << "  Date/Heure (format: 2025-01-01 14:30) [Entree pour heure actuelle]: ";
    std::getline(std::cin, dateTime);
    if (dateTime.empty()) {
        dateTime = getCurrentDateTime();
    }

    Measurement m(temp, humidity, windSpeed, windDir, dateTime);
    station.addMeasurement(m);

    std::cout << "\n  Mesure ajoutee avec succes!\n"
              << "  Total: " << station.getCount() << " mesure(s).\n";
}

void removeMeasurementInteractive(WeatherStation& station) {
    if (station.getCount() == 0) {
        std::cout << "  Aucune mesure a supprimer.\n";
        return;
    }

    std::cout << std::string(60, '-') << "\n"
              << "  SUPPRIMER UNE MESURE\n"
              << std::string(60, '-') << "\n";

    station.listAll();

    int index;
    std::cout << "\n  Entrez le numero de la mesure a supprimer (1 a "
              << station.getCount() << "): ";
    std::cin >> index;
    std::cin.ignore();

    index--; // Convertir en index 0-based

    if (station.removeMeasurement(index)) {
        std::cout << "\n  Mesure supprimee.\n"
                  << "  Total: " << station.getCount() << " mesure(s) restante(s).\n";
    } else {
        std::cout << "\n  Numero invalide.\n";
    }
}

void displayAnalysisMenu(WeatherStation& station) {
    const auto& measurements = station.getMeasurements();

    if (measurements.empty()) {
        std::cout << "  Aucune mesure disponible pour l'analyse.\n";
        return;
    }

    std::cout << std::string(60, '=') << "\n"
              << "  ANALYSES STATISTIQUES\n"
              << std::string(60, '=') << "\n";

    std::cout << std::fixed << std::setprecision(2);

    float avgTemp = Analyzer::averageTemperature(measurements);
    float minTemp = Analyzer::minTemperature(measurements);
    float maxTemp = Analyzer::maxTemperature(measurements);
    float avgHumidity = Analyzer::averageHumidity(measurements);
    float avgWind = Analyzer::averageWindSpeed(measurements);
    std::string trend = Analyzer::detectTemperatureTrend(measurements);

    std::cout << "\n  TEMPERATURE:\n"
              << "      - Moyenne: " << avgTemp << "°C\n"
              << "      - Minimum: " << minTemp << "°C\n"
              << "      - Maximum: " << maxTemp << "°C\n"
              << "      - Tendance (3 dernieres): " << trend << "\n";

    std::cout << "\n  HUMIDITE:\n"
              << "      - Moyenne: " << avgHumidity << "%\n";

    std::cout << "\n  VENT:\n"
              << "      - Vitesse moyenne: " << avgWind << " km/h\n";

    std::cout << "\n" << std::string(60, '=') << "\n";
}
