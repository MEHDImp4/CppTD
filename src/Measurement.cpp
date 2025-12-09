#include "../include/Measurement.h"
#include <sstream>
#include <iomanip>


Measurement::Measurement(float temp, float hum, float wind,
                         const std::string& dir, const std::string& time)
    : temperature(temp), humidity(hum), windSpeed(wind),
      windDirection(dir), dateTime(time) {}


float Measurement::getTemperature() const {
    return temperature;
}

float Measurement::getHumidity() const {
    return humidity;
}

float Measurement::getWindSpeed() const {
    return windSpeed;
}

std::string Measurement::getWindDirection() const {
    return windDirection;
}

std::string Measurement::getDateTime() const {
    return dateTime;
}


void Measurement::setTemperature(float temp) {
    temperature = temp;
}

void Measurement::setHumidity(float hum) {
    humidity = hum;
}

void Measurement::setWindSpeed(float wind) {
    windSpeed = wind;
}

void Measurement::setWindDirection(const std::string& dir) {
    windDirection = dir;
}

void Measurement::setDateTime(const std::string& time) {
    dateTime = time;
}


std::string Measurement::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1)
        << "  Date/Heure: " << dateTime << "\n"
        << "  Temperature: " << temperature << " C\n"
        << "  Humidite: " << humidity << "%\n"
        << "  Vitesse du vent: " << windSpeed << " km/h\n"
        << "  Direction du vent: " << windDirection;
    return oss.str();
}


std::string Measurement::serialize() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2)
        << temperature << "|" << humidity << "|" << windSpeed << "|"
        << windDirection << "|" << dateTime;
    return oss.str();
}


Measurement Measurement::deserialize(const std::string& line) {

    if (line.empty() || line[0] == '#') {
        return Measurement();
    }

    std::istringstream iss(line);
    float temp = 0.0f, hum = 0.0f, wind = 0.0f;
    std::string dir = "N", time = "";
    char pipe;


    if (iss >> temp >> pipe >> hum >> pipe >> wind >> pipe) {
        std::getline(iss, dir, '|');
        std::getline(iss, time);
    }

    return Measurement(temp, hum, wind, dir, time);
}
