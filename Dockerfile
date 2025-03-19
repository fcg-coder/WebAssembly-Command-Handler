
FROM emscripten/emsdk:3.1.44 AS build


WORKDIR /app


COPY . .


RUN mkdir build

RUN emcmake cmake -S . -B build && \
    emmake cmake --build build


FROM nginx:alpine


COPY index.html /usr/share/nginx/html/
COPY --from=build /app/build/main.html /usr/share/nginx/html/
COPY --from=build /app/build/main.js /usr/share/nginx/html/
COPY --from=build /app/build/main.wasm /usr/share/nginx/html/


EXPOSE 80


CMD ["nginx", "-g", "daemon off;"]