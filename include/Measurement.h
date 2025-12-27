#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>

class Measurement {
private:
    int id;
    float temperature;
    float humidity;
    float windSpeed;
    std::string date;

public:
    Measurement();
    Measurement(int id, float temp, float hum, float wind, std::string d);

    int getId() const;
    float getTemperature() const;
    float getHumidity() const;
    float getWindSpeed() const;
    std::string getDate() const;

    void setId(int id);
    void setTemperature(float temp);
    void setHumidity(float hum);
    void setWindSpeed(float wind);
    void setDate(std::string d);

    void display() const;
    std::string toTextLine() const;
};

#endif
