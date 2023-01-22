#include <SString/algorithm.h>
#include <gtest/gtest.h>

const char *str = "Hello 你好 こんにちは";
const char *sub0 = "你好";
const char *sub1 = "He";
const char *sub2 = "にちは";

TEST(TestAlgol, Normal) {
    EXPECT_TRUE(sstr::NORMAL(str, sub0) == 6);
    EXPECT_TRUE(sstr::NORMAL(str, sub1) == 0);
    EXPECT_TRUE(sstr::NORMAL(str, sub2) == 19);
}

TEST(TestAlgol, BM) {
    EXPECT_TRUE(sstr::BM(str, sub0) == 6);
    EXPECT_TRUE(sstr::BM(str, sub1) == 0);
    EXPECT_TRUE(sstr::BM(str, sub2) == 19);
}

TEST(TestAlgol, KMP) {
    EXPECT_TRUE(sstr::KMP(str, sub0) == 6);
    EXPECT_TRUE(sstr::KMP(str, sub1) == 0);
    EXPECT_TRUE(sstr::KMP(str, sub2) == 19);
}