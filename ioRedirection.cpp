#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;

#include "custom.h"

int ioRedirection(vector <string> tokens){
    string input_file = "", output_file = "", command_name=tokens[0];
    auto it1 = find(tokens.begin(), tokens.end(), "<");
    int new_stdin, new_stdout;

    int original_stdin = dup(0);
    int original_stdout = dup(1);

    if(it1 != tokens.end() && (it1 + 1) != tokens.end()){
        input_file = *(it1 + 1);
        tokens.erase(it1+1);
        tokens.erase(it1);
        new_stdin = open(input_file.c_str(), O_RDONLY);

        if (new_stdin == -1) {
            perror("open");
            return 1;
        }

        if (dup2(new_stdin, 0) == -1) {
            perror("dup2");
            return 1;
        }
        close(new_stdin);

    }
    auto it2 = find(tokens.begin(), tokens.end(), ">");
    if(it2 != tokens.end() && (it2 + 1) != tokens.end()){
        output_file = *(it2 + 1);
        tokens.erase(it2+1);
        tokens.erase(it2);
        new_stdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (new_stdout == -1) {
            perror("open");
            return 1;
        }


        if (dup2(new_stdout, 1) == -1) {
            perror("dup2");
            return 1;
        }
        close(new_stdout);
    }

    auto it3 = find(tokens.begin(), tokens.end(), ">>");

    if(it3 != tokens.end() && (it3 + 1) != tokens.end()){
        output_file = *(it3 + 1);
        tokens.erase(it3+1);
        tokens.erase(it3);
        new_stdout = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);

        if (new_stdout == -1) {
            perror("open");
            return 1;
        }

        if (dup2(new_stdout, 1) == -1) {
            perror("dup2");
            return 1;
        }

        close(new_stdout);
    }
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
    }

    dup2(original_stdin, 0);
    dup2(original_stdout, 1);

    close(original_stdin);
    close(original_stdout);


    return 0;

}
