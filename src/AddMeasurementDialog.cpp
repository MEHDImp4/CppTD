#include "AddMeasurementDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

AddMeasurementDialog::AddMeasurementDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Ajouter une mesure");

    auto *mainLayout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    tempSpinBox = new QDoubleSpinBox();
    tempSpinBox->setRange(-50.0, 100.0);
    tempSpinBox->setSuffix(" °C");

    humiditySpinBox = new QDoubleSpinBox();
    humiditySpinBox->setRange(0.0, 100.0);
    humiditySpinBox->setSuffix(" %");

    windSpeedSpinBox = new QDoubleSpinBox();
    windSpeedSpinBox->setRange(0.0, 500.0);
    windSpeedSpinBox->setSuffix(" km/h");

    windDirComboBox = new QComboBox();
    windDirComboBox->addItems({"N", "S", "E", "O", "NE", "NO", "SE", "SO"});

    dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    dateTimeEdit->setCalendarPopup(true);

    formLayout->addRow("Temperature:", tempSpinBox);
    formLayout->addRow("Humidite:", humiditySpinBox);
    formLayout->addRow("Vitesse du vent:", windSpeedSpinBox);
    formLayout->addRow("Direction du vent:", windDirComboBox);
    formLayout->addRow("Date/Heure:", dateTimeEdit);

    mainLayout->addLayout(formLayout);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttonBox);
}

Measurement AddMeasurementDialog::getMeasurement() const {
    float temp = static_cast<float>(tempSpinBox->value());
    float humidity = static_cast<float>(humiditySpinBox->value());
    float windSpeed = static_cast<float>(windSpeedSpinBox->value());
    std::string windDir = windDirComboBox->currentText().toStdString();
    std::string dateTime = dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm").toStdString();

    return Measurement(temp, humidity, windSpeed, windDir, dateTime);
}
