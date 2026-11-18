#pragma once
#include <string>
#include <stdexcept>

enum class AlertStatus {
    ACTIVE,
    ACKNOWLEDGED,
    DISMISSED,
    ESCALATED,
    ARCHIVED
};

class Alert {
private:
    std::string zoneName;
    AlertStatus status;
    std::string description;
public:
    Alert(std::string zone, std::string desc);
    std::string get_zone() const;
    AlertStatus get_status() const;
    std::string get_description() const;
    void acknowledge();
    void dismiss(const std::string& reason);
    void escalate();
    void archive();
};