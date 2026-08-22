FROM emscripten/emsdk:3.1.44 AS build

WORKDIR /app

COPY . .

RUN rm -rf build && \
    emcmake cmake \
        -S /app/src \
        -B /app/build \
        -DUSE_TERMINAL=OFF \
        -DUSE_WEBASM=ON && \
    emmake cmake --build /app/build -j$(nproc)

FROM nginx:alpine

COPY page/fonts/ByteBounce.ttf /usr/share/nginx/html/
COPY page/fonts/UbuntuMono-Regular.ttf /usr/share/nginx/html/

COPY page/index.html /usr/share/nginx/html/
COPY page/styles.css /usr/share/nginx/html/
COPY page/webasm.js /usr/share/nginx/html/

COPY --from=build /app/build/main.html /usr/share/nginx/html/
COPY --from=build /app/build/main.js /usr/share/nginx/html/
COPY --from=build /app/build/main.wasm /usr/share/nginx/html/

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]