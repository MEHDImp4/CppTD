#ifndef ADDMEASUREMENTDIALOG_H
#define ADDMEASUREMENTDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDateTime>
#include "../include/Measurement.h"

class AddMeasurementDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddMeasurementDialog(QWidget *parent = nullptr);
    Measurement getMeasurement() const;

private:
    QDoubleSpinBox *tempSpinBox;
    QDoubleSpinBox *humiditySpinBox;
    QDoubleSpinBox *windSpeedSpinBox;
    QComboBox *windDirComboBox;
    QDateTimeEdit *dateTimeEdit;
};

#endif // ADDMEASUREMENTDIALOG_H
