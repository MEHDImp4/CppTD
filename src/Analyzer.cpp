#include "../include/Analyzer.h"
#include <algorithm>
#include <iostream>

// Température moyenne
float Analyzer::averageTemperature(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (const auto& m : measurements) {
        sum += m.getTemperature();
    }
    return sum / measurements.size();
}

// Température minimale
float Analyzer::minTemperature(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float minTemp = measurements[0].getTemperature();
    for (const auto& m : measurements) {
        if (m.getTemperature() < minTemp) {
            minTemp = m.getTemperature();
        }
    }
    return minTemp;
}

// Température maximale
float Analyzer::maxTemperature(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float maxTemp = measurements[0].getTemperature();
    for (const auto& m : measurements) {
        if (m.getTemperature() > maxTemp) {
            maxTemp = m.getTemperature();
        }
    }
    return maxTemp;
}

// Humidité moyenne
float Analyzer::averageHumidity(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (const auto& m : measurements) {
        sum += m.getHumidity();
    }
    return sum / measurements.size();
}

// Vitesse du vent moyenne
float Analyzer::averageWindSpeed(const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (const auto& m : measurements) {
        sum += m.getWindSpeed();
    }
    return sum / measurements.size();
}

// Détection de tendance
std::string Analyzer::detectTemperatureTrend(const std::vector<Measurement>& measurements) {
    // Besoin d'au moins 3 mesures pour analyser la tendance
    if (measurements.size() < 3) {
        return "données insuffisantes";
    }

    // Récupère les 3 dernières mesures
    size_t size = measurements.size();
    float temp1 = measurements[size - 3].getTemperature();
    float temp2 = measurements[size - 2].getTemperature();
    float temp3 = measurements[size - 1].getTemperature();

    // Compare les évolutions
    float change1 = temp2 - temp1;  // variation entre mesure 1 et 2
    float change2 = temp3 - temp2;  // variation entre mesure 2 et 3

    const float epsilon = 0.5f;  // Seuil pour considérer comme stable

    // Si les deux changements ont le même signe et sont significatifs
    if (change1 > epsilon && change2 > epsilon) {
        return "hausse";
    } else if (change1 < -epsilon && change2 < -epsilon) {
        return "baisse";
    } else {
        return "stable";
    }
}
