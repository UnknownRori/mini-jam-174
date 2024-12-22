# Set the CMAKE_FLAGS
CMAKE_FLAGS = -G "MSYS Makefiles"

all: desktop
	./build/salvager
desktop:
	([ ! -e ./build ] && cmake $(CMAKE_FLAGS) -S . -B build) || [ -e ./build ]
	cmake --build build
	cp -r ./resources/ ./build/

web:
	([ ! -e ./web-build ] && emcmake cmake -S . -B web-build -DPLATFORM=Web) || [ -e ./web-build ]
	([ ! -e ./dist ] && mkdir dist) || [ -e ./dist ]
	cmake --build web-build
	cp -r ./resources ./dist
	# cp ./web-build/salvager.html ./dist/index.html
	cp ./index.html ./dist/index.html
	cp ./web-build/salvager.js ./dist
	cp ./web-build/salvager.data ./dist
	cp ./web-build/salvager.wasm ./dist
