#include "Measurement.h"
#include <iostream>
#include <sstream>

Measurement::Measurement() {
    id = 0;
    temperature = 0.0f;
    humidity = 0.0f;
    windSpeed = 0.0f;
    date = "";
}

Measurement::Measurement(int id, float temp, float hum, float wind, std::string d) {
    this->id = id;
    this->temperature = temp;
    this->humidity = hum;
    this->windSpeed = wind;
    this->date = d;
}

int Measurement::getId() const { return id; }
float Measurement::getTemperature() const { return temperature; }
float Measurement::getHumidity() const { return humidity; }
float Measurement::getWindSpeed() const { return windSpeed; }
std::string Measurement::getDate() const { return date; }

void Measurement::setId(int id) { this->id = id; }
void Measurement::setTemperature(float temp) { this->temperature = temp; }
void Measurement::setHumidity(float hum) { this->humidity = hum; }
void Measurement::setWindSpeed(float wind) { this->windSpeed = wind; }
void Measurement::setDate(std::string d) { this->date = d; }

void Measurement::display() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Temperature: " << temperature << " C" << std::endl;
    std::cout << "Humidity: " << humidity << " %" << std::endl;
    std::cout << "Wind Speed: " << windSpeed << " km/h" << std::endl;
    std::cout << "Date: " << date << std::endl;
    std::cout << "------------------------" << std::endl;
}

std::string Measurement::toTextLine() const {
    std::stringstream ss;
    ss << id << ";" << temperature << ";" << humidity << ";" << windSpeed << ";" << date;
    return ss.str();
}
