#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include "WeatherStation.h"
#include "Analyzer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addMeasurement();
    void deleteMeasurement();
    void loadFromFile();
    void saveToFile();
    void showStatistics();
    void refreshTable();

private:
    void setupUI();
    void createInputSection(QVBoxLayout *mainLayout);
    void createTableSection(QVBoxLayout *mainLayout);
    void createButtonSection(QVBoxLayout *mainLayout);
    void createStatsSection(QVBoxLayout *mainLayout);

    // Data
    WeatherStation station;
    int nextId;
    QString dataFile;

    // Input fields
    QLineEdit *temperatureEdit;
    QLineEdit *humidityEdit;
    QLineEdit *windSpeedEdit;

    // Table
    QTableWidget *measurementTable;

    // Stats labels
    QLabel *avgTempLabel;
    QLabel *minTempLabel;
    QLabel *maxTempLabel;
    QLabel *avgHumidityLabel;
    QLabel *avgWindLabel;
};

#endif // MAINWINDOW_H
