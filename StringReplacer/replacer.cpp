#include "replacer.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include "windows.h"

using namespace std;

void replaceString(pid_t pid, string source, string replacement) {

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (process != nullptr) {
        MEMORY_BASIC_INFORMATION memInfo;
        unsigned char *addr = nullptr;

        while (VirtualQueryEx(process, addr, &memInfo, sizeof(memInfo))) {
            if ((memInfo.State == MEM_COMMIT) && (memInfo.Type != MEM_IMAGE)) {
                size_t bytesRead;
                vector<char> buffer;
                buffer.resize(memInfo.RegionSize);

                ReadProcessMemory(process, addr, buffer.data(), memInfo.RegionSize, &bytesRead);
                buffer.resize(bytesRead);

                auto searchResult = search(buffer.begin(), buffer.end(), source.begin(), source.end());

                if (searchResult != buffer.end()) {
                    int offset = distance(buffer.begin(), searchResult);
                    LPVOID stringAddress = addr + offset;

                    if (WriteProcessMemory(process, stringAddress, replacement.data(), replacement.length() + 1,
                                           nullptr)) {
                        cout << "Successfully altered string " << endl;
                    } else {
                        DWORD error = GetLastError();
                        cout << "Error writing to virtual memory " << error << endl;
                    }
                }
            }
            addr += memInfo.RegionSize;
        }
        CloseHandle(process);
    } else {
        std::cout << "Can't connect to Process  " << pid << std::endl;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            cout << "DLL ATTACHED TO PROCESS" << endl;
            replaceString(_getpid(), "Some Context", "MULTIVERSE");
            break;
        case DLL_THREAD_ATTACH:
            cout << "DLL ATTACHED TO THREAD" << endl;
            break;
        case DLL_THREAD_DETACH:
            cout << "DLL DETACHED FROM THREAD" << endl;
            break;
        case DLL_PROCESS_DETACH:
            cout << "DLL DETACHED FROM PROCESS" << endl;
            break;
    }

    return TRUE;
}

