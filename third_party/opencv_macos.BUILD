# Description:
#   OpenCV libraries for video/image processing on MacOS

licenses(["notice"])  # BSD license

exports_files(["LICENSE"])

cc_library(
    name = "opencv",
    srcs = glob(
        [
            "lib/libopencv_core.a",
            "lib/libopencv_calib3d.a",
            "lib/libopencv_features2d.a",
            #"lib/libopencv_highgui.a",#this causes problems when compiling to wasm
            "lib/libopencv_imgcodecs.a",
            "lib/libopencv_imgproc.a",
            "lib/libopencv_video.a",
            #"lib/libopencv_videoio.a",#this causes problems when compiling to wasm
        ],
    ),
    hdrs = glob(["include/opencv2/**/*.h*"]),
    includes = ["include/"],
    linkstatic = 0,
    visibility = ["//visibility:public"],
)
