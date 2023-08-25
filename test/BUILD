cc_test(
    name = "TestAlgol",
    srcs = [
        "TestAlgol.cpp",
    ],
    copts = select({
        "@platforms//os:windows": [
            "/utf-8",
        ],
    }),
    deps = [
        "@com_google_googletest//:gtest_main",
        "@com_sese_sstring//:sstring",
    ],
)

cc_test(
    name = "TestSString",
    srcs = [
        "TestSString.cpp",
    ],
    copts = select({
        "@platforms//os:windows": [
            "/utf-8",
        ],
    }),
    deps = [
        "@com_google_googletest//:gtest_main",
        "@com_sese_sstring//:sstring",
    ],
)

cc_test(
    name = "TestSStringBuilder",
    srcs = [
        "TestSStringBuilder.cpp",
    ],
    copts = [
        "/utf-8",
    ],
    deps = [
        "@com_google_googletest//:gtest_main",
        "@com_sese_sstring//:sstring",
    ],
)
