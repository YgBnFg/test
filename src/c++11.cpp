#include <iostream>
#include <atomic>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include "lockedcontainer.h"
#include "mytime.h"
void test_atomic() {
    std::atomic_int tot1 = 0;
    auto click = [&tot1]() {
        for (int i = 0; i < 10000000; i++) tot1++;
    };
    std::thread a[10];
    auto st = mt::getNowTime();
    for (int i = 0; i < 10; i++) {
        a[i] = std::thread(click);
    }
    for (int i = 0; i < 10; i++) {
        a[i].join();
    }
    std::cout << "time: " << mt::getTimeDiff(st, mt::getNowTime()) << std::endl;
    std::cout << tot1 << std::endl;
}
void test_sstream() {

}
void test_autoPoint() {

}
void test_sharedPoint() {

}
void test_rtti() {

}
int main() {
    //test_atomic();


    getchar();
    return 0;
}

