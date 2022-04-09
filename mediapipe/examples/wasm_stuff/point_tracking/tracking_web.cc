// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An example of sending OpenCV webcam frames into a MediaPipe graph.
#include <cstdlib>
#include <iostream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
// #include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

// #include <emscripten/bind.h>
#include <emscripten.h>

// using namespace emscripten;
#define PI 3.14159265
#include <math.h>

using namespace cv;
//using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
extern "C"{
    constexpr char kInputStream[] = "input_video";
    constexpr char kOutputStream[] = "output_video";
    constexpr char kWindowName[] = "MediaPipe";

    bool frame_available = true;

    mediapipe::CalculatorGraph graph;
    mediapipe::CalculatorGraphConfig config =
          mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(R"pb(
            input_stream: "in"
            output_stream: "out"
            node {
              calculator: "PassThroughCalculator"
              input_stream: "in"
              output_stream: "out1"
            }
            node {
              calculator: "PassThroughCalculator"
              input_stream: "out1"
              output_stream: "out"
            }
          )pb");


    int result[5];//return from function that contains the output buffer and size


        void set_frame_available(){
          frame_available = true;
        }

        void set_frame_unavailable(){
          frame_available = false;
        }

        EMSCRIPTEN_KEEPALIVE
        uint8_t* createBuffer(int width, int height) {
                //std::cout << "buffer created.." << std::endl;
                return (uint8_t*)malloc(width * height * 4 * sizeof(uint8_t));
              }
        EMSCRIPTEN_KEEPALIVE
        void destroyBuffer(uint8_t* p) {
                //std::cout << "buffer destroyed.." << std::endl;
                free(p);
              }

        EMSCRIPTEN_KEEPALIVE
        absl::Status initialise_and_run_graph(){
          std::cout << "Initialising calculator graph.." << std::endl;
          MP_RETURN_IF_ERROR(graph.Initialize(config));
          std::cout << "Calculator graph run started.." << std::endl;
          ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller,
                          graph.AddOutputStreamPoller(kOutputStream));
          graph.StartRun({});
          std::cout << "about to start processing frames" << std::endl;
        }

        //call this once from the js before passing frames in loop
        absl::Status initialize_proxy(){
          
        }

        EMSCRIPTEN_KEEPALIVE
        int runMainProcess(uint8_t* img_in, int width, int height) {
          
            auto t1 = high_resolution_clock::now();  
        //======================================= run execution timer

            if(!frame_available){
              return 1;
            }
            cv::Mat camera_frame_raw;
            //capture >> camera_frame_raw;
            int rows = 480; //height
            int cols = 640; //width
            camera_frame_raw = Mat(rows, cols, CV_8UC4);
            camera_frame_raw.data = img_in;

            if (camera_frame_raw.empty()) { 
                return 0; 
            }
            Mat camera_frame;
            cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);
            flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);

            // Wrap Mat into an ImageFrame.
            auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
                mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
                mediapipe::ImageFrame::kDefaultAlignmentBoundary);
            Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
            camera_frame.copyTo(input_frame_mat);

            // Send image packet into the graph.
            size_t frame_timestamp_us =
                (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;
                graph.AddPacketToInputStream(
                kInputStream, mediapipe::Adopt(input_frame.release())
                                  .At(mediapipe::Timestamp(frame_timestamp_us)));

            // Get the graph result packet, or stop if that fails.
            mediapipe::Packet packet;
            // if (!poller.Next(&packet)) {
            //   std::cout << "poller.Next error. No next packet.." << std::endl;
            //    return 5;
            //   //break;
            // }
            auto& output_frame = packet.Get<mediapipe::ImageFrame>();

            // Convert back to opencv for display or saving.
            Mat output_frame_mat = mediapipe::formats::MatView(&output_frame);
            cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);

            //arranging the output into arrays to be accessed by js
            //uint8_t* outputBuffer1 = (uint8_t*)malloc(4 * 4 * sizeof(uint8_t));//the box coordinates
            uint8_t* outputVideoBuffer = (uint8_t*)malloc(cols * rows * 4 * sizeof(uint8_t));
            outputVideoBuffer = output_frame_mat.data;
            result[3] = (int)outputVideoBuffer;
            result[4] = rows*cols*4;

            //===========================================
            //execution time....
                  auto t2 = high_resolution_clock::now();
                  auto ms_int = duration_cast<milliseconds>(t2 - t1);
                  duration<double, std::milli> ms_double = t2 - t1;
                  return ms_int.count();
        // auto t1 = high_resolution_clock::now();  
        //     //============================================
        //       Mat matrix1;
        //       Mat transformationMatCurrentFrame;
        //       uint8_t* img_out;
        //       size_t size;
        //       int rows = height;
        //       int cols = width;
        //       //============
        
        //       matrix1 = Mat(rows, cols, CV_8UC4);
        //       matrix1.data = img_in;
        //       uint8_t* outputVideoBuffer = (uint8_t*)malloc(cols * rows * 4 * sizeof(uint8_t));
        //       outputVideoBuffer = matrix1.data;
        //       result[3] = (int)outputVideoBuffer;
        //       result[4] = rows*cols*4;
        //       matrix1.release();

        //       //===========================================
        //       //execution time....
        //       auto t2 = high_resolution_clock::now();
        //       auto ms_int = duration_cast<milliseconds>(t2 - t1);
        //       duration<double, std::milli> ms_double = t2 - t1;
        //       return ms_int.count();

        }

        EMSCRIPTEN_KEEPALIVE
        int getVideoResultPointer() {
                return result[3];//gets the pointer to the output buffer image
              }
        EMSCRIPTEN_KEEPALIVE
        int getVideoResultSize() {
                return result[4];
              }
        EMSCRIPTEN_KEEPALIVE
        void shut_down_graph(){
          std::cout << "Shutting down.." << std::endl;
          graph.CloseInputStream(kInputStream);
          graph.WaitUntilDone();
        }

        int main(int argc, char** argv) {
          std::cout << "box tracking wasm loaded." << std::endl;
          return 0;
        }

        // EMSCRIPTEN_BINDINGS(tracking_module) {
        //   function("createBuffer", &createBuffer,allow_raw_pointers());
        //   function("destroyBuffer", &destroyBuffer,allow_raw_pointers());
        //   function("initialise_and_run_graph", &initialise_and_run_graph);
        //   function("runMainProcess", &runMainProcess,allow_raw_pointers());
        //   function("getVideoResultPointer", &getVideoResultPointer);
        //   function("getVideoResultSize", &getVideoResultSize);
        //   function("shut_down_graph", &shut_down_graph);
        // }
}