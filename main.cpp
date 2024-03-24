#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <bits/stdc++.h>
#include <stdio.h>


using namespace std;

#include "custom.h"

int main(){
    cout<< endl;
    setHomeDirectory();
    while(1){
        prompt();
    
        string input;
        getline(cin, input);
        vector <string> tokens;

        getTokens(tokens, ";", input);

        for(auto it : tokens){
            vector <string> subTokens;
            getTokens(subTokens, " ", it);

            string command_name = subTokens[0];

            string lastToken = subTokens.back();

            auto it1 = find(subTokens.begin(), subTokens.end(), "<");
            auto it2 = find(subTokens.begin(), subTokens.end(), ">");
            auto it3 = find(subTokens.begin(), subTokens.end(), ">>");
            if (it1 != subTokens.end() || it2 != subTokens.end() || it3 != subTokens.end()) {
                // *it
                ioRedirection(subTokens);
            } else if(lastToken == "&" || lastToken[lastToken.size() - 1] == '&'){
                background_process_command(subTokens);
            } else if(command_name == "cd"){
                cd_command(subTokens);
            } else if (command_name == "echo") {
                echo_command(subTokens);   
            } else if (command_name == "pwd") {
                pwd_command();
            } else if (command_name == "ls") {
                ls_handler(subTokens);
            } else if (command_name == "pinfo") {
                pinfo(subTokens);
            } else if (command_name == "search") {
                recursiveSearchHandler(subTokens);
            } else if (command_name == "exit") {
                exit(0);
            }
            else {
                foreground_process_command(subTokens);
            }
        }
    }
    return 0;
}