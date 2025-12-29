#include "MainWindow.h"
#include <QHeaderView>
#include <QGridLayout>
#include <QSizePolicy>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), nextId(1), dataFile("data/measurements.txt")
{
    setupUI();
    setWindowTitle("Weather Station");
    setMinimumSize(800, 800);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    createInputSection(mainLayout);
    createTableSection(mainLayout);
    createButtonSection(mainLayout);
    createStatsSection(mainLayout);
}

void MainWindow::createInputSection(QVBoxLayout *mainLayout) {
    QGroupBox *inputGroup = new QGroupBox("Add New Measurement");
    QGridLayout *gridLayout = new QGridLayout(inputGroup);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(3, 1);
    gridLayout->setSpacing(15);

    // Row 0: Temperature & Humidity
    gridLayout->addWidget(new QLabel("Temperature (°C):"), 0, 0);
    temperatureEdit = new QLineEdit();
    temperatureEdit->setPlaceholderText("e.g. 22.5");
    gridLayout->addWidget(temperatureEdit, 0, 1);

    gridLayout->addWidget(new QLabel("Humidity (%):"), 0, 2);
    humidityEdit = new QLineEdit();
    humidityEdit->setPlaceholderText("e.g. 65");
    gridLayout->addWidget(humidityEdit, 0, 3);

    // Row 1: Wind Speed
    gridLayout->addWidget(new QLabel("Wind Speed (km/h):"), 1, 0);
    windSpeedEdit = new QLineEdit();
    windSpeedEdit->setPlaceholderText("e.g. 15");
    gridLayout->addWidget(windSpeedEdit, 1, 1);

    // Add visual indicator for auto-date time
    QLabel *autoTimeLabel = new QLabel("Date & Time will be set automatically");
    autoTimeLabel->setStyleSheet("color: #ffffffff; font-style: italic;");
    gridLayout->addWidget(autoTimeLabel, 1, 2, 1, 2);

    // Row 2: Add Button (Spanning all columns)
    QPushButton *addButton = new QPushButton("Add Measurement");
    addButton->setObjectName("addBtn");
    addButton->setCursor(Qt::PointingHandCursor);
    addButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addMeasurement);
    gridLayout->addWidget(addButton, 2, 0, 1, 4);

    mainLayout->addWidget(inputGroup);
}

void MainWindow::createTableSection(QVBoxLayout *mainLayout) {
    QGroupBox *tableGroup = new QGroupBox("Measurements");
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);

    measurementTable = new QTableWidget();
    measurementTable->setColumnCount(6);
    measurementTable->setHorizontalHeaderLabels({"ID", "Temperature (°C)", "Humidity (%)", "Wind (km/h)", "Date", "Time"});
    measurementTable->horizontalHeader()->setStretchLastSection(true);
    measurementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    measurementTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    measurementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    measurementTable->setAlternatingRowColors(true);
    measurementTable->verticalHeader()->setVisible(false);

    tableLayout->addWidget(measurementTable);
    mainLayout->addWidget(tableGroup, 1);
}

void MainWindow::createButtonSection(QVBoxLayout *mainLayout) {
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    auto setupButton = [&](const QString &text, const QString &objName, auto slot) {
        QPushButton *btn = new QPushButton(text);
        btn->setObjectName(objName);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        connect(btn, &QPushButton::clicked, this, slot);
        return btn;
    };

    buttonLayout->addWidget(setupButton("Delete Selected", "deleteBtn", &MainWindow::deleteMeasurement));
    buttonLayout->addWidget(setupButton("Load from File", "loadBtn", &MainWindow::loadFromFile));
    buttonLayout->addWidget(setupButton("Save to File", "saveBtn", &MainWindow::saveToFile));
    buttonLayout->addWidget(setupButton("Refresh Stats", "statsBtn", &MainWindow::showStatistics));

    mainLayout->addLayout(buttonLayout);
}

void MainWindow::createStatsSection(QVBoxLayout *mainLayout) {
    QGroupBox *statsGroup = new QGroupBox("Statistics");
    QGridLayout *statsLayout = new QGridLayout(statsGroup);
    statsLayout->setSpacing(10);

    avgTempLabel = new QLabel("Avg: --");
    avgTempLabel->setObjectName("statLabel");
    
    minTempLabel = new QLabel("Min: --");
    minTempLabel->setObjectName("statLabel");
    
    maxTempLabel = new QLabel("Max: --");
    maxTempLabel->setObjectName("statLabel");
    
    avgHumidityLabel = new QLabel("Humidity: --");
    avgHumidityLabel->setObjectName("statLabel");
    
    avgWindLabel = new QLabel("Wind: --");
    avgWindLabel->setObjectName("statLabel");

    // Responsive grid layout for stats
    statsLayout->addWidget(avgTempLabel, 0, 0);
    statsLayout->addWidget(minTempLabel, 0, 1);
    statsLayout->addWidget(maxTempLabel, 0, 2);
    statsLayout->addWidget(avgHumidityLabel, 1, 0);
    statsLayout->addWidget(avgWindLabel, 1, 1);
    
    // Make the last item span remaining columns if needed or just fit nicely
    statsLayout->addWidget(avgWindLabel, 1, 1, 1, 2);

    mainLayout->addWidget(statsGroup);
}

