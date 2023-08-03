#include <iostream>
#include "dependencies/FileHandler.hpp"
#include "dependencies/TimeHandler.hpp"
#include <istream>
#include <csignal>

#ifdef _WIN32
void clearConsole() {
    std::system("cls");
}
#else
void clearConsole() {
    std::system("clear");
}
#endif

void printCommands(const std::string& commands)
{
    std::cout << commands << std::endl;
}

fs::path addNewTodoList(const fs::path& dirPath, const std::string& listName)
{
    fs::path path = dirPath;
    if(!dirPath.string().ends_with('/') && dirPath.string().ends_with('\\'))
        path += '/';
    path += listName;
    path += ".txt";

    if(fs::exists(path))
    {
       std::cerr << "Failed to create new todo list with name[" << listName << "]. this list already exist" <<std::endl;
       return {};
    }

    if(!FileHandler::CreateFile(path))
        return {};
    return path;
}

std::string getUserInput()
{
    std::string inputBuffer;
    std::getline(std::cin, inputBuffer);
    return inputBuffer;
}

std::string getNext(std::string& input)
{
    std::string command;
    std::size_t commandLen = input.find_first_of(' ');
    if(commandLen == std::basic_string<char>::npos)
    {
        command.append(input);
        input.clear();
    }
    else
    {
        command.assign(input.begin(), input.begin() + static_cast<long long>(commandLen));
        input.erase(input.begin(), input.begin() + static_cast<long long>(commandLen + 1));
    }
    return command;
}

void printEntries(const std::vector<std::string>& entries)
{
    for(auto& entry : entries)
        if(entry.ends_with('\n'))
            std::cout << entry;
        else
            std::cout << entry << std::endl;
}

struct OnSignalSaveData
{
    fs::path& mPath;
    std::vector<std::filesystem::path>& mPaths;
};

std::unique_ptr<OnSignalSaveData> save_data_ptr;

void signalHandler(int signum)
{
    std::string outString;
    for(auto& path : save_data_ptr->mPaths)
        outString += path.string() + "\n";

    FileHandler::WriteToFile(save_data_ptr->mPath, outString);

    exit(signum);
}

int main() {
    std::signal(SIGBREAK , signalHandler);
    std::signal(SIGINT , signalHandler);

    fs::path dirPath("todo_lists/");
    fs::path listDirPath("data/paths.txt");
    fs::path currentTodoList;

    std::vector<fs::path> todoListPaths;
    std::vector<std::string> listDirPathBuffer;
    save_data_ptr = std::make_unique<OnSignalSaveData>(OnSignalSaveData{listDirPath, todoListPaths});

    FileHandler::CreateFile(listDirPath);
    FileHandler::GetLinesFromFile(listDirPath, listDirPathBuffer);

    todoListPaths.reserve(listDirPathBuffer.size());
    for(auto& path : listDirPathBuffer)
        todoListPaths.emplace_back(path);
    listDirPathBuffer.clear();

    std::string menuCommands("Commands: exit"
                             " list"
                             " add [name]"
                             " open [name]");

    std::string listCommands("Commands: add [description]"
                             " done [index]"
                             " close"
                             " exit");

    printCommands(menuCommands);

    while(true)
    {
        auto inputBuffer = getUserInput();
        std::string command = getNext(inputBuffer);

        if(command == "exit")
            break;
        else if(command == "list")
        {
            clearConsole();
            printCommands(menuCommands);

            std::size_t count = 1;
            for(auto& path : todoListPaths)
            {
                std::string name = path.filename().string();
                name.erase(name.end() - 4, name.end());
                std::cout << count++ << " : " << name << std::endl;
            }
        }
        else if(command == "add")
        {
            clearConsole();
            printCommands(menuCommands);

            command = getNext(inputBuffer);
            if(command.empty())
            {
                std::cerr << "No name for the todo list was given!\nUse of add: add [name]" << std::endl;
                continue;
            }

            fs::path newPath = addNewTodoList(dirPath, command);
            if(newPath.empty())
                continue;
            todoListPaths.emplace_back(newPath);
        }
        else if(command == "open")
        {
            clearConsole();
            std::string name = getNext(inputBuffer);
            if(name.empty())
            {
                std::cerr << "Failed to open todo list with name[" << name << "]" << std::endl;
                continue;
            }

            currentTodoList = dirPath;
            currentTodoList += name;
            currentTodoList += ".txt";

            std::vector<std::string> entries;
            if(!FileHandler::GetLinesFromFile(currentTodoList, entries))
            {
                clearConsole();
                printCommands(menuCommands);
                std::cerr << "Failed to open list with name[" << name << "]. This list doesn't exist" << std::endl;
                continue;
            }

            std::cout << "Todo list: " << name << std::endl;
            printCommands(listCommands);
            printEntries(entries);

            std::size_t count = entries.size() + 1;
            bool exit = false;

            while(true)
            {
                inputBuffer = getUserInput();
                command = getNext(inputBuffer);

                if(command == "close")
                    break;
                else if(command == "add")
                {
                    if(inputBuffer.empty())
                    {
                        std::cerr << "Failed to add an entry!\nUse of add: add [description]" << std::endl;
                        continue;
                    }

                    entries.emplace_back(std::format("{}\t[ ] - {}\n",count++, inputBuffer));
                    FileHandler::WriteToFile(currentTodoList, entries.at(entries.size() -1), std::ios::app);

                    clearConsole();
                    std::cout << "Todo list: " << name << std::endl;
                    printCommands(listCommands);
                    printEntries(entries);
                }
                else if(command == "done")
                {
                    if(inputBuffer.empty())
                    {
                        std::cerr << "Failed to add an entry!\nUse of add: add [description]" << std::endl;
                        continue;
                    }

                    int index;
                    std::istringstream iss(inputBuffer);
                    iss >> index;
                    index -= 1;

                    if(index >= 0 && index <= entries.size())
                    {
                        entries.at(index).replace(entries.at(index).find_first_of('['), 3, "[X]");
                    }
                    else
                        std::cerr << "failed do mark entry as done! no entry with index[" << index << "]" << std::endl;

                    std::string out;
                    for(auto& e : entries)
                    {
                        if(e.ends_with('\n'))
                            out += e;
                        else
                            out += e + '\n';
                    }

                    FileHandler::WriteToFile(currentTodoList, out);

                    clearConsole();
                    std::cout << "Todo list: " << name << std::endl;
                    printCommands(listCommands);
                    printEntries(entries);
                }
                else if(command == "exit")
                {
                    exit = true;
                    break;
                }
                else
                {
                    std::cout << "Unknown command["<< command << "]\n"<< listCommands << std::endl;
                }
            }
            if(exit)
                break;
            clearConsole();
            printCommands(menuCommands);
        }
        else
        {
            clearConsole();
            std::cout << "Unknown command["<< command << "]\n"<< menuCommands << std::endl;
        }
    }

    std::string outString;
    for(auto& path : todoListPaths)
        outString += path.string() + "\n";

    FileHandler::WriteToFile(listDirPath, outString);

    return 0;
}
