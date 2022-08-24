#pragma once
#include <cstddef>

namespace sstr {

    class SString final {
        /// 构造相关
    public:
        explicit SString() noexcept;
        SString(const SString &sString) noexcept;
        SString(SString &&sString) noexcept;
        ~SString() noexcept;

        static SString fromUTF8(const char *);
//        static SString fromUTF16(const wchar_t *);
//        static SString fromUTF32(const wchar_t *);

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

    private:
        char *_data = nullptr;
        size_t _capacity = 0;
        size_t _size = 0;
    };
}// namespace sstr