void MainWindow::addMeasurement() {
    bool tempOk, humOk, windOk;
    float temp = temperatureEdit->text().toFloat(&tempOk);
    float hum = humidityEdit->text().toFloat(&humOk);
    float wind = windSpeedEdit->text().toFloat(&windOk);
    
    // Auto-generate date and time from system
    QString date = QDate::currentDate().toString("dd/MM/yyyy");
    QString time = QTime::currentTime().toString("HH:mm");

    if (!tempOk || !humOk || !windOk) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid values for temperature, humidity, and wind speed.");
        return;
    }

    Measurement m(nextId, temp, hum, wind, date.toStdString(), time.toStdString());
    station.addMeasurement(m);
    nextId++;

    // Clear inputs
    temperatureEdit->clear();
    humidityEdit->clear();
    windSpeedEdit->clear();

    refreshTable();
    showStatistics();
    QMessageBox::information(this, "Success", "Measurement added successfully!");
}

void MainWindow::deleteMeasurement() {
    int currentRow = measurementTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a measurement to delete.");
        return;
    }

    int id = measurementTable->item(currentRow, 0)->text().toInt();
    if (station.removeMeasurement(id)) {
        refreshTable();
        showStatistics();
        QMessageBox::information(this, "Success", "Measurement deleted successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Could not delete measurement.");
    }
}

void MainWindow::loadFromFile() {
    if (station.loadFromFile(dataFile.toStdString())) {
        // Update nextId based on loaded data
        const auto& measurements = station.getMeasurements();
        nextId = 1;
        for (const auto& m : measurements) {
            if (m.getId() >= nextId) {
                nextId = m.getId() + 1;
            }
        }
        refreshTable();
        showStatistics();
        QMessageBox::information(this, "Success", "Data loaded from file successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Could not load data from file.");
    }
}

void MainWindow::saveToFile() {
    if (station.saveToFile(dataFile.toStdString())) {
        QMessageBox::information(this, "Success", "Data saved to file successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Could not save data to file.");
    }
}

void MainWindow::showStatistics() {
    const auto& measurements = station.getMeasurements();
    
    if (measurements.empty()) {
        avgTempLabel->setText("Avg: --");
        minTempLabel->setText("Min: --");
        maxTempLabel->setText("Max: --");
        avgHumidityLabel->setText("Humidity: --");
        avgWindLabel->setText("Wind: --");
        return;
    }

    float avgTemp = Analyzer::averageTemperature(measurements);
    float minTemp = Analyzer::minTemperature(measurements);
    float maxTemp = Analyzer::maxTemperature(measurements);
    float avgHum = Analyzer::averageHumidity(measurements);
    float avgWind = Analyzer::averageWindSpeed(measurements);

    avgTempLabel->setText(QString("Avg: %1°C").arg(avgTemp, 0, 'f', 1));
    minTempLabel->setText(QString("Min: %1°C").arg(minTemp, 0, 'f', 1));
    maxTempLabel->setText(QString("Max: %1°C").arg(maxTemp, 0, 'f', 1));
    avgHumidityLabel->setText(QString("Humidity: %1%").arg(avgHum, 0, 'f', 1));
    avgWindLabel->setText(QString("Wind: %1 km/h").arg(avgWind, 0, 'f', 1));
}

void MainWindow::refreshTable() {
    const auto& measurements = station.getMeasurements();
    measurementTable->setRowCount(measurements.size());

    for (size_t i = 0; i < measurements.size(); ++i) {
        const Measurement& m = measurements[i];
        
        auto createItem = [](const QString &text) {
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            return item;
        };

        measurementTable->setItem(i, 0, createItem(QString::number(m.getId())));
        measurementTable->setItem(i, 1, createItem(QString::number(m.getTemperature(), 'f', 1)));
        measurementTable->setItem(i, 2, createItem(QString::number(m.getHumidity(), 'f', 1)));
        measurementTable->setItem(i, 3, createItem(QString::number(m.getWindSpeed(), 'f', 1)));
        measurementTable->setItem(i, 4, createItem(QString::fromStdString(m.getDate())));
        measurementTable->setItem(i, 5, createItem(QString::fromStdString(m.getTime())));
    }
}
