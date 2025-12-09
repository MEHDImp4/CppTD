#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>
#include <vector>


class Measurement {
private:
    float temperature;
    float humidity;
    float windSpeed;
    std::string windDirection;
    std::string dateTime;

public:
    Measurement(float temp = 0.0f, float hum = 0.0f, float wind = 0.0f,
                const std::string& dir = "N", const std::string& time = "");


    float getTemperature() const;
    float getHumidity() const;
    float getWindSpeed() const;
    std::string getWindDirection() const;
    std::string getDateTime() const;


    void setTemperature(float temp);
    void setHumidity(float hum);
    void setWindSpeed(float wind);
    void setWindDirection(const std::string& dir);
    void setDateTime(const std::string& time);

    std::string toString() const;

    std::string serialize() const;

    static Measurement deserialize(const std::string& line);
};

#endif // MEASUREMENT_H
