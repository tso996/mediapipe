# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

licenses(["notice"])

package(default_visibility = ["//mediapipe/examples:__subpackages__"])


DEFAULT_EMSCRIPTEN_LINKOPTS = [
  
    "-s USE_PTHREADS=0",  # Disable pthreads
    "-s USE_WEBGL2=1",
]

cc_binary(
    name = "edge-detection",
    srcs = ["hello_world.cc"],
    visibility = ["//visibility:public"],
    deps = [
         "//third_party:opencv",
        #"//mediapipe/graphs/edge_detection:mobile_calculators",
    ],
    linkopts = [
        "-s USE_PTHREADS=0",
        "-s ALLOW_MEMORY_GROWTH=0",
        "-s ASSERTIONS=1",
        "-s USE_WEBGL2=1", 
    ]
)

wasm_cc_binary(
    name = "edge-detection-wasm",
    cc_target = ":edge-detection",
)