#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Sensor.h"

class Zone {
private:
    std::string name;
    std::vector<std::unique_ptr<Sensor>> sensors;
public:
    Zone(std::string zoneName);
    std::string get_name() const;
    void add_sensor(std::unique_ptr<Sensor> sensor);
    const std::vector<std::unique_ptr<Sensor>>& get_sensors() const;
};