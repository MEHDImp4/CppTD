#ifndef ANALYZER_H
#define ANALYZER_H

#include "Measurement.h"
#include <vector>
#include <string>

// Classe pour analyser les mesures météo (statistiques)
class Analyzer {
public:
    // Calcule la température moyenne
    static float averageTemperature(const std::vector<Measurement>& measurements);

    // Trouve la température minimale
    static float minTemperature(const std::vector<Measurement>& measurements);

    // Trouve la température maximale
    static float maxTemperature(const std::vector<Measurement>& measurements);

    // Calcule l'humidité moyenne
    static float averageHumidity(const std::vector<Measurement>& measurements);

    // Calcule la vitesse du vent moyenne
    static float averageWindSpeed(const std::vector<Measurement>& measurements);

    // Détecte la tendance de température (sur les 3 dernières mesures)
    // Retourne "hausse", "baisse", "stable" ou "données insuffisantes"
    static std::string detectTemperatureTrend(const std::vector<Measurement>& measurements);
};

#endif // ANALYZER_H
