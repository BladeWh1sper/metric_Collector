#include "MetricLogger.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>

MetricLogger::MetricLogger(const std::string& filename, int interval_ms)
    : filename_(filename), interval_ms_(interval_ms), stop_flag_(false) {}

MetricLogger::~MetricLogger() {
    stop();
}

void MetricLogger::start() {
    worker_ = std::thread(&MetricLogger::run, this);
}

void MetricLogger::stop() {
    stop_flag_ = true;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void MetricLogger::run() {
    std::ofstream file(filename_, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename_ << std::endl;
        return;
    }

    while (!stop_flag_) {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm, &t_c);
#else
        localtime_r(&t_c, &tm);
#endif
        std::ostringstream timestamp;
        timestamp << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
                  << "." << std::setfill('0') << std::setw(3) << ms.count();

        file << timestamp.str();

        auto metrics = MetricRegistry::instance().getMetricsSnapshot();
        for (auto& metric : metrics) {
            double value = metric->getValue();
            file << " \"" << metric->getName() << "\" " << value;
        }
        file << std::endl;

        file.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms_));
    }

    file.close();
}
