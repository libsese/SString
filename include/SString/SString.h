#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

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

        explicit operator uint32_t() const;
    };

    extern SChar NullChar;

    extern SChar getSCharFromUTF8Char(const char *u8char);

    class SString final {
        /// 构造相关
    public:
        explicit SString() noexcept;
        SString(const char *str, size_t size);
        SString(const SString &sString) noexcept;
        SString(SString &&sString) noexcept;
        ~SString() noexcept;

        static SString fromSChars(std::vector<SChar> &chars);
        static SString fromUTF8(const char *str);
        static SString fromUCS2LE(const wchar_t *str);

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
        /// 查找字符串，索引单位是字数
        /// \param str 子串
        /// \return 子串位置
        int32_t find(const SString &str) const;
        /// 查找字符串，索引单位是字数
        /// \deprecated 查找子串的字符编码必须也是 UTF-8，否则不建议使用
        /// \param str 子串
        /// \return 子串位置
        int32_t find(const char *str) const;
        /// 查找字节串，索引单位是字节
        /// \param bytes 子串
        /// \return 子串位置
        int32_t findByBytes(const char *bytes) const;
        /// 除去字符串两端空格
        /// \note 注意是空格
        /// \return 处理后对象
        SString trim() const;
        /// 反转字符串
        /// \return 反转后对象
        SString reverse() const;
        /// 尾加字符串
        /// \param str 待尾加字符串
        /// \return 尾加结果字符串
        SString append(const SString &str) const;
        /// 尾加字符串
        /// \deprecated 尾加对象的字符串编码必须也是 UTF-8，否则不建议使用
        /// \param str 待尾加字符串
        /// \return 尾加结果字符串
        SString append(const char *str) const;
        /// 切割字符串
        /// \param str 切割标识符
        /// \return 切割结果
        std::vector<SString> split(const SString &str) const;
        /// 切割字符串
        /// \deprecated 尾加对象的字符串编码必须也是 UTF-8，否则不建议使用
        /// \param str 切割标识符
        /// \return 切割结果
        std::vector<SString> split(const char *str) const;

        // 输出
    public:
        SChar at(size_t index) const;
        std::vector<SChar> toChars() const;
        std::string toString() const;
        std::wstring toWString() const;

        std::unique_ptr<wchar_t[]> toCWString() const;

        // 运算符
    public:
        SChar operator[](size_t index) const;
        bool operator!=(const SString &str) const;
        /// 比较字符串是否不一致
        /// \deprecated 比较对象的字符编码必须也是 UTF-8，否则不建议使用
        /// \param str 待比较字符串
        /// \return 是否不一致
        bool operator!=(const char *str) const;
        bool operator==(const SString &str) const;
        /// 比较字符串是否一致
        /// \deprecated 比较对象的字符编码必须也是 UTF-8，否则不建议使用
        /// \param str 待比较字符串
        /// \return 是否一致
        bool operator==(const char *str) const;
        SString operator+(const SString &str) const;
        /// 尾加字符串
        /// \deprecated 尾加对象的字符串编码必须也是 UTF-8，否则不建议使用
        /// \param str 待尾加字符串
        /// \return 尾加结果字符串
        SString operator+(const char *str) const;
        void operator+=(const SString &str);
        /// 尾加字符串
        /// \deprecated 尾加对象的字符串编码必须也是 UTF-8，否则不建议使用
        /// \param str 待尾加字符串
        /// \return 尾加结果字符串
        void operator+=(const char *str);

    private:
        char *_data = nullptr;
        size_t _capacity = 0;
        size_t _size = 0;
    };
}// namespace sstr