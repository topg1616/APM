#pragma once
#include <string>
#include <optional>
#include <QDateTime>

struct Event {
    std::string sensor_id;
    QDateTime timestamp;
    std::string data;
};

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual std::string get_id() const = 0;
    virtual std::optional<Event> detect() = 0;
};

class MotionSensor : public Sensor {
private:
    std::string id;
    double sensitivity;
public:
    MotionSensor(std::string sensor_id, double sens);
    std::string get_id() const override;
    std::optional<Event> detect() override;
};

class OpenSensor : public Sensor {
private:
    std::string id;
    bool is_open;
public:
    OpenSensor(std::string sensor_id);
    std::string get_id() const override;
    std::optional<Event> detect() override;
};