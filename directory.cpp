#include <iostream>
#include <bits/stdc++.h>
#include <string>

using namespace std;

#include "custom.h"

string currentDir = "";

char HOME_DIRECTORY[2048]; 

bool isTruePath = true;

string getCurrentDirectory(){
    return currentDir;
}
void setCurrentDirectory(string x){
    string sub = HOME_DIRECTORY;
    isTruePath = false;
    size_t ind = x.find(sub);
    if(ind != string::npos){
        x.erase(ind, sub.length());
        isTruePath = true;
    }
    currentDir = x;
}

string getHomeDirectory(){
    return HOME_DIRECTORY;
}

void setHomeDirectory(){
    if(getcwd(HOME_DIRECTORY, sizeof(HOME_DIRECTORY)) == nullptr){
        perror("getcwd");
    }
    return;
}

bool getIsTruePath(){
    return isTruePath;
}