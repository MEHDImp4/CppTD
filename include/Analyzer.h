#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include "Measurement.h"

class Analyzer {
public:
    static float averageTemperature(const std::vector<Measurement>& data);
    static float minTemperature(const std::vector<Measurement>& data);
    static float maxTemperature(const std::vector<Measurement>& data);
    static float averageHumidity(const std::vector<Measurement>& data);
    static float averageWindSpeed(const std::vector<Measurement>& data);
    static void displayStats(const std::vector<Measurement>& data);
};

#endif
