#include <iostream>
#include <string>

// �ȼ���Ƿ�ΪWindows����
#ifdef _WIN32
#pragma message("_WIN32 �Ѷ��� - ʹ��Windows����")
#include <windows.h>  // Windows APIͷ�ļ�

// 1. �滻usleep��Windows��Sleep����λ���룩
#define usleep(microseconds) Sleep((microseconds + 999) / 1000)  // ΢��ת���루�������룩

// 2. Windows����̨��ɫ�������滻ANSIת���룩
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

// 3. Windows���ÿ���̨��ɫ���滻ANSIת�壩
void setWinColor(WinColor fgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, fgColor);  // ����ǰ��ɫ
}

// 4. Windows���ÿ���̨��ɫ���ָ�Ĭ�ϣ�
void resetWinColor() {
    setWinColor(WIN_WHITE);  // �ָ���ɫĬ��ɫ
}

// 5. Windows�������滻ANSIת�壩
void clearWinScreen() {
    system("cls");  // Windows��������
}

// 6. Windows�ƶ���굽ָ��λ�ã��滻ANSIת�壩
void moveWinCursor(int row, int col) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(col - 1), static_cast<SHORT>(row - 1) };  // Windows�����(0,0)��ʼ
    SetConsoleCursorPosition(hConsole, pos);
}

// ��Windows������Linux/macOS������ԭANSIת��ʵ��
#elif defined(__linux__) || defined(__APPLE__)
#pragma message("ʹ����Unix����(Linux/macOS)")
#include <unistd.h>

// ��ɫ����ö�٣�ANSIת���룩
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

// �����ı���ɫ��ANSI��
void setColor(Color color) {
    std::cout << "\033[" << color << "m";
}

// �����ı���ɫ��ANSI��
void resetColor() {
    setColor(RESET);
}

// ������ANSI��
void clearScreen() {
    std::cout << "\033[H\033[J";
}

// �ƶ���겢��ӡ�ı���ANSI��
void printAt(int row, int col, const std::string& text, Color color = WHITE) {
    std::cout << "\033[" << row << ";" << col << "H";
    setColor(color);
    std::cout << text;
    resetColor();
}

#else
#pragma message("δ֪����ϵͳ����֧�ֵ�ǰ����")
#error "��ǰ����ϵͳ��֧�ֱ�����Ŀ���̨�����߼�"
#endif

// ��ȡ�ն˳ߴ磨�򻯰棬��ƽ̨ͨ�ã�
void getTerminalSize(int& rows, int& cols) {
    // ʵ����Ŀ��ͨ��Windows API��GetConsoleScreenBufferInfo������Unix��ioctl����ȡ��ʵ�ߴ�
    rows = 24;  // �����ն�24��
    cols = 80;  // �����ն�80��
}

int main() {
    int rows, cols;
    getTerminalSize(rows, cols);
    int frame = 0;

    // �򵥵Ķ�����ʾ
    while (true) {
        // 1. ������Windows/Linux�ֱ���
#ifdef _WIN32
        clearWinScreen();
#else
        clearScreen();
#endif

        // 2. ���Ʊ߿򣨿�ƽ̨�߼�ͳһ������ƽ̨�ض��ӿڣ�
#ifdef _WIN32
        setWinColor(WIN_CYAN);  // Windows��ɫ
        // �ϱ߿�
        moveWinCursor(1, 1);
        for (int i = 1; i <= cols; ++i) std::cout << "-";
        // �±߿�
        moveWinCursor(rows, 1);
        for (int i = 1; i <= cols; ++i) std::cout << "-";
        // ���ұ߿�
        for (int i = 2; i < rows; ++i) {
            moveWinCursor(i, 1);   std::cout << "|";
            moveWinCursor(i, cols); std::cout << "|";
        }
        resetWinColor();  // ����Windows��ɫ
#else
        setColor(CYAN);  // Linux��ɫ
        // �ϱ߿�
        printAt(1, 1, std::string(cols, '-'), CYAN);
        // �±߿�
        printAt(rows, 1, std::string(cols, '-'), CYAN);
        // ���ұ߿�
        for (int i = 2; i < rows; ++i) {
            printAt(i, 1, "|", CYAN);
            printAt(i, cols, "|", CYAN);
        }
        resetColor();  // ����Linux��ɫ
#endif

        // 3. ��ʾ��ɫ�ı���ƽ̨�ض��ӿڣ�
#ifdef _WIN32
        moveWinCursor(3, 5);
        setWinColor(WIN_MAGENTA); std::cout << "Windows ����̨��ɫ�����ʾ"; resetWinColor();

        moveWinCursor(5, 5);
        setWinColor(WIN_GREEN); std::cout << "ȫ��ˢ��: ÿ��ѭ�����������Ļ"; resetWinColor();

        moveWinCursor(6, 5);
        setWinColor(WIN_GREEN); std::cout << "������ɫ: �����������ɫ��"; resetWinColor();

        moveWinCursor(7, 5);
        setWinColor(WIN_RED); std::cout << "��������Ǻ�ɫ��"; resetWinColor();

        moveWinCursor(8, 5);
        setWinColor(WIN_BLUE); std::cout << "�����������ɫ��"; resetWinColor();

        moveWinCursor(9, 5);
        setWinColor(WIN_YELLOW); std::cout << "��������ǻ�ɫ��"; resetWinColor();
#else
        printAt(3, 5, "Linux/macOS ����̨��ɫ�����ʾ", MAGENTA);
        printAt(5, 5, "ȫ��ˢ��: ÿ��ѭ�����������Ļ", GREEN);
        printAt(6, 5, "������ɫ: �����������ɫ��", GREEN);
        printAt(7, 5, "��������Ǻ�ɫ��", RED);
        printAt(8, 5, "�����������ɫ��", BLUE);
        printAt(9, 5, "��������ǻ�ɫ��", YELLOW);
#endif

        // 4. �ƶ��Ķ���Ԫ��
#ifdef _WIN32
        int pos = (frame % (cols - 10)) + 2;
        moveWinCursor(12, pos);
        setWinColor(WIN_MAGENTA); std::cout << "�ƶ���..."; resetWinColor();
#else
        int pos = (frame % (cols - 10)) + 2;
        printAt(12, pos, "�ƶ���...", MAGENTA);
#endif

        // 5. ��ʾ֡��
#ifdef _WIN32
        moveWinCursor(rows - 2, 5);
        setWinColor(WIN_YELLOW); std::cout << "֡����: " << frame; resetWinColor();
#else
        printAt(rows - 2, 5, "֡����: " + std::to_string(frame), YELLOW);
#endif

        // 6. ˢ���������ʱ
        std::cout.flush();
        usleep(1000000/60);  // 100���루��ƽ̨���ݣ���ͨ�������䣩

        frame++;
        // ��Ctrl+C�˳�����
    }

    return 0;
}