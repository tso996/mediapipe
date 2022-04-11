var Module = {
   onRuntimeInitialized: function() {
  console.log("wasm file ready!")
  //let js_wrapped_print_hello_world = Module.cwrap("PrintHelloWorld", "string");
  // const myTimeout = setTimeout(function() {
  //   //js_wrapped_print_hello_world();
  //   let a = Module.PrintHelloWorld();
  //   console.log("this is the output from wasm: ", a);
  // }, 3000);
  
 }
}
