#include <SString/SStringBuilder.h>

using sstr::SChar;
using sstr::SString;
using sstr::SStringBuilder;
using sstr::NullChar;

int main() {
    SStringBuilder builder(1024);
    printf("builder.null = %s\n", builder.null() ? "true" : "false");

    builder.append("你好，");
    auto tmp0 = SString::fromUTF8("SString");
    builder.append(tmp0);
    printf("after append = %s\n", builder.toString().data());
    builder.clear();

    builder.append("  0  ");
    builder.trim();
    printf("after trim = %s\n", builder.toString().data());
    builder.clear();

    builder.append("你好 Hello World");
    builder.reverse();
    printf("after reverse = %s\n", builder.toString().data());
    builder.clear();

    builder.append("こんにちは");
    printf("sub pos = %d\n", builder.find("にち"));
    auto tmp1 = SString::fromUTF8("んに");
    printf("sub pos = %d\n", builder.find(tmp1));
    builder.clear();

    builder.append("Hello");
    builder.remove(0);
    builder.remove(3);
    printf("after remove = %s\n", builder.toString().data());
    //all in -> an
    builder.set(0, (SChar) 'a');
    builder.append(" in");
    builder.remove(1, 4);
    SString i = builder.toString();
    printf("after remove = %s\n", i.data());
    builder.clear();

    return 0;
}
