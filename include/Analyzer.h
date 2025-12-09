#ifndef ANALYZER_H
#define ANALYZER_H

#include "Measurement.h"
#include <vector>
#include <string>


class Analyzer {
public:

    static float averageTemperature(const std::vector<Measurement>& measurements);


    static float minTemperature(const std::vector<Measurement>& measurements);


    static float maxTemperature(const std::vector<Measurement>& measurements);


    static float averageHumidity(const std::vector<Measurement>& measurements);


    static float averageWindSpeed(const std::vector<Measurement>& measurements);


    static std::string detectTemperatureTrend(const std::vector<Measurement>& measurements);
};

#endif // ANALYZER_H
