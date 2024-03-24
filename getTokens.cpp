#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#include "custom.h"

int getTokens(vector <string> &tokens, string div, string i){
    // cout << "The tokens are:" << endl;
    char *token = strtok((char*)i.c_str(), div.c_str());
    while (token != nullptr){
        // cout << token << endl;
        tokens.push_back(token);
        token = strtok(NULL, " ");
    }
    return 0;
}