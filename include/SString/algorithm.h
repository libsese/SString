#pragma once

namespace sstr {

    typedef int (*FindFunction)(const char *, const char *);

    extern int KMP(const char *str, const char *sub);

    extern int BM(const char *str, const char *sub);

    extern int NORMAL(const char *str, const char *sub);

}// namespace sstr
