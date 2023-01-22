#include <SString/SString.h>
#include <gtest/gtest.h>
#include <cstdio>
#include <iostream>

using sstr::SChar;
using sstr::SString;
using sstr::SStringView;

TEST(TestSString, V1_0) {
    const char *str = "你好 こんにちは Hello";
    auto string = SString::fromUTF8(str);
    printf("string.data = %s\n", string.data());
    printf("string.len = %d\n", (int) string.len());
    printf("string.cap = %d\n", (int) string.cap());
    printf("string.size = %d\n", (int) string.size());
    printf("string.empty = %s\n", string.empty() ? "true" : "false");
    puts("");

    const char *s1 = "你こH";
    SChar ch0 = sstr::getUnicodeFromUTF8Char(s1);
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
    puts("begin..end");
    for (auto ch: string) {
        printf("\\u%04X", ch.code);
    }
    puts("");
    puts("");

    auto string1 = SStringView(str);
    printf("string1.data = %s\n", string1.data());
    printf("string == string1 = %s\n", string == string1 ? "true" : "false");
    printf("string != str = %s\n", string != str ? "true" : "false");
    printf("string == str1 = %s\n", string == str1 ? "true" : "false");
    puts("");

    SStringView front = SStringView("你好");
    SStringView back = SStringView("Hello");
    const char *extra = "こんにちは";
    SString res0 = front + back;
    printf("front + back = %s\n", res0.data());
    res0 += extra;
    printf("front + back + extra = %s\n", res0.data());
    puts("");

    printf("find by bytes: %d\n", string.findByBytes("こん"));
    printf("find by utf-8 char: %d\n", string.find("にちは"));
    puts("");

    SStringView spaceString = SStringView("  こんにちは  ");
    SString trimString = spaceString.trim();
    printf("after trim: %s\n", trimString.data());
    printf("after reverse: %s\n", trimString.reverse().data());
    puts("");

    SStringView spiltString = SStringView("こんにちは、わたくしはSStringです");
    printf("spilt string: %s\n", spiltString.data());
    auto spiltRes = spiltString.split("は");
    for (auto i: spiltRes) {
        printf("%s\n", i.data());
    }
    puts("");

    printf("SString from SChars: %s\n", SString::fromSChars(chars).data());

    SString wstring = SString::fromUCS2LE(L"你好 こんにちは Hello");
    printf("Is SString from wchar* equal raw string: %s\n", wstring == string ? "true" : "false");


    std::cout << "toString: " << wstring.toString() << std::endl;
    // std::wcout << L"toWString: " << wstring.toWString() << std::endl;

    auto sub0 = wstring.substring(3);
    auto sub1 = wstring.substring(3, 5);
    auto sub2 = wstring.substring(9, 10);
    printf("sub0 = %s\n", sub0.data());
    printf("sub1 = %s\n", sub1.data());
    printf("sub2 = %s\n", sub2.data());
}

TEST(TestSString, V1_1) {
    auto str = SStringView();
    printf("str.null = %s\n", str.null() ? "true" : "false");

    str = SStringView("你吃了吗？");
    auto endsWord = SStringView("吗？");
    printf("str.endsWith = %s\n", str.endsWith(endsWord) ? "true" : "false");

    str = SStringView("你好 hello");
    printf("str.isLower = %s\n", str.isLower() ? "true" : "false");
    printf("str.isUpper = %s\n", str.isUpper() ? "true" : "false");

    auto upper = str.toUpper();
    auto lower = str.toLower();
    printf("str.toUpper = %s\n", upper.data());
    printf("str.toLower = %s\n", lower.data());
    lower.toUpper();
    printf("lower.toUpper = %s\n", lower.data());
    upper.toLower();
    printf("upper.toLower = %s\n", upper.data());
}