#include <SString/algorithm.h>
#include <cstdio>

#define Test(str, sub, func) printf("func: " #func " & index: %d\n", func(str, sub))

int main() {
    const char *str = "Hello 你好 こんにちは";
    const char *sub0 = "你好";
    const char *sub1 = "He";
    const char *sub2 = "にちは";

    Test(str, sub0, sstr::NORMAL);
    Test(str, sub0, sstr::KMP);
    Test(str, sub0, sstr::BM);
    puts("");

    Test(str, sub1, sstr::NORMAL);
    Test(str, sub1, sstr::KMP);
    Test(str, sub1, sstr::BM);
    puts("");

    Test(str, sub2, sstr::NORMAL);
    Test(str, sub2, sstr::KMP);
    Test(str, sub2, sstr::BM);

    return 0;
}

#undef Test