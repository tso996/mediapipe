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
    "-s ERROR_ON_UNDEFINED_SYMBOLS=0",
   # "-s LLD_REPORT_UNDEFINED=1",
]

cc_binary(
    name = "object-tracking-mobile",
    srcs = ["box-tracking.cc"],
    deps = [
        "//mediapipe/calculators/core:pass_through_calculator",
        "//mediapipe/framework:calculator_graph",
        "//mediapipe/framework/port:logging",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "//mediapipe/graphs/tracking/subgraphs:object_tracking_gpu",
    ],
    linkopts = [
        "-s USE_PTHREADS=0",
        "-s ALLOW_MEMORY_GROWTH=0",
        "-s ASSERTIONS=1",
        "-s USE_WEBGL2=1", 
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",
    ]
)
wasm_cc_binary(
    name = "object-tracking-wasm",
    cc_target = ":object-tracking-mobile",
)
