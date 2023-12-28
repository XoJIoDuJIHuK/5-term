#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

int main() {
    tm newtime;
    long time;
    _time32(&time);
    _localtime32_s(&newtime, &time);
    char c_year[] = "000";
    int year = newtime.tm_year + 1900;
    for (int i = 0; i < 3; i++) {
        c_year[2 - i] = 48 + year % 10;
        year /= 10;
    }
    printf("%d.%d.%s.%d.%d:%d\n",newtime.tm_mday, newtime.tm_mon + 1, c_year,
        newtime.tm_hour, newtime.tm_min, newtime.tm_sec);
};