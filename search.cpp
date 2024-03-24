#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <dirent.h>

using namespace std;

#include "custom.h"

bool isExist = false;

int recursiveSearch(string dirPath, string name) {
    DIR* dir = opendir(dirPath.c_str());

    if (dir == nullptr) {
        perror("opendir");
        return 1;
    }

    struct dirent* entry;

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, name.c_str()) == 0) {
            isExist = true;
            closedir(dir);
            return 0;
        }

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            string newPath = dirPath + "/" + entry->d_name;
            recursiveSearch(newPath, name);
        }
    }

    closedir(dir);
    return 0;
}

int recursiveSearchHandler(vector <string> tokens){
    if(tokens.size() > 2){
        cerr << "search has too many arguments" << endl;
        return 1;
    }
    if(tokens.size() == 1){
        cerr << "search has only few arguments" << endl;
        return 1;
    }
    recursiveSearch(".", tokens[1]);
    if(isExist == true) cout << "Yes" << endl;
    else cout << "No" << endl;
    isExist = false;
    return 0;
}