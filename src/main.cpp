#include "main.hpp"
#include "command_handler/command_handler.hpp"
#include <iostream>
#include <streambuf>
#include <string>
#include <atomic>

std::atomic<bool> running(true);


#ifndef DEBUG

extern "C" EMSCRIPTEN_KEEPALIVE void printToJS(const char* text) {
    EM_ASM({
        const output = document.getElementById('output');
        if (output) {
            output.innerHTML += UTF8ToString($0) + "<br>";  
            output.scrollTop = output.scrollHeight;   
        }
    }, text);
}

extern "C" EMSCRIPTEN_KEEPALIVE void processInput(const char* input) {
    std::string command(input);
    CommandHandler::getInstance()->setCommand(command);
}


class JSStreamBuffer : public std::streambuf {
private:
    std::string buffer;

protected:
    virtual int overflow(int c) override {
        if (c != EOF) {
            buffer += static_cast<char>(c);  
            if (c == '\n') {  
                printToJS(buffer.c_str());
                buffer.clear();
            }
        }
        return c;
    }

    virtual int sync() override {
        if (!buffer.empty()) {  
            printToJS(buffer.c_str());
            buffer.clear();
        }
        return 0;
    }
};

JSStreamBuffer jsStreamBuffer;
std::ostream jsCout(&jsStreamBuffer);

#endif

int main() { 
     
    #ifndef DEBUG
    std::cout.rdbuf(&jsStreamBuffer);  
    #endif
    std::cout << "Start" << std::endl;
    return 0;
}