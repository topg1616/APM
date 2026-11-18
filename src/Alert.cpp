#include "Alert.h"

Alert::Alert(std::string zone, std::string desc)
    : zoneName(zone), status(AlertStatus::ACTIVE), description(desc) {
    if (zone.empty()) {
        throw std::invalid_argument("Зона не может быть пустой");
    }
}

std::string Alert::get_zone() const { return zoneName; }
AlertStatus Alert::get_status() const { return status; }
std::string Alert::get_description() const { return description; }

void Alert::acknowledge() {
    if (status != AlertStatus::ACTIVE)
        throw std::runtime_error("Нельзя подтвердить: неверный статус");
    status = AlertStatus::ACKNOWLEDGED;
}

void Alert::dismiss(const std::string& reason) {
    if (status != AlertStatus::ACTIVE)
        throw std::runtime_error("Нельзя отклонить: неверный статус");
    status = AlertStatus::DISMISSED;
}

void Alert::escalate() {
    if (status != AlertStatus::ACTIVE)
        throw std::runtime_error("Нельзя эскалировать: неверный статус");
    status = AlertStatus::ESCALATED;
}

void Alert::archive() {
    if (status != AlertStatus::ACKNOWLEDGED && status != AlertStatus::DISMISSED)
        throw std::runtime_error("Нельзя архивировать: тревога не подтверждена и не отклонена");
    status = AlertStatus::ARCHIVED;
}