# Description:
#   OpenCV libraries for video/image processing on MacOS

licenses(["notice"])  # BSD license

exports_files(["LICENSE"])

cc_library(
    name = "opencv",
    srcs = glob(
        [
            "lib/libopencv_core.a",
        ],
    ),
    hdrs = glob(["include/opencv2/**/*.h*"]),
    includes = ["include/"],
    linkstatic = 0,
    visibility = ["//visibility:public"],
)
