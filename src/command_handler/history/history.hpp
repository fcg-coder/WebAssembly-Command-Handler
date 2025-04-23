#pragma once
#include <iostream>
#include <string>
#include <cstdarg>
#include "../commands/commands.hpp"

namespace FormatUtils
{
    static std::string formatString(const char* format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        return std::string(buffer);
    }
} // namespace FormatUtils

class Queue
{
private:
    struct Node
    {
        CommandBase* command;
        std::string commandName;
        Node* next;
        Node* prev;
        int id;

        Node(CommandBase* _command, const std::string& _name, int _id)
            : command(_command), commandName(_name), next(nullptr), prev(nullptr), id(_id) {}
    };

    Node* head;
    Node* tail;
    size_t size;
    static Queue* m_instance;

public:
    static Queue* getInstance()
    {
        if (! m_instance)
        {
            m_instance = new Queue();
        }
        return m_instance;
    }

    void push(CommandBase* _command, std::string _name)
    {
        int newId = isEmpty() ? 0 : tail->id + 1;
        Node* newNode = new Node(_command, _name, newId);

        if (isEmpty())
        {
            head = tail = newNode;
        }
        else
        {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    CommandBase* pop()
    {
        if (isEmpty())
            return nullptr;

        Node* temp = head;
        CommandBase* cmd = head->command;

        head = head->next;
        if (head)
        {
            head->prev = nullptr;
        }
        else
        {
            tail = nullptr;
        }

        delete temp;
        size--;
        return cmd;
    }

    template <typename F>
    void forEach(F&& callback)
    {
        Node* current = head;
        while (current)
        {
            callback(current->id, current->command, current->commandName);
            current = current->next;
        }
    }

    bool isEmpty() const { return size == 0; }
    size_t getSize() const { return size; }

    void clear()
    {
        while (! isEmpty())
        {
            CommandBase* cmd = pop();
        }
    }

    ~Queue()
    {
        clear();
    }
};

#define HISTSIZE   100
#define HISTFORMAT "%d %s"

class History : public CommandBase
{
private:
    Queue* commandQueue;

public:
    History();

    void addCommand(CommandBase* command, std::string name);
    void printHistory();
    void execute() override;
    void man() override;
    void description() override;
};