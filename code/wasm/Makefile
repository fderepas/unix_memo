wasm.js: wasm.cc
	@if [ -z "`which em++`"  ]; then echo 'Error, "em++" not found. Please install Emscripten SDK.'; exit 1 ; fi
	em++ -DJS_BINDINGS -o $@ -Wall -g -std=c++20 wasm.cc --bind

server:
	python -m http.server 8080

c:wasm.exe
	./$<

wasm.exe: wasm.cc
	g++ -Wall -g -std=c++20 $< -o $@

clean:
	rm -rf *~ *.o *.exe wasm.js wasm.wasm
