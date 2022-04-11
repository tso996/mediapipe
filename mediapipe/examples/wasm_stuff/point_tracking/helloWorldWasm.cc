// #include <cstdlib>
// #include <iostream>

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

#include <emscripten/bind.h>
//#include <emscripten.h>

//extern "C"{

          //EMSCRIPTEN_KEEPALIVE
    int PrintHelloWorld() {
                std::cout << "flag 1"<< std::endl;
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

            mediapipe::CalculatorGraph graph;
            graph.Initialize(config);
            auto status_or_poller = graph.AddOutputStreamPoller("out");
            mediapipe::OutputStreamPoller poller = std::move(status_or_poller.value());
            graph.StartRun({});
            std::cout << "flag 2"<< std::endl;
            // Give 10 input packets that contains the same std::string "Hello World!".
            for (int i = 0; i < 10; ++i) {
                graph.AddPacketToInputStream(
                    "in", mediapipe::MakePacket<std::string>("Hello World!").At(mediapipe::Timestamp(i)));
            }
            // Close the input stream "in".
            graph.CloseInputStream("in");
            std::cout << "flag 3"<< std::endl;
            mediapipe::Packet packet;
            // Get the output packets std::string.
            while (poller.Next(&packet)) {
                std::cout << "flag 4"<< std::endl;
                std::cout << poller.QueueSize()<< std::endl;
                std::cout << packet.Get<std::string>()<<std::endl;
            }
            // return graph.WaitUntilDone();
            std::cout << "flag 5"<< std::endl;
            return 1;
    }
        int main(int argc, char** argv) {
            google::InitGoogleLogging(argv[0]);
            std::cout << "hello world wasm file loaded." << std::endl;
            PrintHelloWorld();
            return 0;
          }


     EMSCRIPTEN_BINDINGS(tracking_module) {
             emscripten::function("PrintHelloWorld", &PrintHelloWorld);
          
     }
//}
