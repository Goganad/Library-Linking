#include <iostream>
#include "windows.h"
#include <thread>

using namespace std;

const char str[100] = "Some Context";
char *s;

int main() {
    s = (char*)calloc(1024, sizeof(char));
    strcpy(s, str);
    pid_t pid = _getpid();
    cout << "Process with PID " << pid << " started" << endl;

    while (true){
        cout << s << endl;
        Sleep(1000);
    }

    return 0;
}
