
# WebAssembly Command Handler

This project is a C++ application using WebAssembly (Emscripten) that enables command handling through the browser.

### Technologies Used
- **WebAssembly**: To execute C++ code in the browser.
- **Docker**: For creating an isolated build and runtime environment.
- **C++**: The main programming language of the project.
- **Emscripten**: To compile C++ code into WebAssembly.

## Project Structure

 
```sh
.
├── docker-compose.yaml                     # Docker Compose configuration.
├── Dockerfile                              # Docker image build instructions.
├── docs
│   └── uml.drawio.png                      # Project architecture diagram.
├── page
│   ├── ByteBounce.ttf                      # Custom font.
│   ├── index.html                          # Web interface HTML.
│   ├── styles.css                          # Web interface styles.
│   └── webasm.js                           # WebAssembly interaction script.
├── README.md                               # Project overview.
├── server.py                               # Web application server.
└── src
    ├── CMakeLists.txt                      # Source directory build configuration.
    ├── command_handler
    │   ├── CMakeLists.txt                  # Command handler build configuration.
    │   ├── command_handler.cpp             # Command handler implementation.
    │   ├── command_handler.hpp             # Command handler header.
    │   └── commands
    │       ├── CMakeLists.txt              # Commands module build configuration.
    │       ├── commands.cpp                # Commands implementation.
    │       └── commands.hpp                # Commands header.
    ├── graphic_lib
    │   ├── CMakeLists.txt                  # Graphics library build configuration.
    │   ├── screen.cpp                      # Screen rendering implementation.
    │   ├── screen.hpp                      # Screen rendering header.
    │   └── shapes
    │       ├── CMakeLists.txt              # Shapes module build configuration.
    │       ├── shapes.cpp                  # Shapes implementation.
    │       └── shapes.hpp                  # Shapes header.
    ├── main.cpp                            # Application entry point.
    ├── main.hpp                            # Main logic header.
    └── README.md                           # Source directory documentation.
```
 
