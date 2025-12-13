#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H

#include "Measurement.h"
#include <vector>
#include <string>

// Classe pour gérer toutes les mesures météo
class WeatherStation {
private:
    std::vector<Measurement> measurements; // Stockage des mesures

public:
    // Constructeur
    WeatherStation();

    // Ajoute une mesure
    void addMeasurement(const Measurement& measurement);

    // Supprime une mesure (retourne true si réussi)
    bool removeMeasurement(int index);

    // Retourne toutes les mesures
    const std::vector<Measurement>& getMeasurements() const;

    // Retourne le nombre de mesures
    int getCount() const;

    // Affiche toutes les mesures
    void listAll() const;

    // Sauvegarde les mesures dans un fichier
    bool saveToFile(const std::string& path);

    // Charge les mesures depuis un fichier
    bool loadFromFile(const std::string& path);

    // Efface toutes les mesures
    void clear();
};

#endif // WEATHER_STATION_H
