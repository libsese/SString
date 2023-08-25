cc_library(
    name = "sstring",
    srcs = [
        "src/SString.cpp",
        "src/SStringBuilder.cpp",
        "src/algorithm.cpp",
    ],
    hdrs = [
        "include/SString/SString.h",
        "include/SString/SStringBuilder.h",
        "include/SString/algorithm.h",
    ],
    copts = select({
        "@platforms//os:windows": [
            "/utf-8",
        ],
    }),
    includes = [
        "include",
    ],
    visibility = ["//visibility:public"],
)
