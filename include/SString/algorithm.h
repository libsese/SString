#pragma once
#include <cstdint>
#include <vector>

namespace sstr {

    struct SChar;

    typedef int (*FindFunction)(const char *, const char *);

    extern int KMP(const char *str, const char *sub);

    extern int BM(const char *str, const char *sub);

    extern int BM(const uint32_t *str, size_t size, std::vector<SChar> &sub);

    extern int NORMAL(const char *str, const char *sub);

}// namespace sstr
