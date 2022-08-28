#include <SString/SString.h>
#include <cstdio>

using sstr::SChar;
using sstr::SString;

int main() {
    const char *str = "你好 こんにちは Hello";
    auto string = SString::fromUTF8(str);
    printf("string.data = %s\n", string.data());
    printf("string.len = %lu\n", string.len());
    printf("string.cap = %lu\n", string.cap());
    printf("string.size = %lu\n", string.size());
    printf("string.emtpy = %s\n", string.emtpy() ? "true" : "false");
    puts("");

    const char *s1 = "你こH";
    SChar ch0 = sstr::getSCharFromUTF8Char(s1);
    printf("ch0 = %u\n", (uint32_t) ch0);

    auto str1 = SString::fromUTF8(s1);
    printf("str1.data = %s\n", str1.data());
    printf("string.at(0) == str1.at(0) = %s\n", string.at(0) == str1.at(0) ? "true" : "false");
    printf("string.at(3) == str1.at(1) = %s\n", string.at(3) == str1.at(1) ? "true" : "false");
    printf("string[9] == str1[2] = %s\n", string[9] == str1[2] ? "true" : "false");
    puts("");

    auto chars = string.toChars();
    for (auto ch: chars) {
        printf("\\u%04X", ch.code);
    }
    puts("");
    puts("");

    auto string1 = SString::fromUTF8(str);
    printf("string1.data = %s\n", string1.data());
    printf("string == string1 = %s\n", string == string1 ? "true" : "false");
    printf("string != str = %s\n", string != str ? "true" : "false");
    printf("string == str1 = %s\n", string == str1 ? "true" : "false");
    puts("");

    SString front = SString::fromUTF8("你好");
    SString back = SString::fromUTF8("Hello");
    const char *extra = "こんにちは";
    SString res0 = front + back;
    printf("front + back = %s\n", res0.data());
    res0 += extra;
    printf("front + back + extra = %s\n", res0.data());

    return 0;
}