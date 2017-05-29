#pragma once
#include <chrono>
namespace mt {
    typedef std::chrono::steady_clock::time_point timePoint;
    static auto getNowTime() {
        return std::chrono::steady_clock::now();
    }
    static auto getTimeDiff(timePoint a,timePoint b) {
        return std::chrono::duration_cast<std::chrono::duration<double>>(b - a).count();
    }
};