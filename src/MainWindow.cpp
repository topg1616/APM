#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("АРМ Оператора Электроподстанции");
    resize(900, 600);
    
    setupUI();
    setupSensors();
    refreshSensorTable();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);

    auto *sensorGroup = new QGroupBox("Датчики зоны");
    auto *sensorLayout = new QVBoxLayout(sensorGroup);
    
    sensorTable = new QTableWidget();
    sensorTable->setColumnCount(3);
    sensorTable->setHorizontalHeaderLabels({"ID", "Тип", "Последнее событие"});
    sensorTable->horizontalHeader()->setStretchLastSection(true);
    sensorTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sensorLayout->addWidget(sensorTable);

    auto *checkBtn = new QPushButton("Проверить датчики");
    connect(checkBtn, &QPushButton::clicked, this, &MainWindow::onCheckSensors);
    sensorLayout->addWidget(checkBtn);
    
    mainLayout->addWidget(sensorGroup);

    auto *alertGroup = new QGroupBox("Тревоги");
    auto *alertLayout = new QVBoxLayout(alertGroup);
    
    alertTable = new QTableWidget();
    alertTable->setColumnCount(3);
    alertTable->setHorizontalHeaderLabels({"Зона", "Описание", "Статус"});
    alertTable->horizontalHeader()->setStretchLastSection(true);
    alertTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    alertTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    alertLayout->addWidget(alertTable);

    auto *btnLayout = new QHBoxLayout();
    auto *addBtn = new QPushButton("Добавить тревогу");
    auto *ackBtn = new QPushButton("Подтвердить");
    auto *arcBtn = new QPushButton("Архивировать");
    
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddAlert);
    connect(ackBtn, &QPushButton::clicked, this, &MainWindow::onAcknowledge);
    connect(arcBtn, &QPushButton::clicked, this, &MainWindow::onArchive);
    
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(ackBtn);
    btnLayout->addWidget(arcBtn);
    alertLayout->addLayout(btnLayout);
    
    mainLayout->addWidget(alertGroup);

    statusLabel = new QLabel("Готов к работе");
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);
}

void MainWindow::setupSensors() {
    zone = std::make_unique<Zone>("Зона 1 - Электроподстанция");
    zone->add_sensor(std::make_unique<MotionSensor>("MS-01", 0.8));
    zone->add_sensor(std::make_unique<OpenSensor>("OS-01"));
}

void MainWindow::refreshSensorTable() {
    sensorTable->setRowCount(zone->get_sensors().size());
    int row = 0;
    for (const auto& sensor : zone->get_sensors()) {
        sensorTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(sensor->get_id())));
        QString type = dynamic_cast<MotionSensor*>(sensor.get()) ? "Датчик движения" : "Датчик открытия";
        sensorTable->setItem(row, 1, new QTableWidgetItem(type));
        sensorTable->setItem(row, 2, new QTableWidgetItem("Ожидание"));
        row++;
    }
}

void MainWindow::refreshAlertTable() {
    alertTable->setRowCount(alerts.size());
    for (int row = 0; row < alerts.size(); row++) {
        const auto& alert = alerts[row];
        alertTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(alert->get_zone())));
        alertTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(alert->get_description())));
        
        QString status;
        switch (alert->get_status()) {
            case AlertStatus::ACTIVE: status = "Активна"; break;
            case AlertStatus::ACKNOWLEDGED: status = "Подтверждена"; break;
            case AlertStatus::DISMISSED: status = "Отклонена"; break;
            case AlertStatus::ESCALATED: status = "Эскалирована"; break;
            case AlertStatus::ARCHIVED: status = "Архивирована"; break;
        }
        alertTable->setItem(row, 2, new QTableWidgetItem(status));
    }
}

void MainWindow::onCheckSensors() {
    for (int row = 0; row < zone->get_sensors().size(); row++) {
        auto event = zone->get_sensors()[row]->detect();
        if (event.has_value()) {
            sensorTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(event->data)));
            alerts.push_back(std::make_unique<Alert>(zone->get_name(), event->data));
        }
    }
    refreshAlertTable();
    statusLabel->setText("Датчики проверены. Активных тревог: " + QString::number(alerts.size()));
}

void MainWindow::onAddAlert() {
    bool ok;
    QString desc = QInputDialog::getText(this, "Новая тревога", "Описание:", QLineEdit::Normal, "", &ok);
    if (ok && !desc.isEmpty()) {
        alerts.push_back(std::make_unique<Alert>(zone->get_name(), desc.toStdString()));
        refreshAlertTable();
        statusLabel->setText("Тревога добавлена вручную");
    }
}

void MainWindow::onAcknowledge() {
    int row = alertTable->currentRow();
    if (row < 0 || row >= alerts.size()) {
        QMessageBox::warning(this, "Внимание", "Выберите строку с тревогой");
        return;
    }
    try {
        alerts[row]->acknowledge();
        refreshAlertTable();
        statusLabel->setText("Тревога подтверждена");
    } catch (const std::runtime_error& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::onArchive() {
    int row = alertTable->currentRow();
    if (row < 0 || row >= alerts.size()) {
        QMessageBox::warning(this, "Внимание", "Выберите строку с тревогой");
        return;
    }
    try {
        alerts[row]->archive();
        refreshAlertTable();
        statusLabel->setText("Тревога архивирована");
    } catch (const std::runtime_error& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}