#pragma once
#include <string>
#include <vector>
class Menu
{

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

    static void destroyInstance();

    int getSelected() const;

private:
    std::string m_question;
    std::vector<std::string> m_options;
    int m_selected = 0;

    static inline std::unique_ptr<Menu> m_currentInstance;
    void m_updateScreen();
};