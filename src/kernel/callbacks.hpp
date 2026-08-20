#pragma once

#include <emscripten.h>
#include <emscripten/bind.h>
#include "kernel.hpp"
#include "web_ioh.hpp"

extern "C" EMSCRIPTEN_KEEPALIVE kernel::InputOutputMode getMode();

extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen();

extern "C" EMSCRIPTEN_KEEPALIVE void printToJS(const char* text);

extern "C" EMSCRIPTEN_KEEPALIVE void processInput(const char* input);

extern "C" EMSCRIPTEN_KEEPALIVE void setSize(int height, int width);

extern "C" EMSCRIPTEN_KEEPALIVE void pressButton(const char* input);

extern "C" EMSCRIPTEN_KEEPALIVE void initScreen();

extern "C" EMSCRIPTEN_KEEPALIVE void initKernel()
{
    Kernel::init(web::WebAsmIOH::getInstance());
}