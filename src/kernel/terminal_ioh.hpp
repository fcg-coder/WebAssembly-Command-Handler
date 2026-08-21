#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <cstdint>

#include <ncurses.h>

#include "shell_base.hpp"
#include "kernel.hpp"

namespace kernel
{
    class TerminalGraphics
    {
    public:
        TerminalGraphics() = default;
        ~TerminalGraphics() = default;

        void clear();

        void drawScreen(const uint32_t* screen, int width, int height);

        std::pair<int, int> getWindowSize();

        void refresh();

        void getTerminalSize(int& rows, int& cols);
    };

    class TerminalShell
        : public kernel::IShell<TerminalShell>
    {
        friend class kernel::IShell<TerminalShell>;

    public:
        using IShell<TerminalShell>::output;

        TerminalGraphics& getGraphics();

        void clear();

    private:
        TerminalShell();
        ~TerminalShell();

        void inputImpl(const std::string& inputString);

        void outputImpl(const std::string& outputString);

        void inputLoop();

        void processCommandQueue();

        std::string m_inputString;
        std::string m_outputString;

        std::vector<std::string> m_outputLines;
        std::string m_currentInput;

        std::mutex m_outputMutex;

        std::unique_ptr<TerminalGraphics> m_graphics;

        std::atomic<bool> m_running;
        std::thread m_inputThread;

        std::queue<std::string> m_commandQueue;
        std::mutex m_queueMutex;
        std::condition_variable m_cv;

        std::atomic<bool> m_inputReady;
    };
} // namespace kernel