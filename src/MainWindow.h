#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <memory>
#include <vector>
#include "Zone.h"
#include "Alert.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCheckSensors();
    void onAddAlert();
    void onAcknowledge();
    void onArchive();

private:
    QTableWidget *sensorTable;
    QTableWidget *alertTable;
    QLabel *statusLabel;

    std::unique_ptr<Zone> zone;
    std::vector<std::unique_ptr<Alert>> alerts;

    void setupUI();
    void setupSensors();
    void refreshSensorTable();
    void refreshAlertTable();
};