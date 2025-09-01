#include <iostream>
#include <string>
#include <unistd.h> // 用于sleep函数

// 颜色代码枚举
enum Color {
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    RESET = 0
};

// 设置文本颜色
void setColor(Color color) {
    std::cout << "\033[" << color << "m";
}

// 重置文本颜色
void resetColor() {
    setColor(RESET);
}

// 清屏（全屏刷新）
void clearScreen() {
    // 清屏命令 + 移动光标到左上角
     std::cout << "\033[H\033[J";
}

// 获取终端尺寸（简化版）
void getTerminalSize(int &rows, int &cols) {
    // 这里使用固定值作为示例，实际应用中可以使用ioctl获取真实尺寸
    rows = 24;
    cols = 80;
}

// 在指定位置打印文本
void printAt(int row, int col, const std::string &text, Color color = WHITE) {
    // 移动光标到指定位置
    std::cout << "\033[" << row << ";" << col << "H";
    // 设置颜色并打印文本
    setColor(color);
    std::cout << text;
    resetColor();
}

int main() {
    int rows, cols;
    getTerminalSize(rows, cols);
    int frame = 0;
    
    // 简单的动画演示
    while (true) {
        // 全屏刷新
        clearScreen();
        
        // 绘制边框
        setColor(CYAN);
        for (int i = 1; i <= cols; ++i) {
            printAt(1, i, "-", CYAN);
            printAt(rows, i, "-", CYAN);
        }
        for (int i = 1; i <= rows; ++i) {
            printAt(i, 1, "|", CYAN);
            printAt(i, cols, "|", CYAN);
        }
        resetColor();
        
        // 显示彩色文本
        printAt(3, 5, "Mac 控制台彩色输出演示", MAGENTA);
        printAt(5, 5, "全屏刷新: 每次循环都会清空屏幕", GREEN);
        printAt(6, 5, "字体着色: 这段文字是绿色的", GREEN);
        printAt(7, 5, "这段文字是红色的", RED);
        printAt(8, 5, "这段文字是蓝色的", BLUE);
        printAt(9, 5, "这段文字是黄色的", YELLOW);
        
        // 移动的动画元素
        int pos = (frame % (cols - 10)) + 2;
        printAt(12, pos, "移动中...", MAGENTA);
        
        // 显示帧数
        printAt(rows - 2, 5, "帧计数: " + std::to_string(frame), YELLOW);
        
        // 刷新输出
        std::cout.flush();
        
        // 等待一段时间
        usleep(100000); // 100毫秒
        
        frame++;
        
        // 按Ctrl+C退出程序
    }
    
    return 0;
}
