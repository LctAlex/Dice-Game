#Build-time
FROM ubuntu:22.04 AS build

# Only what emscripten needs to compile with Ubuntu (xz, libatomic1)
RUN apt-get update && apt-get install -y \
    git \
    python3 \
    nodejs \
    xz-utils \
    libatomic1 \
    && rm -rf /var/lib/apt/lists/*

# Install emsdk
# emsdk (Emscripten) is a complier/convertor from C/C++ (and more) code to WebAssembly (for Web)
# Important file here: .emscripten (configuration file, done with './emsdk activate latest')
RUN git clone https://github.com/emscripten-core/emsdk.git /emsdk && \
    cd /emsdk && ./emsdk install latest && ./emsdk activate latest

ENV PATH="/emsdk:/emsdk/upstream/emscripten:${PATH}"

WORKDIR /build

# Necessary files from Raylib (no compiler needed, emsdk takes care)
# NOTE: You can directly just copy the raylib/src subfolder of raylib 
# Important files here: Makefile (configured), libray.a
COPY raylib-src-docker-web ./raylib

# Copy source + raylib + shell
# IMPORTANT NOTE: your shell.html file should be here! (tho it's usually inside raylib/src)
COPY ./assets ./assets
COPY ./src ./src


# Compile at BUILD TIME
# src/main.cpp src/AButton.cpp src/other_file.cpp -> src/*.cpp
RUN emcc src/*.cpp \
    -o dice_game.html \
    -std=c++14 -Os \
    -I ./raylib -I ./raylib/external \
    raylib/web/libraylib.a \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    -s TOTAL_MEMORY=67108864 \
    -s FORCE_FILESYSTEM=1 \
    --preload-file ./assets \
    --shell-file ./src/custom_shell.html \
    -DPLATFORM_WEB \
    -s 'EXPORTED_FUNCTIONS=["_main","_malloc","_free"]' \
    -s EXPORTED_RUNTIME_METHODS=ccall

#Run-time
FROM nginx:alpine

# Copy ONLY the final artifacts
# This is standard for nginx
COPY --from=build /build/dice_game.* /usr/share/nginx/html/
COPY --from=build /build/assets /usr/share/nginx/html/assets

EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]

# Build: docker build -t test_game .
# Run: docker run -p 8000:80 test_game 
# Open: http://localhost:8000/test_game.html                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  