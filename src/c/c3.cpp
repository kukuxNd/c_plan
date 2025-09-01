#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <random>
#include <algorithm>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <map>

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
    RESET = 0,
    LIGHT_YELLOW = 93 // 淡黄色
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
    std::cout << "\033[2J\033[1;1H";
}

// 获取终端尺寸（简化版）
void getTerminalSize(int &rows, int &cols) {
    rows = 24;
    cols = 80;
}

// 在指定位置打印文本
void printAt(int row, int col, const std::string &text, Color color = WHITE) {
    std::cout << "\033[" << row << ";" << col << "H";
    setColor(color);
    std::cout << text;
    resetColor();
}

// 资源/货币类型
struct ResourceInfo {
    std::string name;
    int value;
    Color color;
    bool highlight = false; // 本轮是否被扣
};

class Game {
public:
    Game() {
        srand(time(nullptr));
        rows = 24;
        cols = 80;
        countdown = 30;
        frame = 0;
        timeCounter = 0;

        // 初始化货币
        resources.push_back({"A", rand() % 1000 + 100, GREEN});
        resources.push_back({"B", rand() % 500 + 50, BLUE});
        resources.push_back({"C", rand() % 200 + 20, LIGHT_YELLOW});

        // 初始化资源 D~Z
        for (char ch = 'D'; ch <= 'Z'; ++ch) {
            resources.push_back({std::string(1, ch), rand() % 100 + 1, WHITE});
        }

        // 兑换比例，A=1, B=2, C=3, D=4, ..., K=11
        for (int i = 0; i < 11; ++i) {
            exchangeRate.push_back(i + 1);
        }
    }

    void run() {
        while (getResource("A") > 0 && countdown > 0) {
            clearScreen();
            drawUI();
            std::cout.flush();
            usleep(100000); // 100ms

            frame++;
            timeCounter++;

            // 每秒扣一次
            if (timeCounter % 10 == 0) {
                countdown--;
                deductRound();
            }
        }
        clearScreen();
        printAt(rows / 2, (cols - 10) / 2, "游戏结束！A资源耗尽", RED);
        std::cout.flush();
        sleep(2);
    }

private:
    int rows, cols;
    int countdown;
    int frame;
    int timeCounter;
    std::vector<ResourceInfo> resources; // 货币+资源
    std::vector<int> exchangeRate; // 兑换比例
    std::deque<std::string> deductHistory; // 扣除历史

    // 获取资源/货币当前值
    int getResource(const std::string& name) {
        for (const auto& res : resources) {
            if (res.name == name) return res.value;
        }
        return 0;
    }

    // 设置资源/货币当前值
    void setResource(const std::string& name, int val) {
        for (auto& res : resources) {
            if (res.name == name) {
                res.value = val;
                return;
            }
        }
    }

    // 获取资源/货币在vector中的下标
    int getIndex(const std::string& name) {
        for (size_t i = 0; i < resources.size(); ++i) {
            if (resources[i].name == name) return i;
        }
        return -1;
    }

    // 本轮扣除
    void deductRound() {
        // 取消上轮高亮
        for (auto& res : resources) res.highlight = false;

        // 随机选择3~5种不同资源/货币
        int kinds = rand() % 3 + 3; // 3~5
        std::vector<int> types(resources.size());
        for (size_t i = 0; i < resources.size(); ++i) types[i] = i;
        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(types.begin(), types.end(), g);

        std::string roundMsg;
        bool gameOver = false;

        for (int i = 0; i < kinds; ++i) {
            int idx = types[i];
            std::string name = resources[idx].name;
            int deductAmount = rand() % 10 + 1; // 1~10
            bool paid = false;

            // 货币A/B/C
            if (name == "A") {
                if (resources[idx].value >= deductAmount) {
                    resources[idx].value -= deductAmount;
                    roundMsg += "A -" + std::to_string(deductAmount) + " ";
                    resources[idx].highlight = true;
                    paid = true;
                }
            } else if (name == "B") {
                if (resources[idx].value >= deductAmount) {
                    resources[idx].value -= deductAmount;
                    roundMsg += "B -" + std::to_string(deductAmount) + " ";
                    resources[idx].highlight = true;
                    paid = true;
                }
            } else if (name == "C") {
                if (resources[idx].value >= deductAmount) {
                    resources[idx].value -= deductAmount;
                    roundMsg += "C -" + std::to_string(deductAmount) + " ";
                    resources[idx].highlight = true;
                    paid = true;
                }
            } else { // D~K
                int resIdx = name[0] - 'D';
                if (resIdx < 11 && resources[idx].value >= deductAmount) {
                    resources[idx].value -= deductAmount;
                    roundMsg += name + " -" + std::to_string(deductAmount) + " ";
                    resources[idx].highlight = true;
                    paid = true;
                }
            }

            // 不够则用A兑换
            if (!paid) {
                int needA = 0;
                if (name == "A") {
                    needA = deductAmount;
                } else if (name == "B") {
                    needA = (deductAmount + 1) / 2;
                } else if (name == "C") {
                    needA = (deductAmount + 2) / 3;
                } else {
                    int resIdx = name[0] - 'D';
                    if (resIdx < 11) {
                        int rate = exchangeRate[resIdx + 3];
                        needA = (deductAmount + rate - 1) / rate;
                    } else {
                        needA = deductAmount; // 其他资源不能兑换，直接用A
                    }
                }
                int aIdx = getIndex("A");
                if (resources[aIdx].value >= needA) {
                    resources[aIdx].value -= needA;
                    roundMsg += "A -" + std::to_string(needA) + "(兑) ";
                    resources[aIdx].highlight = true;
                } else {
                    roundMsg += "A不足 ";
                    resources[aIdx].value = 0;
                    gameOver = true;
                    break;
                }
            }
        }

        // 记录本轮扣除
        deductHistory.push_front(roundMsg);
        if (deductHistory.size() > 5) deductHistory.pop_back();
    }

    // 绘制UI
    void drawUI() {
        // 边框
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
        int col = 15;
        for (int i = 0; i < 3; ++i) {
            Color showColor = resources[i].highlight ? RED : resources[i].color;
            printAt(2, col, resources[i].name + ": " + std::to_string(resources[i].value), showColor);
            col += 15;
        }

        // 显示资源 D-Z
        printAt(3, 5, "我的资源：", YELLOW);
        col = 15;
        for (size_t i = 3; i < resources.size(); ++i) {
            Color showColor = resources[i].highlight ? RED : resources[i].color;
            printAt(3, col, resources[i].name + ": " + std::to_string(resources[i].value), showColor);
            col += 8;
            if (col > cols - 8) break;
        }

        // 倒计时
        printAt(4, 5, "倒计时: " + std::to_string(countdown) + " 秒", YELLOW);

        // 扣除历史
        printAt(5, 5, "最近5次扣除：", RED);
        int historyRow = 6;
        for (const auto& record : deductHistory) {
            printAt(historyRow++, 7, record, RED);
        }

        int pos = (frame % (cols - 10)) + 2;
        printAt(13, pos, "移动中...", MAGENTA);

        printAt(rows - 2, 5, "帧计数: " + std::to_string(frame), YELLOW);
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}