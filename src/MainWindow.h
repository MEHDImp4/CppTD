#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include "../include/WeatherStation.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onAnalyzeClicked();
    void onSaveClicked();
    void onLoadClicked();

private:
    void setupUi();
    void refreshTable();
    void showStatus(const QString &message);
    QString getDataFilePath();

    QTableWidget *tableWidget;
    WeatherStation station;
    QString currentDataFile;
};

#endif // MAINWINDOW_H
