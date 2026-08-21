#include "terminal_ioh.hpp"

#include <chrono>
#include <cmath>
#include <iostream>

namespace kernel
{

    void TerminalGraphics::clear()
    {
        ::clear();
        refresh();
    }

    void TerminalGraphics::drawPoint(
        int x,
        int y,
        char symbol)
    {
        if (x >= 0 && x < COLS &&
            y >= 0 && y < LINES)
        {
            mvaddch(y, x, symbol);
        }
    }

    void TerminalGraphics::drawLine(
        int x1,
        int y1,
        int x2,
        int y2,
        char symbol)
    {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);

        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;

        int err = dx - dy;

        while (true)
        {
            drawPoint(x1, y1, symbol);

            if (x1 == x2 && y1 == y2)
                break;

            const int e2 = 2 * err;

            if (e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }

            if (e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }

    void TerminalGraphics::drawRectangle(
        int x,
        int y,
        int width,
        int height,
        char symbol)
    {
        for (int i = 0; i < width; ++i)
        {
            drawPoint(x + i, y, symbol);
            drawPoint(x + i, y + height - 1, symbol);
        }

        for (int i = 0; i < height; ++i)
        {
            drawPoint(x, y + i, symbol);
            drawPoint(x + width - 1, y + i, symbol);
        }
    }

    void TerminalGraphics::drawFilledRectangle(
        int x,
        int y,
        int width,
        int height,
        char symbol)
    {
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                drawPoint(x + j, y + i, symbol);
            }
        }
    }

    void TerminalGraphics::drawCircle(
        int cx,
        int cy,
        int radius,
        char symbol)
    {
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y)
        {
            drawPoint(cx + x, cy + y, symbol);
            drawPoint(cx + y, cy + x, symbol);
            drawPoint(cx - y, cy + x, symbol);
            drawPoint(cx - x, cy + y, symbol);
            drawPoint(cx - x, cy - y, symbol);
            drawPoint(cx - y, cy - x, symbol);
            drawPoint(cx + y, cy - x, symbol);
            drawPoint(cx + x, cy - y, symbol);

            if (err <= 0)
            {
                ++y;
                err += 2 * y + 1;
            }

            if (err > 0)
            {
                --x;
                err -= 2 * x + 1;
            }
        }
    }

    void TerminalGraphics::drawText(
        int x,
        int y,
        const std::string& text)
    {
        mvprintw(y, x, "%s", text.c_str());
    }

    void TerminalGraphics::refresh()
    {
        ::refresh();
    }

    void TerminalGraphics::getTerminalSize(
        int& rows,
        int& cols)
    {
        getmaxyx(stdscr, rows, cols);
    }

    TerminalIOH::TerminalIOH()
        : m_running(true),
          m_inputReady(false)
    {
        initscr();
        cbreak();
        noecho();

        keypad(stdscr, TRUE);
        curs_set(0);
        nodelay(stdscr, TRUE);

        if (has_colors())
        {
            start_color();

            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            init_pair(2, COLOR_RED, COLOR_BLACK);
            init_pair(3, COLOR_GREEN, COLOR_BLACK);
            init_pair(4, COLOR_YELLOW, COLOR_BLACK);
            init_pair(5, COLOR_BLUE, COLOR_BLACK);
            init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
            init_pair(7, COLOR_CYAN, COLOR_BLACK);
        }

        m_graphics = std::make_unique<TerminalGraphics>();

        m_inputThread =
            std::thread(&TerminalIOH::inputLoop, this);
    }

    TerminalIOH::~TerminalIOH()
    {
        m_running = false;

        if (m_inputThread.joinable())
            m_inputThread.join();

        endwin();
    }

    void TerminalIOH::input(
        const std::string& inputString)
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);

        m_commandQueue.push(inputString);
        m_inputReady = true;

        m_cv.notify_one();
    }

    void TerminalIOH::output(
        const std::string& outputString)
    {
        if (outputString.empty())
            return;

        m_graphics->drawText(
            0,
            LINES - 2,
            std::string(COLS, ' '));

        m_graphics->drawText(
            0,
            LINES - 2,
            outputString);

        m_graphics->refresh();
    }

    TerminalGraphics& TerminalIOH::getGraphics()
    {
        return *m_graphics;
    }

    void TerminalIOH::clear()
    {
        ::clear();

        if (m_graphics)
            m_graphics->refresh();
    }

    void TerminalIOH::inputLoop()
    {
        std::string currentInput;

        while (m_running)
        {
            const int ch = getch();

            if (ch == ERR)
            {
                processCommandQueue();

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(10));

                continue;
            }

            if (ch == '\n' || ch == '\r')
            {
                if (! currentInput.empty())
                {

                    m_graphics->drawText(
                        2,
                        LINES - 2,
                        std::string(COLS - 4, ' '));

                    m_graphics->drawText(
                        2,
                        LINES - 2,
                        currentInput);

                    m_graphics->refresh();

                    input(currentInput);
                    currentInput.clear();
                }
            }
            else if (ch == 127 || ch == KEY_BACKSPACE)
            {
                if (! currentInput.empty())
                {
                    currentInput.pop_back();
                }
            }
            else if (ch >= 32 && ch <= 126)
            {
                currentInput.push_back(
                    static_cast<char>(ch));
            }
            else if (ch == KEY_UP)
            {
            }

            processCommandQueue();
        }
    }

    void TerminalIOH::processCommandQueue()
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);

        while (! m_commandQueue.empty())
        {
            std::string cmd =
                m_commandQueue.front();

            m_commandQueue.pop();

            CommandHandler::getInstance().execute(cmd);
        }
    }
} // namespace kernel