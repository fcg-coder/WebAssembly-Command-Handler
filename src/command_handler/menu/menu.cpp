#include "menu.hpp"
#include <sstream>
#include <algorithm>

extern InputOutputHandler* IOH;

std::unique_ptr<Menu> Menu::m_currentInstance = nullptr;

void Menu::m_updateScreen()
{

    IOH->output(render());
}

Menu::Menu(const std::string& question, const std::vector<std::string>& options)
    : m_question(question), m_options(options), m_selected(0)
{
    if (m_currentInstance)
    {
        m_currentInstance.reset();
    }
    m_currentInstance.reset(this);
    m_updateScreen();
}

void Menu::moveUp()
{
    if (m_options.empty())
    {
        return;
    }
    m_selected = std::clamp(m_selected - 1, 0, static_cast<int>(m_options.size()) - 1);
    m_updateScreen();
}

void Menu::moveDown()
{

    if (m_options.empty())
    {

        return;
    }
    m_selected = std::clamp(m_selected + 1, 0, static_cast<int>(m_options.size()) - 1);
    m_updateScreen();
}

int Menu::getSelected() const
{
    return m_selected;
}

std::string Menu::render() const
{
    std::stringstream ss;

    // base container (background)
    ss << "<div style='"
       << "position: fixed; "
       << "top: 0; left: 0; right: 0; bottom: 0; "
       << "background-color: #68acf6; "
       << "display: flex; "
       << "justify-content: center; "
       << "align-items: center; "
       << "'>";

    // menu container
    ss << "<div style='"
       << "background-color: #f0f0f0; "
       << "border-radius: 8px; "
       << "padding: 30px; "
       << "box-shadow: 0 10px 8px rgba(0,0,0,0.1); "
       << "max-width: 80%; "
       << "color: black; "
       << "text-align: center; "
       << "'>";

    ss << "<div style='font-family: \"Courier New\", monospace; font-size: 18px;'>";
    ss << "<div style='margin-bottom: 15px; font-weight: bold;'>"
       << m_question
       << "</div>";

    for (size_t i = 0; i < m_options.size(); ++i)
    {
        if (i == static_cast<size_t>(m_selected))
        {
            ss << "<div style='color: #000000; font-weight: bold; margin: 5px 0;'>"
               << "&lt; " << m_options[i] << " &gt;"
               << "</div>";
        }
        else
        {
            ss << "<div style='color: #666666; margin: 5px 0;'>"
               << m_options[i]
               << "</div>";
        }
    }

    ss << "</div></div></div> ";
    return ss.str();
}