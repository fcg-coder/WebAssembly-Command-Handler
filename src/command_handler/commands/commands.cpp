#include "commands.hpp"

extern InputOutputHandler* IOH;

void CommandHelp::execute()
{
    IOH->output("Help handler");
}

void CommandDrawSquare::execute()
{
    // // Получаем размеры окна из MainWindow
    // // int width = MainWindow::getWidth();
    // // int height = MainWindow::getHeight();

    // // Определяем размер квадрата (1/4 от минимального из высоты и ширины)
    // int squareSize = std::min(width, height) / 4;

    // // Создаем двумерный массив, заполненный пробелами
    // std::vector<std::vector<char>> canvas(height, std::vector<char>(width, ' '));

    // // Вычисляем координаты верхнего левого угла квадрата
    // int startX = std::max(0, (width - squareSize) / 2);
    // int startY = std::max(0, (height - squareSize) / 2);

    // // Рисуем квадрат
    // for (int i = 0; i < squareSize; ++i)
    // {
    //     for (int j = 0; j < squareSize; ++j)
    //     {
    //         canvas[startY + i][startX + j] = '*';
    //     }
    // }

    // std::stringstream outputStream;
    // for (const auto& row : canvas)
    // {
    //     for (char cell : row)
    //     {
    //         outputStream << cell;
    //     }
    //     outputStream << '\n';
    // }
    // IOH->output(outputStream.str());
}

void CommandScreen::execute()
{
    IOH->mode = InputOutputMode::SCREEN;
}

void CommandShell::execute()
{
    IOH->mode = InputOutputMode::SHELL;
}
