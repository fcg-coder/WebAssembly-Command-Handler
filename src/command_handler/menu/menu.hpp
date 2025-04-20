#pragma once
#include <string>
#include <vector>
#include "../../main.hpp"

class Menu
{
private:
    std::string m_question;
    std::vector<std::string> m_options;
    int m_selected = 0;

    static std::unique_ptr<Menu> m_currentInstance;
    void m_updateScreen();

public:
    Menu(const std::string& question, const std::vector<std::string>& options);
    void moveUp();
    void moveDown();
    void pressEnter();

    std::string render() const;

    static Menu* getCurrentInstance()
    {
        return m_currentInstance.get();
    }

    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    static void destroyInstance();

    int getSelected() const;
};