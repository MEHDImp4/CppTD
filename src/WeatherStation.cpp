#include "WeatherStation.h"
#include <iostream>
#include <fstream>
#include <sstream>

void WeatherStation::addMeasurement(const Measurement& m) {
    measurements.push_back(m);
}

bool WeatherStation::removeMeasurement(int id) {
    for (size_t i = 0; i < measurements.size(); i++) {
        if (measurements[i].getId() == id) {
            measurements.erase(measurements.begin() + i);
            return true;
        }
    }
    return false;
}

void WeatherStation::displayAll() const {
    if (measurements.empty()) {
        std::cout << "No measurements available." << std::endl;
        return;
    }
    for (size_t i = 0; i < measurements.size(); i++) {
        measurements[i].display();
    }
}

bool WeatherStation::loadFromFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        return false;
    }

    measurements.clear();
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        int id;
        float temp, hum, wind;
        std::string date;

        std::getline(ss, token, ';');
        id = atoi(token.c_str());

        std::getline(ss, token, ';');
        temp = atof(token.c_str());

        std::getline(ss, token, ';');
        hum = atof(token.c_str());

        std::getline(ss, token, ';');
        wind = atof(token.c_str());

        std::getline(ss, date, ';');

        Measurement m(id, temp, hum, wind, date);
        measurements.push_back(m);
    }

    file.close();
    return true;
}

bool WeatherStation::saveToFile(const std::string& filename) const {
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        return false;
    }

    for (size_t i = 0; i < measurements.size(); i++) {
        file << measurements[i].toTextLine() << std::endl;
    }

    file.close();
    return true;
}

const std::vector<Measurement>& WeatherStation::getMeasurements() const {
    return measurements;
}
