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

// Эмулятор доступности DB
class DbAvailabilitySimulator {
public:
    DbAvailabilitySimulator() : gen_(rd_()), dist_(0, 1), value_(1) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(500));
            }
        });
    }
    ~DbAvailabilitySimulator() { stop_ = true; worker_.join(); }
    double getValue() const { return value_; }

private:
    random_device rd_;
    mt19937 gen_;
    uniform_int_distribution<> dist_;
    atomic<int> value_;
    atomic<bool> stop_{false};
    thread worker_;
};


// Эмулятор активного пользователя
class ActiveUsersSimulator {
public:
    ActiveUsersSimulator() : gen_(rd_()), dist_(0, 10000), value_(0) {
        worker_ = thread([this]() {
            while (!stop_) {
                value_ = dist_(gen_);
                this_thread::sleep_for(chrono::milliseconds(150));
            }
        });
    }
    ~ActiveUsersSimulator() { stop_ = true; worker_.join(); }
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
    DbAvailabilitySimulator db;
    ActiveUsersSimulator users;

    // Регистрируем все метрики через function
    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("DB Availability", [&db]() { return db.getValue(); })
    );
    MetricRegistry::instance().registerMetric(
        make_shared<Metric>("Active Users", [&users]() { return users.getValue(); })
    );

    MetricLogger logger("metrics.log", 1000);
    logger.start();

    this_thread::sleep_for(chrono::seconds(10));

    logger.stop();
    return 0;
}
