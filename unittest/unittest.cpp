#include "gtest\gtest.h"

TEST(HandHandling, DepthSegmentation)
{
    EXPECT_EQ(1, 1);
}

TEST(HandHandling, HandHandling)
{
    EXPECT_EQ(1, 1);
}



extern "C" __declspec(dllexport) int RunAllUnitTests(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
