#include "Sensor.h"

MotionSensor::MotionSensor(std::string sensor_id, double sens)
    : id(sensor_id), sensitivity(sens) {}

std::string MotionSensor::get_id() const {
    return id;
}

std::optional<Event> MotionSensor::detect() {
    return Event{id, QDateTime::currentDateTime(), "Движение обнаружено"};
}

OpenSensor::OpenSensor(std::string sensor_id)
    : id(sensor_id), is_open(false) {}

std::string OpenSensor::get_id() const {
    return id;
}

std::optional<Event> OpenSensor::detect() {
    is_open = !is_open;
    if (is_open) {
        return Event{id, QDateTime::currentDateTime(), "Калитка открыта"};
    }
    return std::nullopt;
}