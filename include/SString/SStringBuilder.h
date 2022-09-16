#pragma once
#include <SString/SString.h>

namespace sstr {

    class SStringBuilder final {
        // 构造相关
    public:
        SStringBuilder(const SStringBuilder &builder);
        SStringBuilder(SStringBuilder &&builder);
        SStringBuilder(size_t bufferSize);
        ~SStringBuilder();

        // 基础功能
    public:
        const uint32_t *data() const;
        size_t size() const;
        size_t cap() const;

        bool null() const;
        bool emtpy() const;
        /// 扩容
        /// \param size 扩容大小，单位为 4 bytes
        /// \return 操作是否成功
        bool reserve(size_t size);
        void trim();
        void reverse();
        int32_t find(const char *str) const;
        int32_t find(const SString &str) const;
        void append(const char *str);
        void append(const SString &str);
        // std::vector<SString> split(const char *str) const;
        // std::vector<SString> split(const SString &str) const;
        SChar at(size_t index) const;
        void clear();
    
        SString toString() const;

    private:
        /// 数据指针
        uint32_t *_data = nullptr;
        /// 字符个数
        size_t _size = 0;
        /// 容量（单位 uint32_t 即 4 bytes）
        size_t _cap = 0;
    };

}// namespace sstr