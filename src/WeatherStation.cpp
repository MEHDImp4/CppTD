#include "../include/WeatherStation.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "../include/ConsoleUtf8.h"

// Constructeur
WeatherStation::WeatherStation() {}

// Ajouter une mesure
void WeatherStation::addMeasurement(const Measurement& measurement) {
    measurements.push_back(measurement);
}

// Supprimer une mesure
bool WeatherStation::removeMeasurement(int index) {
    if (index < 0 || index >= static_cast<int>(measurements.size())) {
        return false;
    }
    measurements.erase(measurements.begin() + index);
    return true;
}

// Obtenir la liste des mesures
const std::vector<Measurement>& WeatherStation::getMeasurements() const {
    return measurements;
}

// Obtenir le nombre de mesures
int WeatherStation::getCount() const {
    return measurements.size();
}

// Afficher toutes les mesures
void WeatherStation::listAll() const {
    if (measurements.empty()) {
        ConsoleUtf8::writeLine("\n  Aucune mesure stockee.");
        return;
    }

    ConsoleUtf8::write("\n");
    ConsoleUtf8::write(std::string(60, '=') + "\n");
    ConsoleUtf8::write("  LISTE DE TOUTES LES MESURES (" + std::to_string(measurements.size()) + " total)\n");
    ConsoleUtf8::write(std::string(60, '=') + "\n");

    for (size_t i = 0; i < measurements.size(); ++i) {
        ConsoleUtf8::write("\n  [Mesure #" + std::to_string(i + 1) + "]\n");
        ConsoleUtf8::write(measurements[i].toString() + "\n");
    }

    ConsoleUtf8::write("\n" + std::string(60, '=') + "\n");
}

// Sauvegarder en fichier
bool WeatherStation::saveToFile(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        ConsoleUtf8::writeLine("  Erreur: impossible d'ouvrir " + path + " en ecriture.");
        return false;
    }

    for (const auto& measurement : measurements) {
        file << measurement.serialize() << "\n";
    }

    file.close();
    ConsoleUtf8::writeLine("  " + std::to_string(measurements.size()) + " mesure(s) sauvegardee(s) dans " + path);
    return true;
}

// Charger depuis fichier
bool WeatherStation::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        ConsoleUtf8::writeLine("  Fichier " + path + " introuvable.");
        return false;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            measurements.push_back(Measurement::deserialize(line));
            count++;
        }
    }

    file.close();
    ConsoleUtf8::writeLine("  " + std::to_string(count) + " mesure(s) chargee(s) depuis " + path);
    return true;
}

// Effacer toutes les mesures
void WeatherStation::clear() {
    measurements.clear();
}
