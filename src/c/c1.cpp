#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
    #define usleep(ms) Sleep(ms / 1000)  // 转换为毫秒
#else
    #include <unistd.h>
#endif

#include <ctime> // 添加头文件用于随机数种子
#include <cstdlib> // 添加头文件用于rand
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
    std::cout << "\033[2J\033[1;1H";
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

    // ...existing code...
int main() {
    int rows, cols;
    getTerminalSize(rows, cols);
    int frame = 0;

    srand(time(nullptr));

    // 货币与资源
    int currencyA = rand() % 1000 + 100; // 100~1099
    int currencyB = rand() % 500 + 50;   // 50~549
    int currencyC = rand() % 200 + 20;   // 20~219

    int resource[26]; // D-Z
    for (int i = 0; i < 26; ++i) {
        resource[i] = rand() % 100 + 1; // 1~100
    }

    int countdown = 30; // 倒计时30秒
    int timeCounter = 0;
    std::string lastDeductMsg = "";

    // 兑换比例，A=1, B=2, C=3, D=4, ..., K=11
    int exchangeRate[11] = {1,2,3,4,5,6,7,8,9,10,11}; // A~K

    while (currencyA > 0 && countdown > 0) {
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

        // 显示货币
        printAt(2, 5, "我的货币：", YELLOW);
        printAt(2, 15, "A: " + std::to_string(currencyA), GREEN);
        printAt(2, 30, "B: " + std::to_string(currencyB), BLUE);
        printAt(2, 45, "C: " + std::to_string(currencyC), MAGENTA);

        // 显示资源 D-Z
        printAt(3, 5, "我的资源：", YELLOW);
        int col = 15;
        for (int i = 0; i < 23; ++i) { // D~Z 共23个
            char resName = 'D' + i;
            printAt(3, col, std::string(1, resName) + ": " + std::to_string(resource[i]), WHITE);
            col += 8;
            if (col > cols - 8) break;
        }

        // 显示倒计时
        printAt(4, 5, "倒计时: " + std::to_string(countdown) + " 秒", YELLOW);

        // 显示上次扣除资源信息
        printAt(5, 5, "本轮扣除: " + lastDeductMsg, RED);

        // ...existing code...
        printAt(7, 5, "全屏刷新: 每次循环都会清空屏幕", GREEN);
        printAt(8, 5, "字体着色: 这段文字是绿色的", GREEN);
        printAt(9, 5, "这段文字是红色的", RED);
        printAt(10, 5, "这段文字是蓝色的", BLUE);
        printAt(11, 5, "这段文字是黄色的", YELLOW);

        int pos = (frame % (cols - 10)) + 2;
        printAt(13, pos, "移动中...", MAGENTA);

        printAt(rows - 2, 5, "帧计数: " + std::to_string(frame), YELLOW);

        std::cout.flush();

        usleep(100000); // 100毫秒

        frame++;
        timeCounter++;

        // 每秒扣一次
        if (timeCounter % 10 == 0) {
            countdown--;
            lastDeductMsg = "";

            // 随机选择要扣的资源类型（0:A, 1:B, 2:C, 3:D, ..., 12:K）
            int deductType = rand() % 13;
            int deductAmount = rand() % 10 + 1; // 每次扣1~10

            // 处理扣除
            bool paid = false;
            if (deductType == 0) { // A
                if (currencyA >= deductAmount) {
                    currencyA -= deductAmount;
                    lastDeductMsg = "A -" + std::to_string(deductAmount);
                    paid = true;
                }
            } else if (deductType == 1) { // B
                if (currencyB >= deductAmount) {
                    currencyB -= deductAmount;
                    lastDeductMsg = "B -" + std::to_string(deductAmount);
                    paid = true;
                }
            } else if (deductType == 2) { // C
                if (currencyC >= deductAmount) {
                    currencyC -= deductAmount;
                    lastDeductMsg = "C -" + std::to_string(deductAmount);
                    paid = true;
                }
            } else { // D~K
                int idx = deductType - 3;
                if (resource[idx] >= deductAmount) {
                    resource[idx] -= deductAmount;
                    lastDeductMsg = std::string(1, 'D'+idx) + " -" + std::to_string(deductAmount);
                    paid = true;
                }
            }

            // 如果当前资源不够，尝试用A兑换
            if (!paid) {
                int needA = 0;
                if (deductType == 0) {
                    needA = deductAmount;
                } else if (deductType == 1) {
                    needA = (deductAmount + 1) / 2;
                } else if (deductType == 2) {
                    needA = (deductAmount + 2) / 3;
                } else {
                    int rate = exchangeRate[deductType];
                    needA = (deductAmount + rate - 1) / rate;
                }
                if (currencyA >= needA) {
                    currencyA -= needA;
                    lastDeductMsg = "A -" + std::to_string(needA) + " (兑换支付)";
                    paid = true;
                } else {
                    lastDeductMsg = "A 不足，无法支付";
                    currencyA = 0;
                }
            }
        }
    }

    // 游戏结束
    clearScreen();
    printAt(rows / 2, (cols - 10) / 2, "游戏结束！A资源耗尽", RED);
    std::cout.flush();
    sleep(2);

    return 0;

//
}
