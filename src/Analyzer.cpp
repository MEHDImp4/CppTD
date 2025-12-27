#include "Analyzer.h"
#include <iostream>

float Analyzer::averageTemperature(const std::vector<Measurement>& data) {
    if (data.empty()) return 0.0f;
    float sum = 0.0f;
    for (size_t i = 0; i < data.size(); i++) {
        sum += data[i].getTemperature();
    }
    return sum / data.size();
}

float Analyzer::minTemperature(const std::vector<Measurement>& data) {
    if (data.empty()) return 0.0f;
    float min = data[0].getTemperature();
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i].getTemperature() < min) {
            min = data[i].getTemperature();
        }
    }
    return min;
}

float Analyzer::maxTemperature(const std::vector<Measurement>& data) {
    if (data.empty()) return 0.0f;
    float max = data[0].getTemperature();
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i].getTemperature() > max) {
            max = data[i].getTemperature();
        }
    }
    return max;
}

float Analyzer::averageHumidity(const std::vector<Measurement>& data) {
    if (data.empty()) return 0.0f;
    float sum = 0.0f;
    for (size_t i = 0; i < data.size(); i++) {
        sum += data[i].getHumidity();
    }
    return sum / data.size();
}

float Analyzer::averageWindSpeed(const std::vector<Measurement>& data) {
    if (data.empty()) return 0.0f;
    float sum = 0.0f;
    for (size_t i = 0; i < data.size(); i++) {
        sum += data[i].getWindSpeed();
    }
    return sum / data.size();
}

void Analyzer::displayStats(const std::vector<Measurement>& data) {
    if (data.empty()) {
        std::cout << "No data available for analysis." << std::endl;
        return;
    }

    std::cout << "=== Statistics ===" << std::endl;
    std::cout << "Average Temperature: " << averageTemperature(data) << " C" << std::endl;
    std::cout << "Min Temperature: " << minTemperature(data) << " C" << std::endl;
    std::cout << "Max Temperature: " << maxTemperature(data) << " C" << std::endl;
    std::cout << "Average Humidity: " << averageHumidity(data) << " %" << std::endl;
    std::cout << "Average Wind Speed: " << averageWindSpeed(data) << " km/h" << std::endl;
}
