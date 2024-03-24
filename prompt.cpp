#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#include "custom.h"

void prompt(){
    // char *currentDir[2048] = ;
    string currentDir = getCurrentDirectory();
    
    char hostname[256];
    if(gethostname(hostname, sizeof(hostname)) != 0){
        cerr << "Error while fetching the hostname/systemname." << endl;
        return;
    }

    char* username = getenv("USER");
    if(!username){
        cerr << "Unable to find username of the system." << endl;
        return;
    }

    cout << "\033[1;32m" << username << "@" <<  hostname  << "\033[1;0m:\033[1;35m~" << currentDir << "\033[1;0m > ";

}