#pragma once
#include <emscripten.h>
#include <iostream>
#include <atomic>

extern "C" EMSCRIPTEN_KEEPALIVE void printToJS(const char *text);
extern "C" EMSCRIPTEN_KEEPALIVE void processInput(const char *input);