#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

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
        void drawPoint(int x, int y, char symbol = '*');
        void drawLine(int x1, int y1, int x2, int y2, char symbol = '*');
        void drawRectangle(int x, int y, int width, int height, char symbol = '*');
        void drawFilledRectangle(int x, int y, int width, int height, char symbol = '*');
        void drawCircle(int cx, int cy, int radius, char symbol = '*');
        void drawText(int x, int y, const std::string& text);
        void refresh();
        void getTerminalSize(int& rows, int& cols);
    };

    class TerminalShell : public kernel::IShell<TerminalShell>
    {
        /* чтоб IShell мог создать по getInstance*/
        friend class kernel::IShell<TerminalShell>;

    public:
        /* чтобы он видел output с аргументами !  */
        using IShell<TerminalShell>::output;

        TerminalGraphics& getGraphics();

        void clear();
        void setScreenMode(bool enabled);
        bool isScreenMode() const;
        void renderScreen(uint32_t* screenBuffer, int width, int height);

    private:
        TerminalShell();
        ~TerminalShell();

        void inputImpl(const std::string& inputString);
        void outputImpl(const std::string& outputString);

        void inputLoop();
        void processCommandQueue();
        void renderScreenBufferToTerminal(uint32_t* buffer, int width, int height);

        std::string m_inputString;
        std::string m_outputString;

        // История вывода терминала
        std::vector<std::string> m_outputLines;

        // Текущий ввод пользователя
        std::string m_currentInput;

        // Защита истории вывода
        std::mutex m_outputMutex;

        std::unique_ptr<TerminalGraphics> m_graphics;

        std::atomic<bool> m_running;
        std::thread m_inputThread;

        std::queue<std::string> m_commandQueue;
        std::mutex m_queueMutex;
        std::condition_variable m_cv;

        std::atomic<bool> m_inputReady;

        // НОВЫЕ ПОЛЯ ДЛЯ SCREEN MODE
        bool m_screenMode = false;
        int m_screenWidth = 0;
        int m_screenHeight = 0;
        std::vector<std::string> m_history;
        int m_historyIndex = -1;
    };
} // namespace kernel