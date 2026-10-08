FROM emscripten/emsdk:3.1.44

WORKDIR /app
COPY . .

RUN rm -rf build && \
    emcmake cmake \
        -S /app \
        -B /app/build \
        -DUSE_TERMINAL=OFF \
        -DUSE_WEBASM=ON && \
    emmake cmake --build /app/build -j$(nproc)

# Артефакты C++ + статика страницы → в /out
RUN mkdir -p /out && \
    cp /app/build/main.html /out/ && \
    cp /app/build/main.js   /out/ && \
    cp /app/build/main.wasm /out/ && \
    cp /app/page/index.html /out/ && \
    cp /app/page/styles.css /out/ && \
    cp /app/page/webasm.js  /out/ && \
    mkdir -p /out/fonts && \
    cp /app/page/fonts/ByteBounce.ttf        /out/fonts/ && \
    cp /app/page/fonts/UbuntuMono-Regular.ttf /out/fonts/