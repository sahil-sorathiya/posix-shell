#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#include "custom.h"

string previous = ""; 
string current = ""; 

int cd_command(vector <string> &v){
    if(previous == "") {
        previous = getHomeDirectory();
    }

    if(current == "") {
        current = getHomeDirectory();
    }

    //: If more than two args, then error
    if(v.size() > 2){
        cout << "Invalid arguments to CD" << endl;
        return 1;
    }

    //: CD to Home
    if(v.size() == 1 || v[1] == "~"){
        string h = getHomeDirectory();
        if (chdir((char *)h.c_str()) != 0) {
            perror("chdir");
            return 1;
        }
        char cwd[2048];
        if(getcwd(cwd, sizeof(cwd)) != nullptr) {
            setCurrentDirectory(cwd);
            previous = current;
            current = cwd;
        } else {
            perror("getcwd");
            return 1;
        }
        return 0;
    }

    //: Toggling last CD
    if(v[1] == "-"){
        if (chdir((char *)previous.c_str()) != 0) {
            perror("chdir");
            return 1;
        }
        char cwd[2048];
        if(getcwd(cwd, sizeof(cwd)) != nullptr) {
            setCurrentDirectory(cwd);
            previous = current;
            current = cwd;
        } else {
            perror("getcwd");
            return 1;
        }
        return 0;
    }

    //: Stop CD to parent div if current == home
    if(v[1] == ".."){
        // if(getCurrentDirectory() == "" || getCurrentDirectory() == getHomeDirectory()){
        // if(getCurrentDirectory() == ""){
            // cout << getHomeDirectory() << endl;
            // return 0;
        // }
    }

    //: CD to given path
    if (chdir((char *)v[1].c_str()) != 0) {
        perror("chdir");
        return 1;
    }
    char cwd[2048];
    if(getcwd(cwd, sizeof(cwd)) != nullptr) {
        setCurrentDirectory(cwd);
        previous = current;
        current = cwd;
    } else {
        perror("getcwd");
        return 1;
    }
    return 0;

}

int echo_command(vector <string> &v){
    for(int i = 1; i < v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
    return 0;
}

void pwd_command(){
    string str1 = getHomeDirectory();
    string str2 = getCurrentDirectory();
    string str = "";
    if(getIsTruePath()){
        str = str1 + str2;
    } else {
        str = str2;
    }
    cout << str << endl;
}


string pwd_internal(){
    string str1 = getHomeDirectory();
    string str2 = getCurrentDirectory();
    string str = "";
    if(getIsTruePath()){
        str = str1 + str2;
    } else {
        str = str2;
    }
    return str;
}