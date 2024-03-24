#include <iostream>
#include <bits/stdc++.h>
#include <sys/wait.h>

using namespace std;

#include "custom.h"

int background_process_command(vector <string> tokens){
    //: NOTE : Here i am assuming that all tokens are sapatated by space only

    //: Removing "&" from the tokens
    string lastToken = tokens.back();
    tokens.pop_back();
    if(lastToken != "&") {
        lastToken.pop_back();
        tokens.push_back(lastToken);
    }

    pid_t child_process_id = fork(); 

    if (child_process_id == -1) {
        cerr << "Fork for child process failed." << endl;
        return 1;
    }

    if (child_process_id == 0) {
        vector<char*> tokenStr;

        for(auto it: tokens){
            tokenStr.push_back(strdup(it.c_str()));
        }
        tokenStr.push_back(nullptr);
        
        if (execvp(tokenStr[0], tokenStr.data()) == -1) {
            cerr << "Execvp failed." << endl;
            return 1;
        }
        
    } else {
        cout << "PID: " << child_process_id << endl;
    }
    return 0;
}

int foreground_process_command(vector <string> tokens){
    //: NOTE : Here i am assuming that all tokens are sapatated by space only
    pid_t child_process_id = fork();

    if (child_process_id == -1) {
        cerr << "Fork failed." << endl;
        return 1;
    }

    if (child_process_id == 0) {

        vector<char*> tokenStr;
        for(auto it: tokens){
            tokenStr.push_back(strdup(it.c_str()));
        }
        tokenStr.push_back(nullptr);
        
        if (execvp(tokenStr[0], tokenStr.data()) == -1) {
            cerr << "Execvp failed." << endl;
            return 1;
        }

    } else {
        waitpid(child_process_id, nullptr, 0);
        // vector <string> v;
        // v.push_back("pinfo");
        // pinfo(v);
    }
    return 0;
}

void pinfo(vector <string> tokens) {
    if(tokens.size() > 2){
        cerr << "pinfo has too much arguments" << endl;
        return;
    }
    pid_t pid;
    if(tokens.size() == 1){
        pid = getpid();
    } else {
        pid = stoi(tokens[1]);
    }
    string procDir = "/proc/" + to_string(pid);
    ifstream statusFile((procDir + "/status").c_str());

    if (!statusFile) {
        cerr << "Error: Process with PID " << pid << " not found." << endl;
        return;
    }

    string line;
    while (getline(statusFile, line)) {
        istringstream iss(line);
        string key, value;

        if (getline(iss, key, ':') && getline(iss, value)) {
            // if(key == "Pid" || key == "State" || key == "VmSize" ) cout << key << ": " << value << endl;
            if(key == "Pid") cout << "pid -- " << value << endl;
            if(key == "State") cout << "Process Status -- " << value << endl;
            if(key == "VmSize") cout << "memory -- " << value << endl;
        }
    }
    char exePath[4096]; // Assuming a reasonable buffer size
    ssize_t len = readlink(("/proc/" + to_string(pid) + "/exe").c_str(), exePath, sizeof(exePath) - 1);
    if (len != -1) {
        exePath[len] = '\0'; // Null-terminate the string
        cout << "Executable Path -- " << exePath << endl;
    }

    statusFile.close();
}