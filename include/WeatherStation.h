#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include <vector>
#include <string>
#include "Measurement.h"

class WeatherStation {
private:
    std::vector<Measurement> measurements;

public:
    void addMeasurement(const Measurement& m);
    bool removeMeasurement(int id);
    void displayAll() const;
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    const std::vector<Measurement>& getMeasurements() const;
};

#endif
