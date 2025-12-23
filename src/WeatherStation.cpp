#include "../include/WeatherStation.h"
#include <iostream>
#include <fstream>
#include <iomanip>

WeatherStation::WeatherStation() {}


void WeatherStation::addMeasurement(const Measurement& measurement) {
    measurements.push_back(measurement);
}


bool WeatherStation::removeMeasurement(int index) {
    if (index < 0 || index >= static_cast<int>(measurements.size())) {
        return false;
    }
    measurements.erase(measurements.begin() + index);
    return true;
}


const std::vector<Measurement>& WeatherStation::getMeasurements() const {
    return measurements;
}


int WeatherStation::getCount() const {
    return measurements.size();
}


void WeatherStation::listAll() const {
    if (measurements.empty()) {
        std::cout << "\n  Aucune mesure stockee." << std::endl;
        return;
    }

    std::cout << "\n";
    std::cout << std::string(60, '=') + "\n";
    std::cout << "  LISTE DE TOUTES LES MESURES (" + std::to_string(measurements.size()) + " total)\n";
    std::cout << std::string(60, '=') + "\n";

    for (size_t i = 0; i < measurements.size(); ++i) {
        std::cout << "\n  [Mesure #" + std::to_string(i + 1) + "]\n";
        std::cout << measurements[i].toString() + "\n";
    }

    std::cout << "\n" + std::string(60, '=') + "\n";
}


bool WeatherStation::saveToFile(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cout << "  Erreur: impossible d'ouvrir " + path + " en ecriture." << std::endl;
        return false;
    }

    for (const auto& measurement : measurements) {
        file << measurement.serialize() << "\n";
    }

    file.close();
    std::cout << "  " + std::to_string(measurements.size()) + " mesure(s) sauvegardee(s) dans " + path << std::endl;
    return true;
}


bool WeatherStation::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "  Fichier " + path + " introuvable." << std::endl;
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
    std::cout << "  " + std::to_string(count) + " mesure(s) chargee(s) depuis " + path << std::endl;
    return true;
}


void WeatherStation::clear() {
    measurements.clear();
}
