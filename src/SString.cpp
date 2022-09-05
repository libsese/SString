#include <SString/SString.h>
#include <SString/algorithm.h>
#include <cstring>
#ifdef _WIN32
#include <Windows.h>
#endif

#define BLOCK_SIZE 16

using sstr::NullChar;
using sstr::SChar;
using sstr::SString;

/// 获取 UTF-8 字符串字节长度
/// \param str
/// \return
static size_t getByteLengthFromUTF8String(const char *str) {
    size_t len = 0;
    while (*(str + len)) {
        len++;
    }
    return len;
}

/// 获取 UTF-8 字符串占位字节数
/// \param ch
/// \return
static char getSizeFromUTF8Char(char ch) {
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

/// 从 SChar 中获取该字符在 UTF-8 中的字节占位字节数
/// \param ch Unicode 字符
/// \return 占位字节数
static char getUTF8SizeFromUnicodeChar(SChar ch) {
    if ((uint32_t) ch <= 0x7f) {
        return 1;
    } else if ((uint32_t) ch > 0x7f && (uint32_t) ch <= 0x7ff) {
        return 2;
    } else if ((uint32_t) ch > 0x7ff && (uint32_t) ch <= 0xffff) {
        return 3;
    } else if ((uint32_t) ch > 0xffff && (uint32_t) ch <= 0x7ffff) {
        return 4;
    } else {
        return -1;
    }
}

/// 从 wchar_t 中获取该字符在 UTF-8 中的字节占位字节数
/// \warning 该函数仅在 *nix 上可用
/// \param ch 宽字节
/// \return 占位字节数
inline char getUTF8SizeFromWChat(wchar_t ch) {
    return getUTF8SizeFromUnicodeChar((SChar) ch);
}

/// 从 UTF-8 字符串中获取 Unicode 字符
/// \param size 该 UTF-8 占位字节数
/// \param ch UTF-8 字符起始位置
/// \return Unicode 字符
static SChar getUnicodeCharFromUTF8Char(char size, const char *ch) {
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

/// 向字节流中写入 UTF-8 编码的 Unicode 字符
/// \param destination 写入位置
/// \param code Unicode 字符
/// \param n 该字符在 UTF-8 占位字节数
/// \return 是否成功
static bool insertUnicodeChar2UTF8String(char *destination, uint32_t code, size_t n) {
    if (1 == n) {
        *(destination + 0) = (char) (0b01111111 & code);
    } else if (2 == n) {
        *(destination + 0) = (char) (0b11000000 | (0b0000011111000000 & code) >> 6);
        *(destination + 1) = (char) (0b10000000 | (0b0000000000111111 & code));
    } else if (3 == n) {
        *(destination + 0) = (char) (0b11100000 | (0b1111000000000000 & code) >> 12);
        *(destination + 1) = (char) (0b10000000 | (0b0000111111000000 & code) >> 6);
        *(destination + 2) = (char) (0b10000000 | (0b0000000000111111 & code));
    } else if (4 == n) {
        *(destination + 0) = (char) (0b11110000 | (0b000111000000000000000000 & code) >> 18);
        *(destination + 1) = (char) (0b10000000 | (0b000000111111000000000000 & code) >> 12);
        *(destination + 2) = (char) (0b10000000 | (0b000000000000111111000000 & code) >> 6);
        *(destination + 3) = (char) (0b10000000 | (0b000000000000000000111111 & code));
    } else {
        return false;
    }
    return true;
}

SChar sstr::getUnicodeFromUTF8Char(const char *u8char) {
    return getUnicodeCharFromUTF8Char(getSizeFromUTF8Char(*u8char), u8char);
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
        free(_data);
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

int32_t SString::findByBytes(const char *bytes) const {
    return BM(_data, bytes);
}

int32_t SString::find(const sstr::SString &str) const {
    return find(str._data);
}

int32_t SString::find(const char *str) const {
    auto index = BM(_data, str);
    auto count = 0;

    if (-1 == index) return -1;

    for (auto i = 0; i < index;) {
        auto n = getSizeFromUTF8Char(_data[i]);
        i += n;
        count++;
    }
    return count;
}

SString SString::trim() const {
    auto newSize = _size;
    for (auto i = 0; i < _size; i++) {
        if (_data[i] == ' ') {
            newSize--;
        }
    }

    auto newCap = (newSize / BLOCK_SIZE + 1) * BLOCK_SIZE;

    char *newData = (char *) malloc(newCap);
    char *p = _data;
    while (*p == ' ') p++;
    memcpy(newData, p, newSize);
    newData[newSize] = '\0';

    SString string;
    string._size = newSize;
    string._capacity = newCap;
    string._data = newData;
    return string;
}

SString SString::reverse() const {
    SString string;
    string._size = _size;
    string._capacity = _capacity;
    string._data = (char *) malloc(_capacity);

    auto index = _size;
    string._data[index] = '\0';

    for (auto i = 0; i < _size;) {
        auto n = getSizeFromUTF8Char(_data[i]);
        index -= n;
        memcpy(string._data + index, _data + i, n);
        i += n;
    }

    return string;
}

SString SString::append(const char *str) const {
    SString res;
    auto len = strlen(str);
    res._size = _size + len;
    auto n = res._size / BLOCK_SIZE + 1;
    res._capacity = n * BLOCK_SIZE;
    res._data = (char *) malloc(res._capacity);
    memcpy(res._data + 0, _data, _size);
    memcpy(res._data + _size, str, len);
    res._data[res._size] = '\0';
    return res;
}

SString SString::append(const sstr::SString &str) const {
    SString res;
    res._size = _size + str._size;
    auto n = res._size / BLOCK_SIZE + 1;
    res._capacity = n * BLOCK_SIZE;
    res._data = (char *) malloc(res._capacity);
    memcpy(res._data + 0, _data, _size);
    memcpy(res._data + _size, str._data, str._size);
    res._data[res._size] = '\0';
    return res;
}

sstr::SString::SString() noexcept = default;

SString::SString(const char *str, size_t size) {
    _size = size;
    _capacity = (size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    _data = (char *) malloc(_capacity);
    memcpy(_data, str, size);
    _data[size] = '\0';
}

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
            return getUnicodeCharFromUTF8Char(c, &_data[i]);
        }
        i += c;
    }
    return NullChar;
}

std::vector<SChar> SString::toChars() const {
    auto len = getByteLengthFromUTF8String(_data);
    std::vector<SChar> chars;
    chars.reserve(len);
    for (size_t i = 0; i < _size;) {
        if (0 == _data[i]) break;
        auto n = getSizeFromUTF8Char(_data[i]);
        if (-1 == n) break;
        if (i + n > _size) break;
        chars.emplace_back(getUnicodeCharFromUTF8Char(n, &_data[i]));
        i += n;
    }
    return chars;
}

std::vector<SString> SString::split(const char *str) const {
    std::vector<SString> v;
    auto size = getByteLengthFromUTF8String(str);

    std::string::size_type pos1, pos2;
    pos2 = BM(_data, str);
    pos1 = 0;
    while (true) {
        v.emplace_back(SString(_data + pos1, pos2 - pos1));

        pos1 = pos2 + size;
        pos2 = BM(_data + pos1, str);
        if (-1 == pos2) {
            v.emplace_back(SString::fromUTF8(_data + pos1));
            break;
        } else {
            pos2 += pos1;
        }
    }
    return v;
}

std::vector<SString> SString::split(const SString &str) const {
    return split(str._data);
}

std::string SString::toString() const {
    return {_data};
}

std::unique_ptr<wchar_t[]> SString::toCWString() const {
#ifdef _WIN32
    size_t size = MultiByteToWideChar(CP_UTF8, 0, _data, -1, NULL, 0);
    auto ptr = std::unique_ptr<wchar_t[]>(new wchar_t[size]);
    MultiByteToWideChar(CP_UTF8, 0, _data, -1, ptr.get(), size);
    return ptr;
#else
    size_t size = len();
    auto str = new wchar_t[size + 1];
    auto ptr = std::unique_ptr<wchar_t[]>(str);
    auto count = 0;
    for (auto i = 0; i < _size;) {
        auto n = getSizeFromUTF8Char(_data[i]);
        str[count] = (wchar_t) (uint32_t) getUnicodeCharFromUTF8Char(n, _data + i);
        i += n;
        count++;
    }
    str[size] = L'\0';
    return ptr;
#endif
}

std::wstring SString::toWString() const {
    return {toCWString().get()};
}

SChar SString::operator[](size_t index) const {
    return at(index);
}

bool SString::operator!=(const char *str) const {
    return 0 != strcmp(_data, str);
}

bool SString::operator!=(const sstr::SString &str) const {
    return 0 != strcmp(_data, str._data);
}

bool SString::operator==(const sstr::SString &str) const {
    return 0 == strcmp(_data, str._data);
}

bool SString::operator==(const char *str) const {
    return 0 == strcmp(_data, str);
}

SString SString::operator+(const SString &str) const {
    return append(str);
}

SString SString::operator+(const char *str) const {
    return append(str);
}

void SString::operator+=(const char *str) {
    auto len = strlen(str);
    auto newSize = _size + len;
    auto n = newSize / BLOCK_SIZE + 1;
    _capacity = n * BLOCK_SIZE;

    auto newData = (char *) malloc(_capacity);
    memcpy(newData + 0, _data, _size);
    memcpy(newData + _size, str, len);
    newData[newSize] = '\0';
    free(_data);

    _data = newData;
    _size = newSize;
}

void SString::operator+=(const sstr::SString &str) {
    auto newSize = _size + str._size;
    auto n = newSize / BLOCK_SIZE + 1;
    _capacity = n * BLOCK_SIZE;

    auto newData = (char *) malloc(_capacity);
    memcpy(newData + 0, _data, _size);
    memcpy(newData + _size, str._data, str._size);
    newData[newSize] = '\0';
    free(_data);

    _data = newData;
    _size = newSize;
}

SString SString::fromUTF8(const char *str) {
    SString sString;
    sString._size = getByteLengthFromUTF8String(str);
    auto n = sString._size / BLOCK_SIZE + 1;
    sString._capacity = n * BLOCK_SIZE;
    sString._data = (char *) malloc(n * sString._capacity);
    memcpy(sString._data, str, sString._size);
    sString._data[sString._size] = '\0';
    return sString;
}

SString SString::fromSChars(std::vector<SChar> &chars) {
    SString string;
    for (auto i: chars) {
        // 暂时不处理损坏的字符
        string._size += getUTF8SizeFromUnicodeChar(i);
    }
    string._capacity = (string._size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    string._data = (char *) malloc(string._capacity);

    auto index = 0;
    for (auto i: chars) {
        auto n = getUTF8SizeFromUnicodeChar(i);
        insertUnicodeChar2UTF8String(string._data + index, (uint32_t) i, n);
        index += n;
    }

    string._data[string._size] = '\0';
    return string;
}

SString SString::fromUCS2LE(const wchar_t *str) {
    // on dos-like, wchar_t use 2 bytes
    // as utf-16le
    // on *nix, wchar_t use 4 bytes by default
    // as utf-32le
    SString sString;
    const wchar_t *p = str;
    while (L'\0' != *p) {
        sString._size += getUTF8SizeFromWChat(*p);
        p++;
    }
    sString._capacity = (sString._size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    sString._data = (char *) malloc(sString._capacity);
    // 无法转换部分字符串
    // wcstombs(sString._data, str, sString._size);

#ifdef _WIN32
    WideCharToMultiByte(CP_UTF8, 0, str, -1, sString._data, sString._size, NULL, NULL);
#else
    p = str;
    auto index = 0;
    while (L'\0' != *p) {
        auto n = getUTF8SizeFromUnicodeChar((SChar) *p);
        insertUnicodeChar2UTF8String(sString._data + index, (uint32_t) *p, n);
        index += n;
        p++;
    }
#endif

    sString._data[sString._size] = '\0';
    return sString;
}