#include <SString/SString.h>
#include <cstring>
#include <string>

#define BLOCK_SIZE 16

using sstr::SString;

size_t strLen(const char *str) {
    size_t len = 0;
    while (*(str + len)) {
        len++;
    }
    return len;
}

char getSizeFromChar(char ch) {
    if ((ch & 0b10000000) == 0b00000000) {
        return 1;
    } else if ((ch & 0b11100000) == 0b11000000) {
        return 2;
    } else if ((ch & 0b11110000) == 0b11100000) {
        return 3;
    } else if ((ch & 0b11111000) == 0b11110000) {
        return 4;
    } else {
        return -1;
    }
}

sstr::SString::~SString() noexcept {
    if (_data) {
        delete _data;
    }
}

bool SString::null() const {
    if(_data) {
        return _data[0] == '\0';
    }
    return true;
}

bool SString::emtpy() const {
    if(_data) {
        return _data[0] == '\0';
    }
    return true;
}

size_t SString::len() const {
    size_t len = 0;
    for (size_t i = 0; i < _size;) {
        if (0 == _data[i]) return len;
        auto n = getSizeFromChar(_data[i]);
        if (-1 == n) return len;
        if (i + n > _size) return len;
        i += n;
        len++;
    }
    return len;
}

size_t SString::cap() const {
    return _capacity;
}

const char *SString::data() const {
    return _data;
}

size_t SString::size() const {
    return _size;
}

sstr::SString::SString() noexcept = default;

SString::SString(const sstr::SString &sString) noexcept {
    _data = sString._data;
    _capacity = sString._capacity;
    _size = sString._size;
}

SString::SString(sstr::SString &&sString) noexcept {
    _data = sString._data;
    _capacity = sString._capacity;
    _size = sString._size;

    sString._data = nullptr;
    sString._capacity = 0;
    sString._size = 0;
}

SString SString::fromUTF8(const char *str) {
    SString sString;
    sString._size = strLen(str);
    auto n = sString._size / BLOCK_SIZE + 1;
    sString._capacity = n * BLOCK_SIZE;
    sString._data = (char *) malloc(n * sString._capacity);
    memcpy(sString._data, str, sString._size);
    return sString;
}