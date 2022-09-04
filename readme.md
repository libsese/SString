# SString 字符串库

跨平台的 UTF-8 字符串库

- Windows👌

- macOS 👌

- Linux👌

## 注意：

在 Windows 下使用 MSVC 编译时请确保源代码文件使用 **UTF-8 With NO BOM** 保存，并且添加 **/utf-8** 编译选项。

CMake 配置示例

```cmake
add_compile_options("/utf-8")
```

*这些改动都是为了使编译器在编码字符串进入二进制文件时，依旧保持 UTF-8 的编码方式。*


