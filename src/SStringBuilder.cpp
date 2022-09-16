#include <SString/SStringBuilder.h>
#include <cstring>

#define BLOCK_SIZE 1024

using sstr::SChar;
using sstr::SString;
using sstr::SStringBuilder;
using sstr::NullChar;

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

const uint32_t * SStringBuilder::data() const {
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
    if(_cap < newSize) {
        reserve((newSize / BLOCK_SIZE + 1) * BLOCK_SIZE);
    }

    auto index = 0;
    for(auto i = 0; i < count; i++) {
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
    if(_cap < newSize) {
        reserve((newSize / BLOCK_SIZE + 1) * BLOCK_SIZE);
    }

    auto p = str.data();
    auto index = 0;
    for(auto i = 0; i < count; i++) {
        auto n = sstr::getSizeFromUTF8Char(p[index]);
        _data[_size + i] = (uint32_t) sstr::getUnicodeCharFromUTF8Char(n, p + index);
        index += n;
    }

    _size = newSize;
}

bool SStringBuilder::reserve(size_t size) {
    if(size > _cap) {
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

SString SStringBuilder::toString() const {
    return SString::fromSChars((SChar *)_data, _size);
}