
#pragma once

#include <iostream>

#define DebugPrintDisabled 1

class DebugPrint {
    
    
public:
    static void print(std::string str){
        if(DebugPrintDisabled){
            return;
        }
        std::cout <<str;
    }
};