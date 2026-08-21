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

    TerminalShell::TerminalShell()
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
            std::thread(&TerminalShell::inputLoop, this);
    }

    TerminalShell::~TerminalShell()
    {
        m_running = false;

        if (m_inputThread.joinable())
            m_inputThread.join();

        endwin();
    }

    void TerminalShell::inputImpl(const std::string& inputString)
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);

        m_commandQueue.push(inputString);
        m_inputReady = true;

        m_cv.notify_one();
    }

    void TerminalShell::outputImpl(const std::string& outputString)
    {
        if (outputString.empty())
            return;

        std::lock_guard<std::mutex> lock(m_outputMutex);

        // Разбиваем output по строкам
        std::string line;

        for (char c : outputString)
        {
            if (c == '\n')
            {
                m_outputLines.push_back(line);
                line.clear();
            }
            else
            {
                line += c;
            }
        }

        if (! line.empty())
            m_outputLines.push_back(line);

        // Ограничиваем историю
        const int maxOutputLines = std::max(1, LINES - 2);

        while (static_cast<int>(m_outputLines.size()) > maxOutputLines)
            m_outputLines.erase(m_outputLines.begin());

        if (! m_screenMode) // ТОЛЬКО ЭТО ДОБАВЛЕНО
        {
            ::clear();

            for (int i = 0; i < static_cast<int>(m_outputLines.size()); ++i)
            {
                m_graphics->drawText(
                    0,
                    i,
                    m_outputLines[i]);
            }

            // Строка ввода
            m_graphics->drawText(
                0,
                LINES - 1,
                "> " + m_currentInput);

            m_graphics->refresh();
        }
    }

    TerminalGraphics& TerminalShell::getGraphics()
    {
        return *m_graphics;
    }

    void TerminalShell::clear()
    {
        ::clear();

        if (m_graphics)
            m_graphics->refresh();
    }

    // НОВЫЙ МЕТОД
    void TerminalShell::setScreenMode(bool enabled)
    {
        m_screenMode = enabled;

        if (enabled)
        {
            int rows, cols;
            m_graphics->getTerminalSize(rows, cols);
            m_screenWidth = cols;
            m_screenHeight = rows - 2;

            ::clear();
            m_graphics->drawText(0, 0, "=== SCREEN MODE ===");
            m_graphics->drawText(0, 1, "Press ESC to exit");
            m_graphics->refresh();
        }
        else
        {
            m_screenWidth = 0;
            m_screenHeight = 0;
            ::clear();
            m_outputLines.clear();
            m_graphics->refresh();
        }
    }

    // НОВЫЙ МЕТОД
    bool TerminalShell::isScreenMode() const
    {
        return m_screenMode;
    }

    // НОВЫЙ МЕТОД
    void TerminalShell::renderScreen(uint32_t* screenBuffer, int width, int height)
    {
        if (! m_screenMode || ! screenBuffer)
            return;

        std::lock_guard<std::mutex> lock(m_outputMutex);

        ::clear();

        int termRows, termCols;
        m_graphics->getTerminalSize(termRows, termCols);

        int renderWidth = std::min(width, termCols);
        int renderHeight = std::min(height, termRows - 2);

        const char* gradient = " .:-=+*#%@";
        const int gradientSize = 10;

        for (int y = 0; y < renderHeight; ++y)
        {
            for (int x = 0; x < renderWidth; ++x)
            {
                uint32_t color = screenBuffer[y * width + x];
                uint8_t r = (color >> 16) & 0xFF;
                uint8_t g = (color >> 8) & 0xFF;
                uint8_t b = color & 0xFF;

                if (r == 0 && g == 0 && b == 0)
                {
                    mvaddch(y + 1, x, ' ');
                    continue;
                }

                int brightness = (r * 30 + g * 59 + b * 11) / 100;
                int index = (brightness * gradientSize) / 256;
                if (index >= gradientSize)
                    index = gradientSize - 1;

                if (has_colors())
                {
                    int colorPair = 1;
                    if (r > 200 && g > 200 && b > 200)
                        colorPair = 1;
                    else if (r > 200 && g < 100 && b < 100)
                        colorPair = 2;
                    else if (r < 100 && g > 200 && b < 100)
                        colorPair = 3;
                    else if (r < 100 && g < 100 && b > 200)
                        colorPair = 5;
                    else if (r > 200 && g > 200 && b < 100)
                        colorPair = 4;
                    else if (r > 200 && g < 100 && b > 200)
                        colorPair = 6;
                    else if (r < 100 && g > 200 && b > 200)
                        colorPair = 7;

                    attron(COLOR_PAIR(colorPair));
                }

                mvaddch(y + 1, x, gradient[index]);

                if (has_colors())
                {
                    attroff(COLOR_PAIR(1));
                }
            }
        }

        attron(A_REVERSE);
        std::string status = " SCREEN MODE | Buffer: " +
                             std::to_string(width) + "x" + std::to_string(height) +
                             " | Press ESC to exit ";
        m_graphics->drawText(0, 0, status);
        attroff(A_REVERSE);

        m_graphics->drawText(0, termRows - 1, " Press ESC to exit ");
        m_graphics->refresh();
    }

    void TerminalShell::inputLoop()
    {
        std::string currentInput;

        m_graphics->drawText(
            0,
            LINES - 2,
            "> ");

        m_graphics->refresh();

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

            // НОВОЕ: Обработка ESC в screen mode
            if (m_screenMode)
            {
                if (ch == 27) // ESC
                {
                    setScreenMode(false);
                    output("Exited screen mode\n");
                    continue;
                }
                continue;
            }

            if (ch == '\n' || ch == '\r')
            {
                if (! currentInput.empty())
                {
                    // Сохраняем в историю
                    if (m_history.empty() || m_history.back() != currentInput)
                    {
                        m_history.push_back(currentInput);
                    }
                    m_historyIndex = m_history.size();

                    // Отправляем команду
                    input(currentInput);

                    currentInput.clear();

                    // Очищаем строку ввода
                    m_graphics->drawText(
                        0,
                        LINES - 2,
                        std::string(COLS, ' '));

                    m_graphics->drawText(
                        0,
                        LINES - 2,
                        "> ");

                    m_graphics->refresh();
                }

                continue;
            }

            if (ch == 127 || ch == KEY_BACKSPACE)
            {
                if (! currentInput.empty())
                {
                    currentInput.pop_back();

                    m_graphics->drawText(
                        0,
                        LINES - 2,
                        std::string(COLS, ' '));

                    m_graphics->drawText(
                        0,
                        LINES - 2,
                        "> " + currentInput);

                    m_graphics->refresh();
                }

                continue;
            }

            if (ch >= 32 && ch <= 126)
            {
                currentInput += static_cast<char>(ch);

                m_graphics->drawText(
                    0,
                    LINES - 2,
                    "> " + currentInput);

                m_graphics->refresh();

                continue;
            }

            // НОВОЕ: История команд
            if (ch == KEY_UP)
            {
                if (! m_history.empty())
                {
                    if (m_historyIndex > 0)
                        m_historyIndex--;
                    else
                        m_historyIndex = 0;

                    currentInput = m_history[m_historyIndex];

                    m_graphics->drawText(0, LINES - 2, std::string(COLS, ' '));
                    m_graphics->drawText(0, LINES - 2, "> " + currentInput);
                    m_graphics->refresh();
                }
                continue;
            }

            if (ch == KEY_DOWN)
            {
                if (! m_history.empty())
                {
                    if (m_historyIndex < static_cast<int>(m_history.size()) - 1)
                        m_historyIndex++;
                    else
                        m_historyIndex = m_history.size();

                    if (m_historyIndex < static_cast<int>(m_history.size()))
                    {
                        currentInput = m_history[m_historyIndex];
                    }
                    else
                    {
                        currentInput.clear();
                    }

                    m_graphics->drawText(0, LINES - 2, std::string(COLS, ' '));
                    m_graphics->drawText(0, LINES - 2, "> " + currentInput);
                    m_graphics->refresh();
                }
                continue;
            }
        }
    }

    void TerminalShell::processCommandQueue()
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);

        while (! m_commandQueue.empty())
        {
            std::string cmd =
                m_commandQueue.front();

            m_commandQueue.pop();

            Kernel::executeCmd(cmd);
        }
    }
} // namespace kernel