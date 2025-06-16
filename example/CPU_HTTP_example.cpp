#include "../metric_lib/MetricLogger.hpp"
#include "../metric_lib/MetricRegistry.hpp"
#include "../metric_lib/Metric.hpp"

#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>
#include <iostream>

using namespace std;

// Эмулятор CPU
class CpuSimulator {
public:
    CpuSimulator() : gen_(rd_()), dist_(0.0, 8.0), value_(0.0) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        });
    }

    ~CpuSimulator() {
        stop_ = true;
        worker_.join();
    }

    double getValue() const { return value_; }

private:
    random_device rd_;
    mt19937 gen_;
    uniform_real_distribution<> dist_;
    atomic<double> value_;
    atomic<bool> stop_{false};
    thread worker_;
};

// Эмулятор HTTP RPS
class HttpSimulator {
public:
    HttpSimulator() : gen_(rd_()), dist_(0, 1000), value_(0) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        });
    }

    ~HttpSimulator() {
        stop_ = true;
        worker_.join();
    }

    double getValue() const { return value_; }

private:
    random_device rd_;
    mt19937 gen_;
    uniform_int_distribution<> dist_;
    atomic<int> value_;
    atomic<bool> stop_{false};
    thread worker_;
};

int main() {
    CpuSimulator cpu;
    HttpSimulator http;

    // Регистрируем все метрики через function
    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("CPU", [&cpu]() { return cpu.getValue(); })
    );

    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("HTTP requests RPS", [&http]() { return http.getValue(); })
    );

    MetricLogger logger("metrics.log", 1000);
    logger.start();

    this_thread::sleep_for(chrono::seconds(10));

    logger.stop();
    return 0;
}
