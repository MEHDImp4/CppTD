#include "../include/Analyzer.h"
#include <algorithm>
#include <iostream>


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


std::string Analyzer::detectTemperatureTrend(const std::vector<Measurement>& measurements) {
    if (measurements.size() < 3) {
        return "donnees insuffisantes";
    }



    size_t size = measurements.size();
    float temp1 = measurements[size - 3].getTemperature();
    float temp2 = measurements[size - 2].getTemperature();
    float temp3 = measurements[size - 1].getTemperature();


    float change1 = temp2 - temp1;
    float change2 = temp3 - temp2;

    const float epsilon = 0.5f;


    if (change1 > epsilon && change2 > epsilon) {
        return "hausse";
    } else if (change1 < -epsilon && change2 < -epsilon) {
        return "baisse";
    } else {
        return "stable";
    }
}
