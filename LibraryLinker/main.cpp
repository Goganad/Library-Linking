#include <iostream>
#include <utility>
#include "windows.h"
#include "../StringReplacer/replacer.h"

using namespace std;

void staticLinking(pid_t pid, string source, string replacement) {
    replaceString(pid, move(source), move(replacement));
}

void dynamicLinking(pid_t pid, const char *lib, string source, string replacement) {
    typedef void(*change)(pid_t pid, string source, string replacement);

    HMODULE libraryHandle = LoadLibraryA(lib);
    if (!libraryHandle) {
        cout << "Can't load library" << endl;
        return;
    }

    auto func = (change) GetProcAddress(libraryHandle, "replaceString");
    if (!func) {
        cout << "Can't find the replacement function" << endl;
        cout << GetLastError() << endl;
        return;
    }

    func(pid, move(source), move(replacement));
}


void remoteProcessInjection(pid_t pid, const char *dllPath) {
    HANDLE targetProcess;

    targetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (targetProcess) {

        auto loadLib = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
        if (loadLib) {
            auto allocatedMemory = VirtualAllocEx(targetProcess, nullptr, strlen(dllPath) + 1,
                                                  MEM_RESERVE | MEM_COMMIT,
                                                  PAGE_READWRITE);
            if (allocatedMemory) {
                if (WriteProcessMemory(targetProcess, (LPVOID) allocatedMemory, dllPath, strlen(dllPath) + 1, nullptr)) {
                    HANDLE thread = CreateRemoteThread(targetProcess, nullptr, 0, (LPTHREAD_START_ROUTINE) loadLib,
                                                       (LPVOID) allocatedMemory, 0, nullptr);
                    if (thread) {
                        WaitForSingleObject(thread, INFINITE);
                        cout << "Injected DLL in target process" << std::endl;
                        CloseHandle(thread);
                    } else {
                        cout << "Can't create remote thread" << std::endl;
                    }
                } else {
                    cout << "Can't write string to process memory" << std::endl;
                }
                VirtualFreeEx(targetProcess, allocatedMemory, 0, MEM_RELEASE);
            } else {
                cout << "Can't allocate memory for remote string" << std::endl;
            }
        } else {
            cout << "Can't get function LoadLibraryA from kernel32.dll" << std::endl;
        }

        CloseHandle(targetProcess);
    } else {
        cout << "Error accessing target process" << std::endl;
    }
}

int main() {
    pid_t pid;
    cout << "Enter target thread PID " << endl;
    cin >> pid;
    const char *lib = "../../StringReplacer/cmake-build-debug/libstring_replacer.dll";

    remoteProcessInjection(pid, lib);
    Sleep(4000);
    staticLinking(pid, "MULTIVERSE", "CRITICAL DENSITY");
    Sleep(4000);
    dynamicLinking(pid, lib, "CRITICAL DENSITY", "SINGULARITY");

    return 0;
}

