#include <cstdlib>
#include <iostream>

// #include "absl/flags/flag.h"
// #include "absl/flags/parse.h"
// #include "mediapipe/framework/calculator_framework.h"
// #include "mediapipe/framework/formats/image_frame.h"
// #include "mediapipe/framework/formats/image_frame_opencv.h"
// #include "mediapipe/framework/port/file_helpers.h"
// // #include "mediapipe/framework/port/opencv_highgui_inc.h"
// #include "mediapipe/framework/port/opencv_imgproc_inc.h"
// #include "mediapipe/framework/port/opencv_video_inc.h"
// #include "mediapipe/framework/port/parse_text_proto.h"
// #include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/logging.h"
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

          int main(int argc, char** argv) {
            google::InitGoogleLogging(argv[0]);
            std::cout << "box tracking wasm loaded." << std::endl;
            return 0;
          }


          EMSCRIPTEN_KEEPALIVE
          absl::Status PrintHelloWorld() {
            std::cout << "inside printHelloWorld"<< std::endl;
            // Configures a simple graph, which concatenates 2 PassThroughCalculators.
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
            std::cout << "inside printHelloWorld1"<< std::endl;
            mediapipe::CalculatorGraph graph;
            graph.Initialize(config);
            auto status_or_poller = graph.AddOutputStreamPoller("out");
            mediapipe::OutputStreamPoller poller = std::move(status_or_poller.value());

            graph.StartRun({});
            // Give 10 input packets that contains the same std::string "Hello World!".
            std::cout << "inside printHelloWorld2"<< std::endl;
            for (int i = 0; i < 10; ++i) {
              graph.AddPacketToInputStream(
                  "in", mediapipe::MakePacket<std::string>("Hello World!").At(mediapipe::Timestamp(i)));
            }
            // Close the input stream "in".
            graph.CloseInputStream("in");
            std::cout << "inside printHelloWorld2.5"<< std::endl;
            mediapipe::Packet packet;
            // while (poller.Next(&packet)) {
            //     LOG(INFO) << packet.Get<std::string>();
            //   }
            //   return graph.WaitUntilDone();
            // Get the output packets std::string.
            for (int i = 0; i < 10; ++i) {
              std::cout <<packet.Get<std::string>()<< std::endl;
            }
            std::cout << "inside printHelloWorld3"<< std::endl;
    }
}
