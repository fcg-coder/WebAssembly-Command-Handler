# WebAssembly Command Handler

# Build

```bash
mkdir build
cd build
```

## WebAssembly

Собираем CMake + make. Получаем .js файл который подключается к html странице. Копируем файлы из page в build и запускаем сервер 

```bash
source ~/emsdk/emsdk_env.sh
emcmake cmake ../src -DUSE_TERMINAL=OFF -DUSE_WEBASM=ON
make
cp ../page/index.html ../page/styles.css ../page/webasm.js .
cp ../page/fonts/*.ttf .
```

Из корня проекта:

```bash
python3 server.py
```

## Terminal

### Linux / macOS

```bash
cmake ../src -DUSE_TERMINAL=ON -DUSE_WEBASM=OFF
make 
```

Запустить 

```sh
./main
```