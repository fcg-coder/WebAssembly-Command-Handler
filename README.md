
# WebAssembly Command Handler

This project is a C++ application using WebAssembly (Emscripten) that enables command handling through the browser.

### Technologies Used
- **WebAssembly**: To execute C++ code in the browser.
- **Docker**: For creating an isolated build and runtime environment.
- **C++**: The main programming language of the project.
- **Emscripten**: To compile C++ code into WebAssembly.

## Project Structure

```
webAsm/
├── src/
│   ├── main.cpp                
│   ├── main.hpp                
│   ├── command_handler/
│   │   ├── command_handler.cpp  
│   │   ├── command_handler.hpp  
├── CMakeLists.txt              
└── README.md                   
```

