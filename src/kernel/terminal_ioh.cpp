#include "terminal_ioh.hpp"

#include <algorithm>
#include <chrono>

namespace kernel
{
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
        m_graphics = std::make_unique<TerminalGraphics>();
        m_inputThread = std::thread(&TerminalShell::inputLoop, this);
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

        if (Kernel::getMode() == InputOutputMode::SCREEN)
            return;

        std::lock_guard<std::mutex> lock(m_outputMutex);
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

        const int maxOutputLines = std::max(1, LINES - 2);

        while (static_cast<int>(m_outputLines.size()) > maxOutputLines)
        {
            m_outputLines.erase(m_outputLines.begin());
        }

        ::clear();

        for (int i = 0; i < static_cast<int>(m_outputLines.size()); ++i)
        {
            mvprintw(i, 0, "%s", m_outputLines[i].c_str());
        }

        mvprintw(LINES - 1, 0, "> %s", m_currentInput.c_str());

        ::refresh();
    }

    TerminalGraphics& TerminalShell::getGraphics()
    {
        return *m_graphics;
    }

    void TerminalShell::clear()
    {
        ::clear();
        ::refresh();
    }

    void TerminalShell::inputLoop()
    {
        std::string currentInput;

        mvprintw(LINES - 1, 0, "> ");

        ::refresh();

        while (m_running)
        {

            if (Kernel::getMode() == InputOutputMode::SCREEN)
            {
                auto* screen = Kernel::SCREEN();

                if (screen)
                {
                    const int width = m_graphics->getWindowSize().first;
                    const int height = m_graphics->getWindowSize().second;
                    screen->setSize(height, width);

                    uint32_t* buffer = screen->getScreen();

                    if (buffer && width > 0 && height > 0)
                    {
                        m_graphics->clear();

                        m_graphics->drawScreen(buffer, width, height);

                        m_graphics->refresh();
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                continue;
            }

            const int ch = getch();

            if (ch == ERR)
            {
                processCommandQueue();

                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                continue;
            }

            /*
             * ENTER
             */
            if (ch == '\n' || ch == '\r')
            {
                if (! currentInput.empty())
                {
                    input(currentInput);

                    currentInput.clear();

                    m_graphics->clear();

                    for (int i = 0; i < static_cast<int>(m_outputLines.size()); ++i)
                    {
                        mvprintw(i, 0, "%s", m_outputLines[i].c_str());
                    }

                    mvprintw(LINES - 1, 0, "> ");

                    m_graphics->refresh();
                }

                continue;
            }

            /*
             * BACKSPACE
             */
            if (ch == 127 || ch == KEY_BACKSPACE)
            {
                if (! currentInput.empty())
                {
                    currentInput.pop_back();

                    m_graphics->clear();

                    for (int i = 0; i < static_cast<int>(m_outputLines.size()); ++i)
                    {
                        mvprintw(i, 0, "%s", m_outputLines[i].c_str());
                    }

                    mvprintw(LINES - 1, 0, "> %s", currentInput.c_str());

                    m_graphics->refresh();
                }

                continue;
            }

            /*
             * Обычный символ.
             */
            if (ch >= 32 && ch <= 126)
            {
                currentInput += static_cast<char>(ch);

                m_graphics->clear();

                for (int i = 0; i < static_cast<int>(m_outputLines.size()); ++i)
                {
                    mvprintw(i, 0, "%s", m_outputLines[i].c_str());
                }

                mvprintw(LINES - 1, 0, "> %s", currentInput.c_str());

                m_graphics->refresh();

                continue;
            }
        }
    }

    void TerminalShell::processCommandQueue()
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);

        while (! m_commandQueue.empty())
        {
            const std::string command = m_commandQueue.front();
            m_commandQueue.pop();

            Kernel::executeCmd(command);
        }
    }

    void TerminalGraphics::clear()
    {
        ::clear();
    }
    void TerminalGraphics::drawScreen(const uint32_t* screen, int width, int height)
    {
        if (! screen || width <= 0 || height <= 0)
            return;

        int rows;
        int cols;

        getTerminalSize(rows, cols);

        const int renderWidth =
            std::min(width, cols);

        const int renderHeight =
            std::min(height, rows);

        if (! has_colors())
            return;

        static bool initialized = false;

        if (! initialized)
        {
            start_color();

            if (COLORS >= 256)
            {
                const int pairCount = std::min(240, COLOR_PAIRS - 1);

                for (int i = 0; i < pairCount; ++i)
                {
                    const int color = 16 + i;

                    init_pair(i + 1, color, color);
                }
            }

            initialized = true;
        }

        auto rgbTo256 =
            [](uint8_t r, uint8_t g, uint8_t b) -> int {
            const int rr = (static_cast<int>(r) * 5 + 127) / 255;

            const int gg = (static_cast<int>(g) * 5 + 127) / 255;

            const int bb = (static_cast<int>(b) * 5 + 127) / 255;
            return 16 + 36 * rr + 6 * gg + bb;
        };

        for (int y = 0; y < renderHeight; ++y)
        {
            for (int x = 0; x < renderWidth; ++x)
            {
                const uint32_t pixel = screen[y * width + x];

                const uint8_t r = static_cast<uint8_t>((pixel >> 24) & 0xFF);
                const uint8_t g = static_cast<uint8_t>((pixel >> 16) & 0xFF);
                const uint8_t b = static_cast<uint8_t>((pixel >> 8) & 0xFF);
                const uint8_t a = static_cast<uint8_t>(pixel & 0xFF);

                if (r == 0 && g == 0 && b == 0)
                {
                    mvaddch(y, x, ' ');
                    continue;
                }

                int pair;

                if (COLORS >= 256)
                {
                    const int color = rgbTo256(r, g, b);

                    pair = color - 15;

                    const int maxPair = std::min(240, COLOR_PAIRS - 1);

                    pair = std::clamp(pair, 1, maxPair);
                }
                else
                {

                    if (r > 200 && g < 100 && b < 100)
                    {
                        pair = 2;
                    }
                    else if (r < 100 && g > 200 && b < 100)
                    {
                        pair = 3;
                    }
                    else if (r > 180 && g > 180 && b < 100)
                    {
                        pair = 4;
                    }
                    else if (r < 100 && g < 100 && b > 200)
                    {
                        pair = 5;
                    }
                    else if (r > 180 && g < 100 && b > 180)
                    {
                        pair = 6;
                    }
                    else if (r < 100 && g > 180 && b > 180)
                    {
                        pair = 7;
                    }
                    else
                    {
                        pair = 8;
                    }
                }

                mvaddch(y, x, ' ' | COLOR_PAIR(pair));
            }
        }
    }

    void TerminalGraphics::refresh()
    {
        ::refresh();
    }

    std::pair<int, int> TerminalGraphics::getWindowSize()
    {
        int rows;
        int cols;

        getmaxyx(stdscr, rows, cols);

        return {cols, rows};
    }

    void TerminalGraphics::getTerminalSize(
        int& rows,
        int& cols)
    {
        getmaxyx(stdscr, rows, cols);
    }
} // namespace kernel