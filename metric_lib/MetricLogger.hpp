#pragma once

#include "MetricRegistry.hpp"
#include <string>
#include <thread>
#include <atomic>

class MetricLogger {
public:
    MetricLogger(const std::string& filename, int interval_ms = 1000);
    ~MetricLogger();

    void start();
    void stop();

private:
    void run();

    std::string filename_;
    int interval_ms_;
    std::atomic<bool> stop_flag_;
    std::thread worker_;
};
