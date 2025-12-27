#include "MainWindow.h"
#include "AddMeasurementDialog.h"
#include "../include/Analyzer.h"
#include <QStatusBar>
#include <QToolButton>
#include <QIcon>
#include <QSize>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <iomanip>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    
    // Locate data file logic
    currentDataFile = getDataFilePath();
    
    // Load data
    if (QFile::exists(currentDataFile)) {
        if (station.loadFromFile(currentDataFile.toStdString())) {
            showStatus("Donnees chargees avec succes.");
        } else {
            showStatus("Erreur lors du chargement des donnees.");
        }
    } else {
        showStatus("Nouveau fichier de donnees sera cree.");
    }

    refreshTable();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Station Meteorologique");
    resize(800, 600);

    auto *mainLayout = new QVBoxLayout(centralWidget);

    // Toolbar
    auto *toolbarLayout = new QHBoxLayout();
    
    auto *btnAdd = new QToolButton();
    btnAdd->setText("Ajouter");
    btnAdd->setIcon(QIcon(":/icons/resources/icons/add.svg"));
    btnAdd->setIconSize(QSize(20,20));
    btnAdd->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    auto *btnDelete = new QToolButton();
    btnDelete->setText("Supprimer");
    btnDelete->setIcon(QIcon(":/icons/resources/icons/delete.svg"));
    btnDelete->setIconSize(QSize(20,20));
    btnDelete->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    auto *btnAnalyze = new QToolButton();
    btnAnalyze->setText("Analyses");
    btnAnalyze->setIcon(QIcon(":/icons/resources/icons/analyze.svg"));
    btnAnalyze->setIconSize(QSize(20,20));
    btnAnalyze->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    auto *btnSave = new QToolButton();
    btnSave->setText("Sauvegarder");
    btnSave->setIcon(QIcon(":/icons/resources/icons/save.svg"));
    btnSave->setIconSize(QSize(20,20));
    btnSave->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    auto *btnLoad = new QToolButton();
    btnLoad->setText("Recharger");
    btnLoad->setIcon(QIcon(":/icons/resources/icons/load.svg"));
    btnLoad->setIconSize(QSize(20,20));
    btnLoad->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // Mark toolbar buttons so stylesheet can target them
    btnAdd->setProperty("role", "toolbar");
    btnDelete->setProperty("role", "toolbar");
    btnAnalyze->setProperty("role", "toolbar");
    btnSave->setProperty("role", "toolbar");
    btnLoad->setProperty("role", "toolbar");

    // Slight UI tweaks
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnAnalyze->setCursor(Qt::PointingHandCursor);
    btnSave->setCursor(Qt::PointingHandCursor);
    btnLoad->setCursor(Qt::PointingHandCursor);

    toolbarLayout->addWidget(btnAdd);
    toolbarLayout->addWidget(btnDelete);
    toolbarLayout->addWidget(btnAnalyze);
    toolbarLayout->addWidget(btnSave);
    toolbarLayout->addWidget(btnLoad);
    toolbarLayout->addStretch();

    mainLayout->addLayout(toolbarLayout);

    // Table
    tableWidget = new QTableWidget();
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels({"Date/Heure", "Temperature (C)", "Humidite (%)", "Vent (km/h)", "Direction"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setShowGrid(false);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->verticalHeader()->setVisible(false);
    
    mainLayout->addWidget(tableWidget);

    // Connections
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(btnAnalyze, &QPushButton::clicked, this, &MainWindow::onAnalyzeClicked);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadClicked);
}

void MainWindow::refreshTable() {
    tableWidget->setRowCount(0);
    const auto &measurements = station.getMeasurements();
    
    for (const auto &m : measurements) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(m.getDateTime())));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(m.getTemperature(), 'f', 1)));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(m.getHumidity(), 'f', 1)));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(m.getWindSpeed(), 'f', 1)));
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(m.getWindDirection())));
    }
}

void MainWindow::onAddClicked() {
    AddMeasurementDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        station.addMeasurement(dialog.getMeasurement());
        refreshTable();
        showStatus("Mesure ajoutee.");
    }
}

void MainWindow::onDeleteClicked() {
    int row = tableWidget->currentRow();
    if (row >= 0) {
        // Warning: removing by index relies on table order matching station vector order
        // Since we refresh table from vector, they should match.
        station.removeMeasurement(row);
        refreshTable();
        showStatus("Mesure supprimee.");
    } else {
        QMessageBox::warning(this, "Attention", "Veuillez selectionner une ligne a supprimer.");
    }
}

void MainWindow::onAnalyzeClicked() {
    const auto& measurements = station.getMeasurements();
    if (measurements.empty()) {
        QMessageBox::information(this, "Analyse", "Aucune donnee a analyser.");
        return;
    }

    float avgTemp = Analyzer::averageTemperature(measurements);
    float minTemp = Analyzer::minTemperature(measurements);
    float maxTemp = Analyzer::maxTemperature(measurements);
    float avgHumidity = Analyzer::averageHumidity(measurements);
    float avgWind = Analyzer::averageWindSpeed(measurements);
    std::string trend = Analyzer::detectTemperatureTrend(measurements);

    std::ostringstream oss;
    oss << "TEMPERATURE:\n"
        << "  Moyenne: " << std::fixed << std::setprecision(2) << avgTemp << " C\n"
        << "  Minimum: " << minTemp << " C\n"
        << "  Maximum: " << maxTemp << " C\n"
        << "  Tendance: " << trend << "\n\n"
        << "HUMIDITE:\n"
        << "  Moyenne: " << avgHumidity << " %\n\n"
        << "VENT:\n"
        << "  Moyenne: " << avgWind << " km/h";

    QMessageBox::information(this, "Statistiques", QString::fromStdString(oss.str()));
}

void MainWindow::onSaveClicked() {
    if (station.saveToFile(currentDataFile.toStdString())) {
        showStatus("Sauvegarde reussie vers " + currentDataFile);
        QMessageBox::information(this, "Sauvegarde", "Donnees sauvegardees avec succes.");
    } else {
        QMessageBox::critical(this, "Erreur", "Echec de la sauvegarde.");
    }
}

void MainWindow::onLoadClicked() {
    if (station.loadFromFile(currentDataFile.toStdString())) {
        refreshTable();
        showStatus("Donnees rechargees.");
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger le fichier.");
    }
}

void MainWindow::showStatus(const QString &message) {
    statusBar()->showMessage(message, 3000);
}

QString MainWindow::getDataFilePath() {
    // Mimic the main.cpp logic or use standard paths
    // For now, let's look for data/measurements.txt relative to cwd
    
    QDir dir = QDir::current();
    if (dir.exists("data/measurements.txt")) {
        return dir.filePath("data/measurements.txt");
    }
    
    // Also check relative to executable if possible, but for simplicity
    // we default to where the old app looked roughly or a standard location.
    // If not found, default to creating it in data/
    
    QString p = "data/measurements.txt";
    QFileInfo fi(p);
    if (!fi.dir().exists()) {
        fi.dir().mkpath(".");
    }
    return p;
}
