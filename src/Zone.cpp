#include "Zone.h"

Zone::Zone(std::string zoneName) : name(zoneName) {}

std::string Zone::get_name() const { return name; }

void Zone::add_sensor(std::unique_ptr<Sensor> sensor) {
    sensors.push_back(std::move(sensor));
}

const std::vector<std::unique_ptr<Sensor>>& Zone::get_sensors() const {
    return sensors;
}