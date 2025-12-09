#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H

#include "Measurement.h"
#include <vector>
#include <string>


class WeatherStation {
private:
    std::vector<Measurement> measurements;

public:

    WeatherStation();


    void addMeasurement(const Measurement& measurement);


    bool removeMeasurement(int index);


    const std::vector<Measurement>& getMeasurements() const;


    int getCount() const;


    void listAll() const;


    bool saveToFile(const std::string& path);


    bool loadFromFile(const std::string& path);


    void clear();
};

#endif // WEATHER_STATION_H
