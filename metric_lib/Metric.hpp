#pragma once

#include <string>
#include <functional>

class Metric {
public:
    Metric(const std::string& name, std::function<double()> getter)
        : name_(name), getter_(getter) {}

    double getValue() const {
        return getter_();
    }

    const std::string& getName() const { return name_; }

private:
    std::string name_;
    std::function<double()> getter_;
};
