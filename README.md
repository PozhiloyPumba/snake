# **Snake**

![Screenshot](pics/snake.jpg)

---
## How to build my project (it is available only for Linux):
```
$ cmake -DCMAKE_BUILD_TYPE=RELEASE -B build && cd build
$ cmake --build .
```
## SFML is required for the project to work. You can find more information on how to install it on their [official website](https://www.sfml-dev.org/).
```
## Compiler settings:
```
$ -std=c++17 -Wall -Wextra -O2
```
## To run the graphic version of snake, run
```
$ make graphSnake 
```
## If you want to play on terminal version of my snake, you should run:
```
$ make textSnake 
```
## Implemented by second-year MIPT student Ivanov Ivan