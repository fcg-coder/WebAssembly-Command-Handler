
# WebAssembly Command Handler

This project is a C++ application using WebAssembly (Emscripten) that enables command handling through the browser.

### Technologies Used
- **WebAssembly**: To execute C++ code in the browser.
- **Docker**: For creating an isolated build and runtime environment.
- **C++**: The main programming language of the project.
- **Emscripten**: To compile C++ code into WebAssembly.

## Project Structure

```
.
├── docker-compose.yaml
├── Dockerfile
├── docs
│   └── uml.drawio.png
├── page
│   ├── ByteBounce.ttf
│   ├── index.html
│   ├── styles.css
│   └── webasm.js
├── README.md
├── server.py
└── src
    ├── CMakeLists.txt
    ├── command_handler
    │   ├── CMakeLists.txt
    │   ├── command_handler.cpp
    │   ├── command_handler.hpp
    │   └── commands
    │       ├── CMakeLists.txt
    │       ├── commands.cpp
    │       └── commands.hpp
    ├── graphic_lib
    │   ├── CMakeLists.txt
    │   ├── screen.cpp
    │   ├── screen.hpp
    │   └── shapes
    │       ├── CMakeLists.txt
    │       ├── shapes.cpp
    │       └── shapes.hpp
    ├── main.cpp
    ├── main.hpp
    └── README.md
```

