#include "../include/WeatherStation.h"
#include "../include/Analyzer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <filesystem>
#include <sstream>


using namespace std;
namespace fs = std::filesystem;

void displayMenu();
string getCurrentDateTime();
string locateDataFile(int argc, char* argv[]);
void addMeasurementInteractive(WeatherStation& station);
void removeMeasurementInteractive(WeatherStation& station);
void displayAnalysisMenu(WeatherStation& station);

string locateDataFile(int argc, char* argv[]) {
    vector<fs::path> candidates;
    candidates.push_back(fs::current_path() / "data/measurements.txt");
    candidates.push_back(fs::current_path() / "src" / "data" / "measurements.txt");

    if (argc > 0 && argv[0] != nullptr) {
        try {
            fs::path exe = fs::absolute(argv[0]);
            fs::path exeDir = exe.parent_path();
            candidates.push_back(exeDir / "data" / "measurements.txt");
            candidates.push_back(exeDir.parent_path() / "data" / "measurements.txt");
        } catch (...) {

        }
    }

    candidates.push_back(fs::current_path().parent_path() / "data" / "measurements.txt");

    for (auto &p : candidates) {
        if (!p.empty() && fs::exists(p)) {
            return p.string();
        }
    }

    return string("data/measurements.txt");
}

int main(int argc, char* argv[]) {
    WeatherStation station;
    string dataFile = locateDataFile(argc, argv);

    try {
        fs::path p(dataFile);
        fs::path dir = p.parent_path();
        if (!dir.empty() && !fs::exists(dir)) {
            fs::create_directories(dir);
        }

        if (!fs::exists(p)) {
            ofstream ofs(p);
            ofs.close();
        }
    } catch (const exception& e) {
        cerr << "Erreur lors de la creation du fichier/dossier: " << e.what() << endl;
    }



    cout << "\n===========================================\n   STATION METEOROLOGIQUE\n===========================================\n";

    station.loadFromFile(dataFile);

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cout << "\n  Entrez votre choix (1-8): ";
        cin >> choice;
        cin.ignore();

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
                cout << "  Chargement en cours..." << endl;
                if (station.loadFromFile(dataFile)) {
                    cout << "  Chargement reussi." << endl;
                }
                break;
            }
            case 5: {
                cout << "  Sauvegarde en cours..." << endl;
                if (station.saveToFile(dataFile)) {
                    cout << "  Sauvegarde reussie." << endl;
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
                    cout << "  Toutes les donnees ont ete effacees." << endl;
                }
                break;
            }
            case 8: {
                cout << "  Au revoir!" << endl;
                running = false;
                break;
            }
            default: {
                cout << "  Choix invalide." << endl;
            }
        }

        if (running && choice >= 1 && choice <= 7) {
            cout << "\n  Appuyez sur Entree pour continuer...";
            cin.get();
        }
    }


    station.saveToFile(dataFile);
    return 0;
}

void displayMenu() {
    cout << "\n";
    cout << std::string(60, '=') + "\n";
    cout << "  MENU PRINCIPAL\n";
    cout << std::string(60, '=') + "\n";
    cout <<
        "  1. Ajouter une nouvelle mesure\n"
        "  2. Afficher toutes les mesures\n"
        "  3. Supprimer une mesure\n"
        "  4. Charger les mesures depuis fichier\n"
        "  5. Sauvegarder les mesures\n"
        "  6. Afficher les analyses\n"
        "  7. Effacer toutes les donnees\n"
        "  8. Quitter\n";
    cout << std::string(60, '=');
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

    cout << "  Temperature (C): ";
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

    cout << "\n  Mesure ajoutee avec succes!" << endl;
    cout << "  Total: " + std::to_string(station.getCount()) + " mesure(s)." << endl;
}

void removeMeasurementInteractive(WeatherStation& station) {
    if (station.getCount() == 0) {
        cout << "  Aucune mesure a supprimer." << endl;
        return;
    }

    cout << std::string(60, '-') + "\n";
    cout << "  SUPPRIMER UNE MESURE" << endl;
    cout << std::string(60, '-') + "\n";

    station.listAll();

    int index;
    cout << "\n  Entrez le numero de la mesure a supprimer (1 a " + std::to_string(station.getCount()) + "): ";
    cin >> index;
    cin.ignore();

    index--;

    if (station.removeMeasurement(index)) {
        cout << "\n  Mesure supprimee." << endl;
        cout << "  Total: " + std::to_string(station.getCount()) + " mesure(s) restante(s)." << endl;
    } else {
        cout << "\n  Numero invalide." << endl;
    }
}

void displayAnalysisMenu(WeatherStation& station) {
    const auto& measurements = station.getMeasurements();

        if (measurements.empty()) {
           cout << "  Aucune mesure disponible pour l'analyse." << endl;
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
        oss << "      - Moyenne: " << avgTemp << " C\n";
        oss << "      - Minimum: " << minTemp << " C\n";
        oss << "      - Maximum: " << maxTemp << " C\n";
        oss << "      - Tendance (3 dernieres): " << trend << "\n";

        oss << "\n  HUMIDITE:\n";
        oss << "      - Moyenne: " << avgHumidity << "%\n";

        oss << "\n  VENT:\n";
        oss << "      - Vitesse moyenne: " << avgWind << " km/h\n";

        oss << "\n" << std::string(60, '=') << "\n";

        cout << oss.str();
}
