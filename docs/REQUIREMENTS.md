### REQUIREMENTS.md

# Project Requirements

This document outlines all the dependencies required to build and run the project, both in Docker and locally, including options for macOS and Ubuntu.

---

## 1. Docker and Docker Compose

- **Description**: Used to create and run containers with the environment for building and running the project.
- **Installation**:
  - **macOS**:
    ```bash
    brew install docker docker-compose
    ```
  - **Ubuntu**:
    ```bash
    sudo apt update
    sudo apt install docker.io docker-compose
    ```
- **Commands**:
  - Build and run containers:
    ```bash
    docker-compose up --build
    ```
  - Stop containers:
    ```bash
    docker-compose down
    ```

---

## 2. Emscripten SDK

- **Description**: Used to compile C++ code into WebAssembly.
- **Installation**:
  - **macOS and Ubuntu**:
    ```bash
    git clone https://github.com/emscripten-core/emsdk.git
    cd emsdk
    ./emsdk install latest
    ./emsdk activate latest
    source ./emsdk_env.sh
    ```
- **Commands**:
  - Initialize the environment:
    ```bash
    source ~/emsdk/emsdk_env.sh
    ```
  - Build the project:
    ```bash
    emcmake cmake -S . -B build
    emmake cmake --build build
    ```

---

## 3. CMake

- **Description**: Used to generate build files.
- **Installation**:
  - **macOS**:
    ```bash
    brew install cmake
    ```
  - **Ubuntu**:
    ```bash
    sudo apt update
    sudo apt install cmake
    ```
- **Commands**:
  - Generate build files:
    ```bash
    cmake -S . -B build
    ```
  - Build the project:
    ```bash
    cmake --build build
    ```

---

## 4. Clang-Format

- **Description**: Used for automatic code formatting.
- **Installation**:
  - **macOS**:
    ```bash
    brew install clang-format
    ```
  - **Ubuntu**:
    ```bash
    sudo apt update
    sudo apt install clang-format
    ```
- **Commands**:
  - Format a file:
    ```bash
    clang-format -i <filename>
    ```

---

## 5. Python 3

- **Description**: Used to run a local server.
- **Installation**:
  - **macOS**:
    ```bash
    brew install python
    ```
  - **Ubuntu**:
    ```bash
    sudo apt update
    sudo apt install python3
    ```
- **Commands**:
  - Run the local server:
    ```bash
    python3 server.py
    ```

---

## 6. Nginx

- **Description**: Used to serve static files in the Docker container.
- **Installation**:
  - **macOS and Ubuntu**: Installed automatically via Docker.

---

## 7. C++ Libraries

- **Description**: Standard C++ libraries used in the project.
  - `<iostream>`: For input/output streams.
  - `<string>`: For string manipulation.
  - `<map>`: For storing commands.
  - `<memory>`: For `std::unique_ptr`.
  - `<sstream>`: For string streams.
  - `<vector>`: For dynamic arrays.

---



# 8 Tasks.json (for VS Code)

```
json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Docker: Build and Run Container",
            "type": "shell",
            "command": "rm -rf build && docker-compose down && docker-compose up --build",
            "group": {
                "kind": "build",
            },
            "problemMatcher": [],
            "isBackground": false,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "CMake: Build Project",
            "type": "shell",
            "command": "source ~/emsdk/emsdk_env.sh && rm -rf build && mkdir -p build && emcmake cmake -S src -B build && emmake cmake --build build -j8 && cp page/index.html page/ByteBounce.ttf page/styles.css page/webasm.js build/",
            "group": {
                "kind": "build",
            },
            "problemMatcher": [],
            "isBackground": false,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "Server: Start Local Server",
            "type": "shell",
            "command": "python3 server.py",
            "group": {
                "kind": "test",
                "isDefault": false
            },
            "problemMatcher": [],
            "isBackground": true,
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        },
        {
            "label": "Static Analysis: Run Cppcheck",
            "type": "shell",
            "command": "cppcheck --suppress=unusedFunction --enable=all  . 2> report.txt && cat report.txt",
            "problemMatcher": []
        },
        {
            "label": "Clean: Remove Build Directory",
            "type": "shell",
            "command": "rm -rf build",
            "problemMatcher": [],
            "runOptions": {
                "reevaluateOnRerun": true,
                "promptOnClose": false
            }
        }
    ]
}
```

## Full Dependency List

1. **Docker**: For containerization.
2. **Docker Compose**: For managing containers.
3. **Emscripten SDK**: For compiling C++ to WebAssembly.
4. **CMake**: For generating build files.
5. **Clang-Format**: For code formatting.
6. **Python 3**: For running a local server.
7. **Nginx**: For serving static files.
8. **C++ Libraries**: `<iostream>`, `<string>`, `<map>`, `<memory>`, `<sstream>`, `<vector>`.

---

## Example Commands for Full Project Setup

### 1. Build and Run with Docker

```bash
docker-compose up --build
```

### 2. Local Build and Run

```bash
source ~/emsdk/emsdk_env.sh
rm -rf build && mkdir build
emcmake cmake -S . -B build
emmake cmake --build build
python3 server.py
```
