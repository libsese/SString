#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

namespace sstr {

    struct SChar final {
        uint32_t code = 0;

        explicit SChar(uint32_t code) noexcept;

        bool operator==(const SChar &ch) const;
        bool operator<(const SChar &ch) const;

        bool operator<=(const SChar &ch) const;
        bool operator>(const SChar &ch) const;
        bool operator>=(const SChar &ch) const;
        bool operator!=(const SChar &ch) const;

        SChar operator+(const SChar &ch) const;
        SChar operator-(const SChar &ch) const;

        explicit operator uint32_t () const;
    };

    extern SChar NullChar;

    extern SChar getSCharFromUTF8Char(const char *u8char);

    class SString final {
        /// 构造相关
    public:
        explicit SString() noexcept;
        SString(const SString &sString) noexcept;
        SString(SString &&sString) noexcept;
        ~SString() noexcept;

        // static SString fromSChars(std::vector<SChar> &chars);
        static SString fromUTF8(const char *str);
        // static SString fromUCS2LE(const wchar_t *str);

        // 基础功能
    public:
        /// 等价于 emtpy
        /// \retval true 字符串为空
        /// \retval false 字符串不为空
        bool null() const;
        /// 字符串是否为空
        /// \retval true 字符串为空
        /// \retval false 字符串不为空
        bool emtpy() const;
        /// 获取字符串中字符个数
        /// \return 字符个数
        size_t len() const;
        /// 获取缓存区容量
        /// \return 缓冲区容量
        size_t cap() const;
        /// 获取缓冲区已用大小
        /// \return 缓冲区已用大小
        size_t size() const;
        /// 获取缓冲区指针
        /// \return 缓冲区指针
        const char *data() const;

        // 输出
    public:
        SChar at(size_t index) const;
        std::vector<SChar> toChars() const;
        std::string toString() const;
        std::wstring toWString() const;

        // 运算符
    public:
        SChar operator[](size_t index) const;

    private:
        char *_data = nullptr;
        size_t _capacity = 0;
        size_t _size = 0;
    };
}// namespace sstr