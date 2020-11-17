//
// Created by Игорь on 15.11.2020.
//

#ifndef STRINGREPLACER_STRINGREPLACER_H
#define STRINGREPLACER_STRINGREPLACER_H

#include <iostream>
#include "windows.h"

extern "C" void replaceString(pid_t pid, std::string source, std::string replacement);

#endif //STRINGREPLACER_STRINGREPLACER_H
