#include <iostream>
#include <string>

// 先检查是否为Windows环境
#ifdef _WIN32
#pragma message("_WIN32 已定义 - 使用Windows环境")
#include <windows.h>  // Windows API头文件

// 1. 替换usleep（Windows用Sleep，单位毫秒）
#define usleep(microseconds) Sleep((microseconds + 999) / 1000)  // 微秒转毫秒（四舍五入）

// 2. Windows控制台颜色常量（替换ANSI转义码）
enum WinColor {
    WIN_BLACK = 0,
    WIN_BLUE = 1,
    WIN_GREEN = 2,
    WIN_CYAN = 3,
    WIN_RED = 4,
    WIN_MAGENTA = 5,
    WIN_YELLOW = 6,
    WIN_WHITE = 7
};

// 3. Windows设置控制台颜色（替换ANSI转义）
void setWinColor(WinColor fgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, fgColor);  // 设置前景色
}

// 4. Windows重置控制台颜色（恢复默认）
void resetWinColor() {
    setWinColor(WIN_WHITE);  // 恢复白色默认色
}

// 5. Windows清屏（替换ANSI转义）
void clearWinScreen() {
    system("cls");  // Windows清屏命令
}

// 6. Windows移动光标到指定位置（替换ANSI转义）
void moveWinCursor(int row, int col) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(col - 1), static_cast<SHORT>(row - 1) };  // Windows坐标从(0,0)开始
    SetConsoleCursorPosition(hConsole, pos);
}

// 非Windows环境（Linux/macOS）保留原ANSI转义实现
#elif defined(__linux__) || defined(__APPLE__)
#pragma message("使用类Unix环境(Linux/macOS)")
#include <unistd.h>

// 颜色代码枚举（ANSI转义码）
enum Color {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    RESET = 0
};

// 设置文本颜色（ANSI）
void setColor(Color color) {
    std::cout << "\033[" << color << "m";
}

// 重置文本颜色（ANSI）
void resetColor() {
    setColor(RESET);
}

// 清屏（ANSI）
void clearScreen() {
    std::cout << "\033[H\033[J";
}

// 移动光标并打印文本（ANSI）
void printAt(int row, int col, const std::string& text, Color color = WHITE) {
    std::cout << "\033[" << row << ";" << col << "H";
    setColor(color);
    std::cout << text;
    resetColor();
}

#else
#pragma message("未知操作系统，不支持当前代码")
#error "当前操作系统不支持本程序的控制台控制逻辑"
#endif

// 获取终端尺寸（简化版，跨平台通用）
void getTerminalSize(int& rows, int& cols) {
    // 实际项目可通过Windows API（GetConsoleScreenBufferInfo）或类Unix（ioctl）获取真实尺寸
    rows = 24;  // 假设终端24行
    cols = 80;  // 假设终端80列
}

int main() {
    int rows, cols;
    getTerminalSize(rows, cols);
    int frame = 0;

    // 简单的动画演示
    while (true) {
        // 1. 清屏（Windows/Linux分别处理）
#ifdef _WIN32
        clearWinScreen();
#else
        clearScreen();
#endif

        // 2. 绘制边框（跨平台逻辑统一，调用平台特定接口）
#ifdef _WIN32
        setWinColor(WIN_CYAN);  // Windows青色
        // 上边框
        moveWinCursor(1, 1);
        for (int i = 1; i <= cols; ++i) std::cout << "-";
        // 下边框
        moveWinCursor(rows, 1);
        for (int i = 1; i <= cols; ++i) std::cout << "-";
        // 左右边框
        for (int i = 2; i < rows; ++i) {
            moveWinCursor(i, 1);   std::cout << "|";
            moveWinCursor(i, cols); std::cout << "|";
        }
        resetWinColor();  // 重置Windows颜色
#else
        setColor(CYAN);  // Linux青色
        // 上边框
        printAt(1, 1, std::string(cols, '-'), CYAN);
        // 下边框
        printAt(rows, 1, std::string(cols, '-'), CYAN);
        // 左右边框
        for (int i = 2; i < rows; ++i) {
            printAt(i, 1, "|", CYAN);
            printAt(i, cols, "|", CYAN);
        }
        resetColor();  // 重置Linux颜色
#endif

        // 3. 显示彩色文本（平台特定接口）
#ifdef _WIN32
        moveWinCursor(3, 5);
        setWinColor(WIN_MAGENTA); std::cout << "Windows 控制台彩色输出演示"; resetWinColor();

        moveWinCursor(5, 5);
        setWinColor(WIN_GREEN); std::cout << "全屏刷新: 每次循环都会清空屏幕"; resetWinColor();

        moveWinCursor(6, 5);
        setWinColor(WIN_GREEN); std::cout << "字体着色: 这段文字是绿色的"; resetWinColor();

        moveWinCursor(7, 5);
        setWinColor(WIN_RED); std::cout << "这段文字是红色的"; resetWinColor();

        moveWinCursor(8, 5);
        setWinColor(WIN_BLUE); std::cout << "这段文字是蓝色的"; resetWinColor();

        moveWinCursor(9, 5);
        setWinColor(WIN_YELLOW); std::cout << "这段文字是黄色的"; resetWinColor();
#else
        printAt(3, 5, "Linux/macOS 控制台彩色输出演示", MAGENTA);
        printAt(5, 5, "全屏刷新: 每次循环都会清空屏幕", GREEN);
        printAt(6, 5, "字体着色: 这段文字是绿色的", GREEN);
        printAt(7, 5, "这段文字是红色的", RED);
        printAt(8, 5, "这段文字是蓝色的", BLUE);
        printAt(9, 5, "这段文字是黄色的", YELLOW);
#endif

        // 4. 移动的动画元素
#ifdef _WIN32
        int pos = (frame % (cols - 10)) + 2;
        moveWinCursor(12, pos);
        setWinColor(WIN_MAGENTA); std::cout << "移动中..."; resetWinColor();
#else
        int pos = (frame % (cols - 10)) + 2;
        printAt(12, pos, "移动中...", MAGENTA);
#endif

        // 5. 显示帧数
#ifdef _WIN32
        moveWinCursor(rows - 2, 5);
        setWinColor(WIN_YELLOW); std::cout << "帧计数: " << frame; resetWinColor();
#else
        printAt(rows - 2, 5, "帧计数: " + std::to_string(frame), YELLOW);
#endif

        // 6. 刷新输出并延时
        std::cout.flush();
        usleep(1000000/60);  // 100毫秒（跨平台兼容，已通过宏适配）

        frame++;
        // 按Ctrl+C退出程序
    }

    return 0;
}