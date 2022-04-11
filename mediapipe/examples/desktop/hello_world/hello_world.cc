// // Copyright 2019 The MediaPipe Authors.
// //
// // Licensed under the Apache License, Version 2.0 (the "License");
// // you may not use this file except in compliance with the License.
// // You may obtain a copy of the License at
// //
// //      http://www.apache.org/licenses/LICENSE-2.0
// //
// // Unless required by applicable law or agreed to in writing, software
// // distributed under the License is distributed on an "AS IS" BASIS,
// // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// // See the License for the specific language governing permissions and
// // limitations under the License.
// //
// // A simple example to print out "Hello World!" from a MediaPipe graph.

// #include "mediapipe/framework/calculator_graph.h"
// #include "mediapipe/framework/port/logging.h"
// #include "mediapipe/framework/port/parse_text_proto.h"
// #include "mediapipe/framework/port/status.h"

// namespace mediapipe {

// absl::Status PrintHelloWorld() {
//   // Configures a simple graph, which concatenates 2 PassThroughCalculators.
//   CalculatorGraphConfig config =
//       ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
//         input_stream: "in"
//         output_stream: "out"
//         node {
//           calculator: "PassThroughCalculator"
//           input_stream: "in"
//           output_stream: "out1"
//         }
//         node {
//           calculator: "PassThroughCalculator"
//           input_stream: "out1"
//           output_stream: "out"
//         }
//       )pb");

//   CalculatorGraph graph;
//   MP_RETURN_IF_ERROR(graph.Initialize(config));
//   ASSIGN_OR_RETURN(OutputStreamPoller poller,
//                    graph.AddOutputStreamPoller("out"));
//   MP_RETURN_IF_ERROR(graph.StartRun({}));
//   // Give 10 input packets that contains the same std::string "Hello World!".
//   for (int i = 0; i < 10; ++i) {
//     MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
//         "in", MakePacket<std::string>("Hello World!").At(Timestamp(i))));
//   }
//   // Close the input stream "in".
//   MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));
//   mediapipe::Packet packet;
//   // Get the output packets std::string.
//   while (poller.Next(&packet)) {
//     LOG(INFO) << packet.Get<std::string>();
//   }
//   return graph.WaitUntilDone();
// }
// }  // namespace mediapipe

// int main(int argc, char** argv) {
//   google::InitGoogleLogging(argv[0]);
//   CHECK(mediapipe::PrintHelloWorld().ok());
//   //std::cout << "hello world!!!!!!!!!" << std::endl;
//   return 0;
// }

// // int main(int argc, char** argv) {
// //   std::cout << "hello world!!!!!!!!!" << std::endl;
// //   return 0;
// // }
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
// A simple example to print out "Hello World!" from a MediaPipe graph.

#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

//namespace mediapipe {

int PrintHelloWorld() {
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
  // Give 10 input packets that contains the same std::string "Hello World!".
  for (int i = 0; i < 10; ++i) {
    graph.AddPacketToInputStream(
        "in", mediapipe::MakePacket<std::string>("Hello World!").At(mediapipe::Timestamp(i)));
  }
  // Close the input stream "in".
  graph.CloseInputStream("in");
  mediapipe::Packet packet;
  // Get the output packets std::string.
  while (poller.Next(&packet)) {
    std::cout << poller.QueueSize()<< std::endl;
    std::cout << packet.Get<std::string>()<<std::endl;
  }
  // return graph.WaitUntilDone();
}
//}  // namespace mediapipe

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  PrintHelloWorld();
  //std::cout << "hello world!!!!!!!!!" << std::endl;
  return 0;
}



