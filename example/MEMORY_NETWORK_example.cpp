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

// Memory эмулятор
class MemorySimulator {
public:
    MemorySimulator() : gen_(rd_()), dist_(100, 8000), value_(0) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        });
    }
    ~MemorySimulator() { stop_ = true; worker_.join(); }
    double getValue() const { return value_; }

private:
    random_device rd_;
    mt19937 gen_;
    uniform_int_distribution<> dist_;
    atomic<int> value_;
    atomic<bool> stop_{false};
    thread worker_;
};

// Network эмулятор
class NetworkSimulator {
public:
    NetworkSimulator() : gen_(rd_()), dist_(0, 10000), value_(0) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        });
    }
    ~NetworkSimulator() { stop_ = true; worker_.join(); }
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
    MemorySimulator memory;
    NetworkSimulator network;

    // Регистрируем все метрики через function
    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("Memory usage MB", [&memory]() { return memory.getValue(); })
    );
    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("Network in KB/s", [&network]() { return network.getValue(); })
    );

    MetricLogger logger("metrics.log", 1000);
    logger.start();

    this_thread::sleep_for(chrono::seconds(10));

    logger.stop();
    return 0;
}
