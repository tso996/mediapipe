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

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //hacked code to simply fit the image to detect inside the little canvas
    //This is not really needed
    //===========================================Image to detect=========

    var fitImageOn = function (canvas, imageObj) {
      var context = canvas.getContext("2d");
      console.log("in fit image");
      console.log(canvas);
      var imageAspectRatio = imageObj.width / imageObj.height;
      var canvasAspectRatio = canvas.width / canvas.height;
      var renderableHeight, renderableWidth, xStart, yStart;

      // If image's aspect ratio is less than canvas's we fit on height
      // and place the image centrally along width
      if (imageAspectRatio < canvasAspectRatio) {
        renderableHeight = canvas.height;
        renderableWidth = imageObj.width * (renderableHeight / imageObj.height);
        xStart = (canvas.width - renderableWidth) / 2;
        yStart = 0;
      }

      // If image's aspect ratio is greater than canvas's we fit on width
      // and place the image centrally along height
      else if (imageAspectRatio > canvasAspectRatio) {
        renderableWidth = canvas.width;
        renderableHeight = imageObj.height * (renderableWidth / imageObj.width);
        xStart = 0;
        yStart = (canvas.height - renderableHeight) / 2;
      } else {
        renderableHeight = canvas.height;
        renderableWidth = canvas.width;
        xStart = 0;
        yStart = 0;
      }
      context.drawImage(
        imageObj,
        xStart,
        yStart,
        renderableWidth,
        renderableHeight
      );
    };

    async function imageToUint8ClampedArray(image, canvasDetect) {
      return new Promise((resolve, reject) => {
        //the following function call actually fits the image in the little canvas
        fitImageOn(canvasDetect, image);
        //================================
        //the following bit of code returns the proper aspect ratio and size of the image
        //we need it to put the buffer size in wasm
        //WE ARE CONVERTING THE DETECTION IMAGE TO THE CANVAS DISPLAYED ON PAGE AND THEN SENDING IT TO WASM MODULE.
        //THIS IS REQUIRED FOR THE LENGTH TO MEET MULTIPLE OF 4.
        temporaryDetectCanvas.width = image.width;
        temporaryDetectCanvas.height = image.height;
        const context = temporaryDetectCanvas.getContext("2d");
        context.drawImage(image, 0, 0);
        context.canvas.toBlob((blob) =>
          blob
            .arrayBuffer()
            .then((buffer) => {
              resolve(context.getImageData(0, 0, image.width, image.height).data);
            })
            .catch(reject)
        );
      });
    }

    const imageToDetect = new Image();
    imageToDetect.src = `./bat.jpeg`;
    const canvasDetect = document.querySelector("#canvasImageToDetect");
    let imageToDetectData = undefined;

    imageToDetect.onload = async () => {
      const view = await imageToUint8ClampedArray(imageToDetect, canvasDetect);
      // use the view...
      imageToDetectData = view;
    };
    //
    //
    //
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
      // const ax = js_wrapped_setCamMatrix(640, 480, 60, 60, 1, 1.5); // this is the settings for the image to detect
      // const imgToDetectPtr = Module.createBuffer(
      //   temporaryDetectCanvas.width,
      //   temporaryDetectCanvas.height
      // );
      // console.log("temporary canvas size");
      // console.log(temporaryDetectCanvas.width);
      // console.log(temporaryDetectCanvas.height);

      // Module.HEAP8.set(imageToDetectData, imgToDetectPtr);
      // js_wrapped_setImageToDetectHelper(
      //   imgToDetectPtr,
      //   temporaryDetectCanvas.width,
      //   temporaryDetectCanvas.height
      // );
      
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
          et.innerHTML = executionTime;

          let destroyBuffer = js_wrapped_destroyBuffer(p);

          // const resultPointer = Module.resultPointer();
          // const resultSize = Module.resultSize();
          // const resultTrackStatus = js_wrapped_getResultTrackStatus();
          // const resultView = new Float32Array(
          //   Module.HEAP8.buffer,
          //   resultPointer,
          //   resultSize
          // );
          // destroyBuffer = js_wrapped_destroyBuffer(resultPointer);
          //======================
          //============================
          //MODIFY THE AR OBJECT ORIENTATION HERE==========
          //TODO: change the video shown based on the track status
          //updateARObject(resultView);

          //===============================================
          //DISPLAYING THE PROCESSED VIDEO
          //if (resultTrackStatus == 1) {
            // console.log("wasm output playing!");
            // outputCanvas.style.visibility = "visible";
            // video.style.visibility = "hidden";
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