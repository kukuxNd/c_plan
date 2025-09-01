#pragma once


#include <string>
#include <unistd.h> // 用于sleep函数
#include <queue>
#include <termios.h>  // 用于终端控制
#include <sys/select.h>
#include <cctype>

#include "ui.h"
#include "cmd.h"
class game{
    public:
    //单例
    static game* instance;
    static game* getInstance(){
        if(instance == nullptr){
            instance = new game();
        }
        return instance;
    }

    game(){
        instance = this;
    }

private:
    struct currency{
        int a;
        int b;
        int c;
    };

    currency curCoins;
    const int roundTimeMax = 3;
    public:
    void init(){
        printf("start game\n");

        curCoins.a = 100;  
        curCoins.b = 100;
        curCoins.c = 100;
    }

    bool exit = true;
    //保存最近五条日志 先进先出 用队列
    //保存最近五条日志 先进先出 用队列
    std::queue<std::string> logs;
void AddLog(std::string log){
        logs.push(log);
        if(logs.size() > 5){
            logs.pop();
        }
}

void printLogs(){
        std::queue<std::string> tempQueue = logs; // 复制原队列
        int j = 0 ; 
        while (!tempQueue.empty()) {
            UIBase::printAt(5+j, 5, tempQueue.front(), CYAN);
            tempQueue.pop(); // 移除队首元素
            j++;
        }
}


std::string inputBuffer;  // 输入缓冲区
void handleInput() {
        // 设置终端为非阻塞模式
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        // 检查键盘输入
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval tv = {0, 0};

        if (select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv) > 0) {
            char c = getchar();
            if (c == '\n') {  // 回车处理
                if (!inputBuffer.empty()) {
                    AddLog("输入: " + inputBuffer);
                    cmd_add_task(inputBuffer);
                    inputBuffer.clear();
                }
            } else if (c == 127) {  // 退格
                if (!inputBuffer.empty()) {
                    inputBuffer.pop_back();
                }
            }else if (c == 'w'){
                // 上箭头
                cmd_add_task("up");
            }
            else if (c == 's'){
                // 上箭头
                cmd_add_task("down");
            }
            else if (c == 'a'){
                // 上箭头
                cmd_add_task("left");
            }
            else if (c == 'd'){
                // 上箭头
                cmd_add_task("right");
            }
             else if (c == 3){ // Ctrl+C
                exit = false;
            }
            else if (isprint(c)) {  // 可打印字符
                inputBuffer += c;
            }
        }

        // 恢复终端设置
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

void run(){
        printf("game running \n");
        int m_round = 1;
        int lifeTime = 3;
        int framecount = 0;
        long long lastTime = 0;
        long long gameTime = 0;
        while (exit)
        {
            /* code */
            UIBase::clearScreen();
            UIBase::printFrame();
            UIBase::printAt(2, 5,  "AP:"+std::to_string(curCoins.a), BLUE);
            UIBase::printAt(2, 12, "BP:"+std::to_string(curCoins.b), GREEN);
            UIBase::printAt(2, 19, "CP:"+std::to_string(curCoins.c), YELLOW);
            UIBase::printAt(3, 5,  "回合数:"+std::to_string(m_round), MAGENTA);
            UIBase::printAt(3, 15, "生命时间:"+std::to_string(lifeTime), CYAN);
            UIBase::printAt(3, 27, "当前帧:"+std::to_string(framecount), RED);
            printLogs();
            UIBase::printLine(11);

            // 设置光标到输入行末尾
            //const std::string prompt = "输入: ";
            int cursorRow = gRows + 1;
            int cursorCol = 1;
            UIBase::setCursorPosition(cursorRow, cursorCol);
            UIBase::showCursor();
            UIBase::printAt(gRows+1, 1, inputBuffer, WHITE);
            std::cout.flush();
            handleInput();  // 处理键盘输入
            usleep(1000000.0f / 60.0f);   // 16ms
            //计算累计时间 
            //系统时间
            long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            //计算累计时间
            long long deltaTime = currentTime - lastTime;
            lastTime = currentTime; // 更新上一帧时间 
            framecount++;
            gameTime += deltaTime;

            bool cmd_res = cmd_loop();
            if(cmd_res == false){
                AddLog("未知命令:"+ last_cmd_txt);
            }

            if(framecount % 60 !=0 )
              continue;

            lifeTime--;
            if(lifeTime <= 0){
                OnRoundEnd(m_round);
                lifeTime = roundTimeMax;
                m_round++;
                if(curCoins.a <= 0){
                    exit = false;
                    UIBase::clearScreen();
                    UIBase::printAt(gRows/2, (gCols-10)/2, "游戏结束！A资源耗尽", RED);
                    std::cout.flush();
                    sleep(2);
                }
            }
        }
        
        UIBase::printAt(1, 1,  "退出游戏！", BLUE);
    }

void OnRoundEnd(int round){
        // 扣除资源
        // 随机选择3种不同资源/货币
        int random = rand() % 3 + 3; // 3~5
        int types[3] = { rand() % 3 + 3, rand() % 3 + 3, rand() % 3 + 3}; // 0:A, 1:B, 2:C, 3:D, ..., 12:K
        std::string log = std::to_string(round) + "回合结束 扣除资源:";
         curCoins.a -= types[0];
         curCoins.b -= types[1];
         curCoins.c -= types[2];
         log += "A:-" + std::to_string(types[0]) + " B:-" + std::to_string(types[1]) + " C:-" + std::to_string(types[2]);
         AddLog(log);
    }
};
