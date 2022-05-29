---
layout: default
title: Home
nav_order: 1
---

![MediaPipe](docs/images/mediapipe_small.png)

--------------------------------------------------------------------------------

## Live ML anywhere

[MediaPipe](https://google.github.io/mediapipe/) offers cross-platform, customizable
ML solutions for live and streaming media.

![accelerated.png](docs/images/accelerated_small.png)                                                               | ![cross_platform.png](docs/images/cross_platform_small.png)
:------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------:
This repo is heavily modified to make it work on M1 mac and compile the MediaPipe modules to WebAssembly.

```mediapipe/mediapipe/examples/wasm_stuff```

This folder contains the files that I had experimented with to compile specific modules to WebAssembly.

The "tracking module webassembly" folder contains the MediaPipe box tracking modules which use OpenCV modules compiled into a WASM file and it's js binding file.