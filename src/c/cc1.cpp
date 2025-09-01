#include <iostream>
#include <iterator>
#include <windows.h>
#include <chrono>
#include <conio.h>  // 用于_kbhit()和_getch()
#include <string>  // 用于字符串转换和截取

// 控制程序状态的标志
bool running = true;    // 程序是否运行
bool paused = false;    // 是否暂停

int hp = 3 ;
long long lasttime = 0;
long long gameTime = 0;
int curSec = 0;
int lastSec = 0;
int hpTime = 3;
// 线程函数：定时打印纳秒时间
DWORD WINAPI PrintNanoSeconds(LPVOID lpParam) {
    auto now = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
    lasttime = ns.time_since_epoch().count();
    while (running) {
        if (!paused) {
            // 获取当前时间（纳秒级）
            auto now = std::chrono::high_resolution_clock::now();
            auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
            auto value = ns.time_since_epoch().count();
            
            gameTime += value - lasttime ; 
            curSec = gameTime / 1000000000;
            hpTime -= (curSec - lastSec);
         
            // 打印纳秒时间
            std::cout <<"游戏时间:" << gameTime << " HP:" << hp << " 充能倒计时："<< hpTime << " 当前秒数:"<< curSec << std::endl;
            if(hpTime <= 0){
                hpTime = 3;
                hp++;
            }

            lasttime =  value;
            lastSec = curSec;
        }
 
        // 等待10毫秒
        Sleep(10);
    }
    return 0;
}

void OnPause(){
    std::string num_str = std::to_string(gameTime);
    std::string middle_part = num_str.substr(4, num_str.length() - 4 - 2);
    std::cout << "\n定格时间:" << middle_part << " HP:" << hp << std::endl;
}

// 线程函数：监听键盘输入
DWORD WINAPI KeyboardListener(LPVOID lpParam) {
    while (running) {
        if (_kbhit()) {  // 检查是否有键盘输入
            char key = _getch();  // 获取按键
            if (key == ' ') {
                if(hp > 0)     // 空格键
                    paused = !paused;  // 切换暂停状态
                else{
                    std::cout << "hp 不足 无法暂停...\n";
                    paused = false;
                }

                if (paused) {
                    hp--;
                  //  std::cout  << "定格时间："<< gameTime << " hp:"<< hp << std::endl;
                  
                    OnPause();
                } else {
                    std::cout << "已继续...\n";
                }

            } else if (key == 27) {  // ESC键
                running = false;     // 退出程序
                std::cout << "\n程序即将退出...\n";
            }
        }
        Sleep(50);  // 减少CPU占用
    }
    return 0;
}

int main() {
    std::cout << "开始每10毫秒打印当前纳秒时间...\n";
    std::cout << "按空格键暂停/继续，按ESC键退出程序\n";
    
    // 创建打印线程
    HANDLE hPrintThread = CreateThread(
        NULL,
        0,
        PrintNanoSeconds,
        NULL,
        0,
        NULL
    );
    
    // 创建键盘监听线程
    HANDLE hKeyThread = CreateThread(
        NULL,
        0,
        KeyboardListener,
        NULL,
        0,
        NULL
    );
    
    // 等待线程结束
    WaitForSingleObject(hPrintThread, INFINITE);
    WaitForSingleObject(hKeyThread, INFINITE);
    
    // 关闭线程句柄
    CloseHandle(hPrintThread);
    CloseHandle(hKeyThread);
    
    std::cout << "程序已停止\n";
    return 0;
}
    