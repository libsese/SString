#include <SString/SString.h>
#include <cstring>

#define BLOCK_SIZE 16

using sstr::NullChar;
using sstr::SChar;
using sstr::SString;

size_t getStringLengthFromUTF8(const char *str) {
    size_t len = 0;
    while (*(str + len)) {
        len++;
    }
    return len;
}

char getSizeFromUTF8Char(char ch) {
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

SChar getCodeFromUTF8(char size, const char *ch) {
    switch (size) {
        case 1:
            return SChar(*ch & 0b01111111);
        case 2:
            return SChar((*ch & 0b00011111) << 6 | (*(ch + 1) & 0b00111111));
        case 3:
            return SChar((*ch & 0b00001111) << 12 | (*(ch + 1) & 0b00111111) << 6 | (*(ch + 2) & 0b00111111));
        case 4:
            return SChar((*ch & 0b00000111) << 18 | (*(ch + 1) & 0b00111111) << 12 | (*(ch + 2) & 0b00111111) << 6 | (*(ch + 3) & 0b00111111));
        default:
            return NullChar;
    }
}

SChar sstr::getSCharFromUTF8Char(const char *u8char) {
    return getCodeFromUTF8(getSizeFromUTF8Char(*u8char), u8char);
}

SChar::SChar(uint32_t _code) noexcept { code = _code; }

bool SChar::operator==(const SChar &ch) const { return ch.code == code; }
bool SChar::operator<(const SChar &ch) const { return code < ch.code; }
bool SChar::operator<=(const SChar &ch) const { return code <= ch.code; }
bool SChar::operator>(const SChar &ch) const { return code > ch.code; }
bool SChar::operator>=(const SChar &ch) const { return code >= ch.code; }
bool SChar::operator!=(const SChar &ch) const { return code != ch.code; }

SChar SChar::operator+(const sstr::SChar &ch) const { return SChar(ch.code + code); }
SChar SChar::operator-(const sstr::SChar &ch) const { return SChar(code - ch.code); }

sstr::SChar::operator uint32_t() const { return code; }

SChar sstr::NullChar = SChar(0);

sstr::SString::~SString() noexcept {
    if (_data) {
        delete _data;
    }
}

bool SString::null() const {
    if (_data) {
        return _data[0] == '\0';
    }
    return true;
}

bool SString::emtpy() const {
    if (_data) {
        return _data[0] == '\0';
    }
    return true;
}

size_t SString::len() const {
    size_t len = 0;
    for (size_t i = 0; i < _size;) {
        if (0 == _data[i]) return len;
        auto n = getSizeFromUTF8Char(_data[i]);
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

SChar SString::at(size_t index) const {
    index += 1;
    size_t n = 0;
    for (size_t i = 0; i < _size;) {
        if (0 == _data[i]) return NullChar;
        auto c = getSizeFromUTF8Char(_data[i]);
        if (-1 == c) return NullChar;
        if (i + c > _size) return NullChar;
        n++;
        if (index == n) {
            return getCodeFromUTF8(c, &_data[i]);
        }
        i += c;
    }
    return NullChar;
}

std::vector<SChar> SString::toChars() const {
    auto len = getStringLengthFromUTF8(_data);
    std::vector<SChar> chars;
    chars.reserve(len);
    for (size_t i = 0; i < _size;) {
        if (0 == _data[i]) break;
        auto n = getSizeFromUTF8Char(_data[i]);
        if (-1 == n) break;
        if (i + n > _size) break;
        chars.emplace_back(getCodeFromUTF8(n, &_data[i]));
        i += n;
    }
    return chars;
}

SChar SString::operator[](size_t index) const {
    return at(index);
}

SString SString::fromUTF8(const char *str) {
    SString sString;
    sString._size = getStringLengthFromUTF8(str);
    auto n = sString._size / BLOCK_SIZE + 1;
    sString._capacity = n * BLOCK_SIZE;
    sString._data = (char *) malloc(n * sString._capacity);
    memcpy(sString._data, str, sString._size);
    return sString;
}

//SString SString::fromUCS2LE(const wchar_t *str) {
//    SString sString;
//#ifdef _WIN32
//    // on dos-like, wchar_t use 2 bytes
//    // as utf-16le
//#else
//    // on *nix, wchar_t use 4 bytes by defalut
//    // as utf-32le
//#endif
//}