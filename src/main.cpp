#include <iostream>
#include "Measurement.h"
#include "WeatherStation.h"
#include "Analyzer.h"

using namespace std;

void displayMenu() {
    cout << endl;
    cout << "=== Weather Station Menu ===" << endl;
    cout << "1. Add measurement" << endl;
    cout << "2. Display measurements" << endl;
    cout << "3. Delete measurement" << endl;
    cout << "4. Load from file" << endl;
    cout << "5. Save to file" << endl;
    cout << "6. Show statistics" << endl;
    cout << "7. Quit" << endl;
    cout << "Choice: ";
}

int main() {
    WeatherStation station;
    string dataFile = "data/measurements.txt";
    int choice = 0;
    int nextId = 1;

    while (choice != 7) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                float temp, hum, wind;
                string date, time;

                cout << "Temperature (C): ";
                cin >> temp;
                cout << "Humidity (%): ";
                cin >> hum;
                cout << "Wind Speed (km/h): ";
                cin >> wind;
                cout << "Date (DD/MM/YYYY): ";
                cin >> date;
                cout << "Time (HH:MM): ";
                cin >> time;

                Measurement m(nextId, temp, hum, wind, date, time);
                station.addMeasurement(m);
                nextId++;

                cout << "Measurement added." << endl;
                break;
            }
            case 2: {
                cout << endl << "=== All Measurements ===" << endl;
                station.displayAll();
                break;
            }
            case 3: {
                int id;
                cout << "Enter ID to delete: ";
                cin >> id;

                if (station.removeMeasurement(id)) {
                    cout << "Measurement deleted." << endl;
                } else {
                    cout << "ID not found." << endl;
                }
                break;
            }
            case 4: {
                if (station.loadFromFile(dataFile)) {
                    cout << "Data loaded from file." << endl;
                } else {
                    cout << "Error loading file." << endl;
                }
                break;
            }
            case 5: {
                if (station.saveToFile(dataFile)) {
                    cout << "Data saved to file." << endl;
                } else {
                    cout << "Error saving file." << endl;
                }
                break;
            }
            case 6: {
                Analyzer::displayStats(station.getMeasurements());
                break;
            }
            case 7: {
                cout << "Goodbye!" << endl;
                break;
            }
            default: {
                cout << "Invalid choice." << endl;
                break;
            }
        }
    }

    return 0;
}
