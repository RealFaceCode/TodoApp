# Todo List Management System

This is a simple command-line Todo List Management System written in C++. It allows you to create, view, and manage todo lists. You can add new tasks, mark tasks as done, and close todo lists. The lists are stored as text files in the `todo_lists/` directory.

## Prerequisites

Make sure you have the following installed on your system:

- C++ Compiler (e.g., g++ for Linux or MinGW for Windows)
- Filesystem Library (Included in C++17)
- C++ Standard Template Library (STL)

## Dependencies

- [https://github.com/RealFaceCode/TimeHandler](https://github.com/RealFaceCode/TimeHandler)
- [https://github.com/RealFaceCode/FileHandler](https://github.com/RealFaceCode/FileHandler)

## Getting Started

1. Clone the repository to your local machine:

```bash
git clone https://github.com/RealFaceCode/TodoApp
```

2. Change directory to the project folder:

```bash
cd TodoApp
```

3. Compile the source code:

For Linux:

```bash
g++ -std=c++23 -o todo_manager main.cpp
```

For Windows (using MinGW):

```bash
g++ -std=c++23 -o todo_manager.exe main.cpp
```

## Usage

To run the Todo List Management System, execute the compiled binary:

For Linux:

```bash
./todo_manager
```

For Windows:

```bash
todo_manager.exe
```

## Commands

- `exit`: Exit the program.
- `list`: List all existing todo lists.
- `add [name]`: Create a new todo list with the specified name.
- `open [name]`: Open an existing todo list to view and manage its tasks.

## Usage Example

1. Create a new todo list:

```bash
> add groceries
Todo list 'groceries' created successfully!
```

2. View all todo lists:

```bash
> list
1 : groceries
```

3. Open an existing todo list:

```bash
> open groceries
Todo list: groceries
Commands: add [description] done [index] close exit
1. [ ] - Buy milk
2. [ ] - Buy eggs
```

4. Add tasks to the todo list:

```bash
> add Buy bread
Todo added successfully!
```

5. Mark a task as done:

```bash
> done 2
Task 2 marked as done!
```

6. Close the current todo list:

```bash
> close
```

7. Exit the program:

```bash
> exit
```

## Saving Data

The program automatically saves the list of todo lists in the file `data/paths.txt` whenever you exit the program or close a todo list. This file keeps track of all created todo lists, allowing you to access them the next time you run the program.

## Signal Handling

The program supports signal handling for graceful termination. If you press `CTRL+C` or send the `SIGINT` signal, the program will save the list of todo lists and exit gracefully.

## Contributions

Contributions to this project are welcome! If you find any bugs or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
