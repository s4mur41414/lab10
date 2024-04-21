#include <lib\TTaskScheduler.h>
#include <gtest/gtest.h>
#include <iostream>
#include <cmath>

TEST(SchedulerTestSuite, DefaultTest) {
float a = 1;
float b = -2;
float c = 0;

TTaskScheduler scheduler;

auto id1 = scheduler.Add([](float a, float c){return -4 * a * c;}, a, c);

auto id2 = scheduler.Add([](float b, float v){return b * b + v;}, b, scheduler.GetFutureResult<float>(id1));

auto id3 = scheduler.Add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));

auto id4 = scheduler.Add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));

auto id5 = scheduler.Add([](float a, float v){return v/(2*a);}, a, scheduler.GetFutureResult<float>(id3));

auto id6 = scheduler.Add([](float a, float v){return v/(2*a);},a, scheduler.GetFutureResult<float>(id4));

scheduler.ExecuteAll();

EXPECT_EQ(scheduler.GetResult<float>(id5), 2);
EXPECT_EQ(scheduler.GetResult<float>(id6), 0);
}

TEST(SchedulerTestSuite, FutureResultTest4) {
    double m = 52.0;
    double n = 2.0;

    TTaskScheduler scheduler;

    auto id1 = scheduler.Add([](double m, double n) { return m - n; }, m, n);
    auto id2 = scheduler.Add([](double m, double n) { return m + n; }, m, scheduler.GetFutureResult<double>(id1));

    scheduler.ExecuteAll();

    EXPECT_DOUBLE_EQ(scheduler.GetResult<double>(id1), 50.0);
    EXPECT_DOUBLE_EQ(scheduler.GetResult<double>(id2), 54.0);
}

TEST(SchedulerTestSuite, FutureResultTest2) {
    int x = 5;
    int y = 4;

    TTaskScheduler scheduler;

    auto id1 = scheduler.Add([](int x, int y) { return x / y; }, x, y);
    auto id2 = scheduler.Add([](int x, int y) { return x * y; }, x, scheduler.GetFutureResult<int>(id1));

    scheduler.ExecuteAll();

    EXPECT_EQ(scheduler.GetResult<int>(id1), 1);
    EXPECT_EQ(scheduler.GetResult<int>(id2), 5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}