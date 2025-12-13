#include "../include/WeatherStation.h"
#include <iostream>
#include <fstream>
#include <iomanip>

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
        std::cout << "\n  Aucune mesure stockee.\n";
        return;
    }

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  LISTE DE TOUTES LES MESURES (" << measurements.size() << " total)\n";
    std::cout << std::string(60, '=') << "\n";

    for (size_t i = 0; i < measurements.size(); ++i) {
        std::cout << "\n  [Mesure #" << (i + 1) << "]\n";
        std::cout << measurements[i].toString() << "\n";
    }

    std::cout << "\n" << std::string(60, '=') << "\n";
}

// Sauvegarder en fichier
bool WeatherStation::saveToFile(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "  Erreur: impossible d'ouvrir " << path << " en ecriture.\n";
        return false;
    }

    for (const auto& measurement : measurements) {
        file << measurement.serialize() << "\n";
    }

    file.close();
    std::cout << "  " << measurements.size() << " mesure(s) sauvegardee(s) dans "
              << path << "\n";
    return true;
}

// Charger depuis fichier
bool WeatherStation::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "  Fichier " << path << " introuvable.\n";
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
    std::cout << "  " << count << " mesure(s) chargee(s) depuis " << path << "\n";
    return true;
}

// Effacer toutes les mesures
void WeatherStation::clear() {
    measurements.clear();
}
