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
    printf("builder.toString = %s\n", builder.toString().data());

    return 0;
}
