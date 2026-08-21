#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include <ncurses.h>

#include "interface.hpp"
#include "../command_handler/command_handler.hpp"

namespace kernel
{
    class TerminalGraphics
    {
    public:
        TerminalGraphics() = default;
        ~TerminalGraphics() = default;

        void clear();
        void drawPoint(int x, int y, char symbol = '*');
        void drawLine(int x1, int y1, int x2, int y2, char symbol = '*');
        void drawRectangle(int x, int y, int width, int height, char symbol = '*');
        void drawFilledRectangle(int x, int y, int width, int height, char symbol = '*');
        void drawCircle(int cx, int cy, int radius, char symbol = '*');
        void drawText(int x, int y, const std::string& text);
        void refresh();
        void getTerminalSize(int& rows, int& cols);
    };

    class TerminalIOH : public kernel::IShell<TerminalIOH>
    {
        /* чтоб IShell мог создать по  getInstance*/
        friend class kernel::IShell<TerminalIOH>;

    public:
        void input(const std::string& inputString);
        void output(const std::string& outputString);

        TerminalGraphics& getGraphics();

        void clear();

    private:
        TerminalIOH();
        ~TerminalIOH();
        void inputLoop();
        void processCommandQueue();

        std::string m_inputString;
        std::string m_outputString;

        std::unique_ptr<TerminalGraphics> m_graphics;

        std::atomic<bool> m_running;
        std::thread m_inputThread;

        std::queue<std::string> m_commandQueue;
        std::mutex m_queueMutex;
        std::condition_variable m_cv;

        std::atomic<bool> m_inputReady;
    };
} // namespace kernel