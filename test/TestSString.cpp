#include <SString/SString.h>
#include <cstdio>

using sstr::SString;

int main() {
    const char *str = "你好 こんにちは Hello";
    auto string = SString::fromUTF8(str);
    printf("string.data = %s\n", string.data());
    printf("string.len = %lu\n", string.len());
    printf("string.cap = %lu\n", string.cap());
    printf("string.size = %lu\n", string.size());
    printf("string.emtpy = %s\n", string.emtpy() ? "true" : "false");

    return 0;
}