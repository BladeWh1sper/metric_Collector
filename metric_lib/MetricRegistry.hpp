#pragma once

#include "Metric.hpp"
#include <vector>
#include <memory>
#include <mutex>

class MetricRegistry {
public:
    static MetricRegistry& instance() {
        static MetricRegistry registry;
        return registry;
    }

    void registerMetric(std::shared_ptr<Metric> metric) {
        std::lock_guard<std::mutex> lock(mutex_);
        metrics_.emplace_back(metric);
    }

    std::vector<std::shared_ptr<Metric>> getMetricsSnapshot() {
        std::lock_guard<std::mutex> lock(mutex_);
        return metrics_;
    }

private:
    MetricRegistry() = default;

    std::vector<std::shared_ptr<Metric>> metrics_;
    std::mutex mutex_;
};
