#include "../metric_lib/MetricLogger.hpp"
#include "../metric_lib/MetricRegistry.hpp"
#include "../metric_lib/Metric.hpp"

#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>
#include <iostream>
#include <iostream>

using namespace std;

// Эмулятор очереди
class QueueSizeSimulator {
public:
    QueueSizeSimulator() : gen_(rd_()), dist_(0, 500), value_(0) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        });
    }
    ~QueueSizeSimulator() { stop_ = true; worker_.join(); }
    double getValue() const { return value_; }

private:
    random_device rd_;
    mt19937 gen_;
    uniform_int_distribution<> dist_;
    atomic<int> value_;
    atomic<bool> stop_{false};
    thread worker_;
};

// Эмулятор темпартуры сервера
class ServerTemperatureSimulator {
public:
    ServerTemperatureSimulator() : gen_(rd_()), dist_(30, 85), value_(40) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(200));
            }
        });
    }
    ~ServerTemperatureSimulator() { stop_ = true; worker_.join(); }
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
    QueueSizeSimulator queue;
    ServerTemperatureSimulator temp;

    // Регистрируем все метрики через function
    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("Queue size", [&queue]() { return queue.getValue(); })
    );
    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("Server Temperature C", [&temp]() { return temp.getValue(); })
    );

    MetricLogger logger("metrics.log", 1000);
    logger.start();

    this_thread::sleep_for(chrono::seconds(10));

    logger.stop();
    return 0;
}
