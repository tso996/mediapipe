// var Module = {
//   onRuntimeInitialized: function() {
  function onWasmLoaded() {
    console.log("output.js is loaded!");  

    console.log("video process started running.");
    let js_wrapped_createBuffer = Module.cwrap("createBuffer", "number", [
      "number",
      "number",
    ]);
    let js_wrapped_destroyBuffer = Module.cwrap("destroyBuffer", "", ["number"]);
    let js_wrapped_initialise_and_run_graph =  Module.cwrap("initialise_and_run_graph","string");
    let js_wrapped_shut_down_graph = Module.cwrap("shut_down_graph","")
    let js_wrapped_runMainProcess = Module.cwrap(
      "runMainProcess",
      "",
      ["number", "number", "number"]
    );
  
    let js_wrapped_getVideoResultPointer = Module.cwrap(
      "getVideoResultPointer",
      "number"
    );
    let js_wrapped_getVideoResultSize = Module.cwrap(
      "getVideoResultSize",
      "number"
    );

    //=======
      
    console.log("video process started running.");
    
    let fps = 30;
    let videoHeight = 480;
    let videoWidth = 640;
    let video = document.querySelector("#video");
    let cnv = document.createElement("canvas");
    cnv.width = videoWidth;
    cnv.height = videoHeight;
    let tempCanvasCtx = cnv.getContext("2d");
    let outputCanvas = document.querySelector("#outputCanvas");
    outputCanvas.width = videoWidth;
    outputCanvas.height = videoHeight;
    let et = document.createElement("div");
    let etParent = document.querySelector(".jumbotron");
    etParent.appendChild(et);
    let temporaryDetectCanvas = document.createElement("canvas");
    //================================
    let outputCanvasCtx = outputCanvas.getContext("2d");
    let animationLoopId = undefined;
    let stopVideo = false;
    //===================================================
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Basic code for getting the video stream from cam
    //===================================================
    let devices = navigator.mediaDevices.enumerateDevices();
    console.log("here are the devices", devices);
    let init = async function () {
      const stream = await navigator.mediaDevices.getUserMedia({
        video: {
          width: videoWidth,
          height: videoHeight,
          facingMode: "environment",
        },
      });
      video.srcObject = stream;
      video.setAttribute("playsinline", "true");
      await video.play();
    };

    let captureVideoImage = function () {
      tempCanvasCtx.drawImage(video, 0, 0);
      const imageData = tempCanvasCtx.getImageData(0, 0, videoWidth, videoHeight);
      return imageData;
    };
    //=========================================================
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //console.log(context.drawImage(video, 0, 0));
    let looper = async function (fps) {
      const initialisations = await init();
      let mPInitStatus = js_wrapped_initialise_and_run_graph();
      console.log("mp graph initialization status: "+mPInitStatus);
 
      // Module.destroyBuffer(imgToDetectPtr);

      //initialization is over
      //===========================================
      (function loop() {
        let begin = Date.now();
        try {
          const imageFrameData = captureVideoImage();
          const p = js_wrapped_createBuffer(videoWidth,videoHeight);//Module.createBuffer(videoWidth, videoHeight);
          Module.HEAP8.set(imageFrameData.data, p);

          let executionTime = js_wrapped_runMainProcess(
            p,
            videoWidth,
            videoHeight
          ); //width, height
          et.innerHTML = executionTime+"ms";

          let destroyBuffer = js_wrapped_destroyBuffer(p);

    
            const videoResultPointer = js_wrapped_getVideoResultPointer();
            const videoResultSize = js_wrapped_getVideoResultSize();

            const videoResultView = new Uint8Array(
              Module.HEAP8.buffer,
              videoResultPointer,
              videoResultSize
            );
            const videoResult = new Uint8Array(videoResultView);

            destroyBuffer = js_wrapped_destroyBuffer(videoResultPointer);

            //:Use U8A to create image data object:
            let imageDataArrayClamped = new Uint8ClampedArray(
              videoResultView,
              videoWidth,
              videoHeight
            );
            var outputImageData = new ImageData(
              imageDataArrayClamped,
              videoWidth,
              videoHeight
            );
            outputCanvasCtx.putImageData(outputImageData, 0, 0);
          // } else {
          //   console.log("not detected!");
          //   outputCanvas.style.visibility = "hidden";
          //   video.style.visibility = "visible";
          // }
        } catch (e) {
          console.log("error is: ", e);
        }
        //====================================
        if (!stopVideo) {
          let delay = 1000 / fps - (Date.now() - begin);
          setTimeout(loop, delay);
        }
        // animationLoopId = window.requestAnimationFrame(loop);
      })();
    };
    looper(fps);

    //press esc to stop the video process
    let a = document.addEventListener("keydown", function (e) {
      if (e.key === "Escape") {
        stop(e);
      }
    });

    //==================
    function stop(e) {
      console.log("video stopped!");
      js_wrapped_shut_down_graph();
      var stream = video.srcObject;
      var tracks = stream.getTracks();

      for (var i = 0; i < tracks.length; i++) {
        var track = tracks[i];
        track.stop();
      }
      video.srcObject = null;
      stopVideo = true;
    }
 }
//}