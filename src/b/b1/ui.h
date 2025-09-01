#pragma once
#include <stdio.h>
#include <iostream>

// 颜色代码枚举
enum Color { ///列出 红橙 黄绿蓝靛紫 黑白
    BLACK = 30, /// 黑色
    RED = 31, /// 红色
    GREEN = 32, /// 绿色
    YELLOW = 33, /// 黄色
    BLUE = 34, /// 蓝色
    MAGENTA = 35, /// 品红色
    CYAN = 36, /// 青色
    WHITE = 37, /// 白色
    RESET = 0 /// 重置颜色
};

int gRows = 24,
    gCols = 80;


class UIBase
{
private:
    /* data */
public:
    UIBase(/* args */){}
    ~UIBase(){}
    //绘制窗体

    static void printFrame(){

        for (int i = 1; i <= gCols; ++i) {
            printAt(1, i, "-", CYAN);
            printAt(gRows, i, "-", CYAN);
        }
        for (int i = 1; i <= gRows; ++i) {
            printAt(i, 1, "|", CYAN);
            printAt(i, gCols, "|", CYAN);
        }
    }

    static void printLine(int row){
        for (int i = 1; i <= gCols; ++i) {
            printAt(row, i, "-", CYAN);
        }
    }
     static void showCursor() {
        std::cout << "\033[?25h";
    }
        // 设置光标坐标（行, 列）
    static void setCursorPosition(int row, int col) {
        // ANSI转义序列：\033[行;列H
        std::cout << "\033[" << row << ";" << col << "H";
    }

    static void hideCursor() {
        std::cout << "\033[?25l";
    }

    static void clearScreen() {
    // 清屏命令 + 移动光标到左上角
      std::cout << "\033[H\033[J";
    }

    static void resetColor() {
        setColor(RESET);
    }
    static void setColor(Color color) {
        std::cout << "\033[" << color << "m";
    }

// 在指定位置打印文本
static void printAt(int row, int col, const std::string &text, Color color = WHITE) {
    // 移动光标到指定位置
    std::cout << "\033[" << row << ";" << col << "H";
    // 设置颜色并打印文本
    setColor(color);
    std::cout << text;
    resetColor();
}

};