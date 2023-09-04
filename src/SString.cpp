#include <SString/SString.h>
#include <SString/algorithm.h>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#pragma warning(disable : 4267)
#endif

#define BLOCK_SIZE 16

using sstr::SChar;
using sstr::SString;
using sstr::SStringView;

#if (__cplusplus < 201703L && _HAS_CXX17 == 0)
using sstr::SStringIterator;
#endif

static SChar NullChar(0);

#pragma region Util

size_t sstr::getByteLengthFromUTF8String(const char *str) {
    size_t len = 0;
    while (*(str + len)) {
        len++;
    }
    return len;
}

size_t sstr::getStringLengthFromUTF8String(const char *str) {
    size_t len = 0;
    size_t i = 0;
    while (true) {
        if (0 == str[i]) return len;
        auto n = getSizeFromUTF8Char(str[i]);
        if (-1 == n) return len;
        i += n;
        len++;
    }
}

char sstr::getSizeFromUTF8Char(char ch) {
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

char sstr::getUTF8SizeFromUnicodeChar(SChar ch) {
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

SChar sstr::getUnicodeCharFromUTF8Char(char size, const char *ch) {
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

/// \brief 获取指定字符的起始指针
/// \param str 字符串
/// \param begin 起始位置(单位 UTF-8 字符)
/// \return 指定字符的起始指针，超出范围返回 nullptr
static const char *at(const char *str, size_t begin) {
    auto p = str;
    auto i = -1;
    while (true) {
        if ('\0' == *p) {
            p = nullptr;
            break;
        }
        i++;
        if (i == begin) {
            break;
        } else {
            auto n = sstr::getSizeFromUTF8Char(*p);
            p += n;
        }
    }
    return p;
}

static void toLower(char *str) {
    while (0 != *str) {
        if (*str >= 'A' && *str <= 'Z') *str += 32;
        ++str;
    }
}

static void toUpper(char *str) {
    while (0 != *str) {
        if (*str >= 'a' && *str <= 'z') *str -= 32;
        ++str;
    }
}

#pragma endregion

#pragma region SChar

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

#pragma endregion

#pragma region SStringIterator
#if (__cplusplus < 201703L && _HAS_CXX17 == 0)

SStringIterator::SStringIterator(const char *ref, size_t size, size_t pos) {
    _ref = ref;
    _size = size;
    _pos = pos;
    _ch = sstr::getUnicodeFromUTF8Char(ref);
}

SStringIterator SStringIterator::operator++() {
    if (NullChar == _ch) {
        return *this;
    }
    auto n = getUTF8SizeFromUnicodeChar(_ch);
    _pos += n;
    _ch = sstr::getUnicodeFromUTF8Char(_ref + _pos);
    return *this;
} /**/

SStringIterator SStringIterator::operator++(int c) {
    auto count = 0;
    while (_ch != NullChar || c == count) {
        auto n = getUTF8SizeFromUnicodeChar(_ch);
        _pos += n;
        _ch = sstr::getUnicodeFromUTF8Char(_ref + _pos);
    }
    return *this;
}

bool SStringIterator::operator==(const SStringIterator &other) const {
    return _ref + _pos == other._ref + other._pos;
}

bool SStringIterator::operator!=(const SStringIterator &other) const {
    return _ref + _pos != other._ref + other._pos;
}

SChar SStringIterator::operator*() {
    return _ch;
}

SStringIterator SStringIterator::begin() {
    _pos = 0;
    _ch = sstr::getUnicodeFromUTF8Char(_ref);
    return *this;
}

SStringIterator SStringIterator::end() {
    SStringIterator end;
    end._ref = _ref;
    end._pos = _size;
    end._size = _size;
    end._ch = sstr::getUnicodeFromUTF8Char(_ref + _size);
    return end;
}

#endif
#pragma endregion

#pragma region SString

char *sstr::SString::data() {
    return _data;
}

void sstr::SString::update() {
    _size = strlen(_data);
}

sstr::SString::~SString() noexcept {
    if (_data) {
        free(_data);
        _data = nullptr;
    }
}

size_t SString::size() const {
    return _size;
}

size_t SString::cap() const {
    return _capacity;
}

sstr::SString::SString() noexcept : SStringView() {}

SString::SString(const char *str, size_t size) : SStringView() {
    _size = size;
    _capacity = (size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    _data = (char *) malloc(_capacity);
    memcpy(_data, str, size);
    _data[size] = '\0';
}

SString::SString(const sstr::SString &sString) noexcept : SStringView(sString) {
    _capacity = sString._capacity;
    _size = sString._size;
    _data = (char *) malloc(_capacity);
    memcpy(_data, sString._data, _size + 1);
}

SString::SString(sstr::SString &&sString) noexcept : SStringView() {
    _data = sString._data;
    _capacity = sString._capacity;
    _size = sString._size;

    sString._data = nullptr;
    sString._capacity = 0;
    sString._size = 0;
}

void SString::toLower() {
    ::toLower(_data);
}

void SString::toUpper() {
    ::toUpper(_data);
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

SString SString::fromSChars(SChar ch[], size_t size) {
    SString string;
    for (auto i = 0; i < size; i++) {
        string._size += getUTF8SizeFromUnicodeChar(ch[i]);
    }
    string._capacity = (string._size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    string._data = (char *) malloc(string._capacity);

    auto index = 0;
    for (auto i = 0; i < size; i++) {
        auto n = getUTF8SizeFromUnicodeChar(ch[i]);
        insertUnicodeChar2UTF8String(string._data + index, (uint32_t) ch[i], n);
        index += n;
    }

    string._data[string._size] = '\0';
    return string;
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

void SString::operator+=(const sstr::SStringView &str) {
    auto newSize = _size + str.size();
    auto n = newSize / BLOCK_SIZE + 1;
    _capacity = n * BLOCK_SIZE;

    auto newData = (char *) malloc(_capacity);
    memcpy(newData + 0, _data, _size);
    memcpy(newData + _size, str.data(), str.size());
    newData[newSize] = '\0';
    free(_data);

    _data = newData;
    _size = newSize;
}

#pragma endregion

#pragma region SStringView

bool SStringView::endsWith(const sstr::SStringView &str) const {
    if (str._size > this->_size) return false;

    auto tmp = this->_data + this->_size - str._size;
    return strcmp(tmp, str._data) == 0;
}

bool SStringView::isLower() const {
    for (int i = 0; i < _size; ++i) {
        if (_data[i] > 'A' && _data[i] < 'Z') return false;
    }
    return true;
}

bool SStringView::isUpper() const {
    for (int i = 0; i < _size; ++i) {
        if (_data[i] > 'a' && _data[i] < 'z') return false;
    }
    return true;
}

SString SStringView::toLower() const {
    SString str(_data, _size);
    ::toLower(str._data);
    return str;
}

SString SStringView::toUpper() const {
    SString str(_data, _size);
    ::toUpper(str._data);
    return str;
}

SStringView::SStringView(const char *u8str) noexcept {
    _data = const_cast<char *>(u8str);
    _size = sstr::getByteLengthFromUTF8String(_data);
}

bool SStringView::null() const {
    return _data == nullptr;
}

bool SStringView::empty() const {
    if (_data) {
        return _data[0] == '\0';
    }
    return true;
}

size_t SStringView::size() const {
    return getByteLengthFromUTF8String(_data);
}

size_t SStringView::len() const {
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

const char *SStringView::data() const {
    return _data;
}

int32_t SStringView::findByBytes(const char *bytes) const {
    return BM(_data, bytes);
}

int32_t SStringView::find(const sstr::SStringView &str) const {
    return find(str.data());
}

int32_t SStringView::find(const char *str) const {
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

#if (__cplusplus < 201703L && _HAS_CXX17 == 0)

SStringView::IteratorType SStringView::iterator() {
    return {_data, _size};
}

SStringView::IteratorType SStringView::begin() {
    return {_data, _size};
}

SString::IteratorType SStringView::end() {
    return {_data, _size, _size};
}

#endif

SString SStringView::trim() const {
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

SString SStringView::reverse() const {
    SString string;
    string._size = _size;
    string._capacity = (_size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    string._data = (char *) malloc(string._capacity);

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

SString SStringView::append(const char *str) const {
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

SString SStringView::append(const sstr::SStringView &str) const {
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

std::vector<SString> SStringView::split(const char *str) const {
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

std::vector<SString> SStringView::split(const SStringView &str) const {
    return split(str._data);
}

SString SStringView::substring(size_t begin) const {
    SString str;
    auto p = ::at(_data, begin);
    if (nullptr == p) return str;

    str._size = _size + _data - p;
    str._capacity = (str._size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    str._data = (char *) malloc(str._capacity);
    memcpy(str._data, p, str._size);
    str._data[str._size] = '\0';
    return str;
}

SString SStringView::substring(size_t begin, size_t len) const {
    SString str;
    auto start = ::at(_data, begin);
    if (nullptr == start) return str;

    // pre calculated
    auto count = 0;
    auto newSize = 0;
    auto p = start;
    while (true) {
        if ('\0' == *p) {
            break;
        } else if (count == len) {
            break;
        } else {
            auto n = sstr::getSizeFromUTF8Char(*p);
            newSize += n;
            p += n;
            count++;
        }
    }

    str._size = newSize;
    str._capacity = (str._size / BLOCK_SIZE + 1) * BLOCK_SIZE;
    str._data = (char *) malloc(str._capacity);
    memcpy(str._data, start, str._size);
    str._data[str._size] = '\0';
    return str;
}

std::vector<SChar> SStringView::toChars() const {
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

std::string SStringView::toString() const {
    return {_data};
}

std::unique_ptr<wchar_t[]> SStringView::toCWString() const {
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

std::wstring SStringView::toWString() const {
    return {toCWString().get()};
}

SChar SStringView::at(size_t index) const {
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

SChar SStringView::operator[](size_t index) const {
    return at(index);
}

bool SStringView::operator!=(const char *str) const {
    return 0 != strcmp(_data, str);
}

bool SStringView::operator!=(const sstr::SStringView &str) const {
    return 0 != strcmp(_data, str._data);
}

bool SStringView::operator==(const sstr::SStringView &str) const {
    return 0 == strcmp(_data, str._data);
}

bool SStringView::operator==(const char *str) const {
    return 0 == strcmp(_data, str);
}

SString SStringView::operator+(const SStringView &str) const {
    return append(str);
}

SString SStringView::operator+(const char *str) const {
    return append(str);
}

#pragma endregion