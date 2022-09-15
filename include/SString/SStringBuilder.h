#pragma once
#include <SString/SString.h>

namespace sstr {

    class SStringBuilder final {
    public:
        SStringBuilder(size_t bufferSize);
        ~SStringBuilder();

        const uint32_t *data() const;
        size_t size() const;
        size_t cap() const;

        bool null() const;
        bool emtpy() const;
        void trim();
        void reverse();
        int32_t find(const char *str) const;
        int32_t find(const SString &str) const;
        void append(const char *str);
        void append(const SString &str);
        std::vector split(const char *str) const;
        std::vector split(const SString &str) const;
        SChar at(size_t index) const;

        SString toString() const;

    private:
        uint32_t *_data = nullptr;
        size_t _size = 0;
        size_t _cap = 0;
    };

}// namespace sstr