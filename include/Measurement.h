#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>
#include <vector>

// Classe pour stocker une mesure météo
class Measurement {
private:
    float temperature;      // Température en °C
    float humidity;         // Humidité en %
    float windSpeed;        // Vitesse du vent en km/h
    std::string windDirection; // Direction du vent (N, S, E, O, etc.)
    std::string dateTime;   // Date et heure de la mesure

public:
    // Constructeur avec paramètres par défaut
    Measurement(float temp = 0.0f, float hum = 0.0f, float wind = 0.0f,
                const std::string& dir = "N", const std::string& time = "");

    // Getters
    float getTemperature() const;
    float getHumidity() const;
    float getWindSpeed() const;
    std::string getWindDirection() const;
    std::string getDateTime() const;

    // Setters
    void setTemperature(float temp);
    void setHumidity(float hum);
    void setWindSpeed(float wind);
    void setWindDirection(const std::string& dir);
    void setDateTime(const std::string& time);

    // Retourne une représentation lisible de la mesure
    std::string toString() const;

    // Convertit la mesure en une ligne de texte pour la sauvegarde (format: temp|humidity|windSpeed|windDirection|dateTime)
    std::string serialize() const;

    // Crée une mesure à partir d'une ligne de texte
    static Measurement deserialize(const std::string& line);
};

#endif // MEASUREMENT_H
