#include <SString/SStringBuilder.h>
#include <SString/algorithm.h>
#include <cstring>

#define BLOCK_SIZE 1024

using sstr::NullChar;
using sstr::SChar;
using sstr::SString;
using sstr::SStringBuilder;

SStringBuilder::SStringBuilder(const SStringBuilder &builder) {
    _cap = builder._cap;
    _size = builder._size;
    _data = (uint32_t *) malloc(_cap * sizeof(uint32_t));
    memcpy(_data, builder._data, (_size + 1) * sizeof(uint32_t));
}

SStringBuilder::SStringBuilder(SStringBuilder &&builder) {
    _data = builder._data;
    _size = builder._size;
    _cap = builder._cap;

    builder._data = nullptr;
    builder._size = 0;
    builder._cap = 0;
}

SStringBuilder::SStringBuilder(size_t bufferSize) {
    _data = (uint32_t *) malloc(bufferSize * sizeof(uint32_t));
    _cap = bufferSize;
}

SStringBuilder::~SStringBuilder() {
    free(_data);
    _data = nullptr;
    _cap = 0;
    _size = 0;
}

const uint32_t *SStringBuilder::data() const {
    return _data;
}

size_t SStringBuilder::size() const {
    return _size;
}

size_t SStringBuilder::cap() const {
    return _cap;
}

bool SStringBuilder::null() const {
    return 0 == _size;
}

bool SStringBuilder::emtpy() const {
    return 0 == _size;
}

void SStringBuilder::append(const char *str) {
    size_t count = sstr::getStringLengthFromUTF8String(str);
    size_t newSize = count + _size;

    // 空间不足以完整追加数据
    if (_cap < newSize) {
        reserve((newSize / BLOCK_SIZE + 1) * BLOCK_SIZE);
    }

    auto index = 0;
    for (auto i = 0; i < count; i++) {
        auto n = sstr::getSizeFromUTF8Char(str[index]);
        _data[_size + i] = (uint32_t) sstr::getUnicodeCharFromUTF8Char(n, str + index);
        index += n;
    }

    _size = newSize;
}

void SStringBuilder::append(const SString &str) {
    size_t count = str.len();
    size_t newSize = count + _size;

    // 空间不足以完整追加数据
    if (_cap < newSize) {
        reserve((newSize / BLOCK_SIZE + 1) * BLOCK_SIZE);
    }

    auto p = str.data();
    auto index = 0;
    for (auto i = 0; i < count; i++) {
        auto n = sstr::getSizeFromUTF8Char(p[index]);
        _data[_size + i] = (uint32_t) sstr::getUnicodeCharFromUTF8Char(n, p + index);
        index += n;
    }

    _size = newSize;
}

bool SStringBuilder::reserve(size_t size) {
    if (size > _cap) {
        auto newData = (uint32_t *) malloc(size * sizeof(uint32_t));
        memcpy(newData, _data, _size * sizeof(uint32_t));
        free(_data);
        _data = newData;
        _cap = size;
        return true;
    } else {
        return false;
    }
}

void SStringBuilder::trim() {
    size_t i = 0;// 头部空格数
    size_t j = 0;// 尾部空格数
    bool flag = true;
    for (auto n = 0; n < _size; n++) {
        if (flag && ' ' == _data[n]) {
            i++;
        } else {
            flag = false;
        }

        if (' ' == _data[_size - 1 - n]) {
            j++;
        } else {
            break;
        }
    }

    // 头尾空白相接
    if (i + j == _size) {
        _data[0] = 0;
        return;
    }
    // 正常处理
    else {
        size_t len = _size - j - i;
        for (size_t n = 0; n < len; n++) {
            _data[n] = _data[i + n];
        }
        _data[len] = 0;
    }
}

void SStringBuilder::reverse() {
    size_t n = _size / 2;
    uint32_t tmp;
    for (auto i = 0; i < n; i++) {
        tmp = _data[i];
        _data[i] = _data[_size - i - 1];
        _data[_size - i - 1] = tmp;
    }
}

void SStringBuilder::clear() {
    _data[0] = 0;
    _size = 0;
}

SChar SStringBuilder::at(size_t index) const {
    if (index + 1 >= _size) {
        return NullChar;
    } else {
        return SChar(_data[index]);
    }
}

SString SStringBuilder::toString() const {
    return SString::fromSChars((SChar *) _data, _size);
}

int32_t SStringBuilder::find(const char *str) const {
    auto sub = SString::fromUTF8(str).toChars();
    return sstr::BM(_data, _size, sub);
}

int32_t SStringBuilder::find(const SString &str) const {
    auto sub = str.toChars();
    return sstr::BM(_data, _size, sub);
}

void SStringBuilder::set(size_t index, SChar ch) {
    if (index + 1 > _size) return;

    _data[index] = (uint32_t) ch;
}

void SStringBuilder::remove(size_t index) {
    if (index + 1 > _size) return;

    for (auto i = index; i < _size; i++) {
        _data[i] = _data[i + 1];
    }

    _size -= 1;
}

void SStringBuilder::remove(size_t begin, size_t len) {
    if (begin + 1 > _size) return;
    // 限制 len 的大小
    len = _size - begin - 1 < len ? _size - begin - 1 : len;

    for (auto i = begin; i < _size - len; i++) {
        _data[i] = _data[i + len];
    }

    _size -= len;
}