#include <iostream>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctime>
#include <pwd.h> 
#include <grp.h> 

using namespace std;

#include "custom.h"

string sanitizePath(string path){
    if(path.size() == 1 && path[0] == '~'){
        return getHomeDirectory();
    }
    if(path[0] == '.' && path[1] == '/') {
        string sanitizedPath = pwd_internal() + path.substr(1);
            
        char resolvedPath[PATH_MAX];
        if (realpath(sanitizedPath.c_str(), resolvedPath) == NULL) {
            perror("realpath");
        }
        string rp = resolvedPath;
        return rp;
    }
    
    if(path[0] == '/') {
        string sanitizedPath = getHomeDirectory() + path;
        char resolvedPath[PATH_MAX];
        if (realpath(sanitizedPath.c_str(), resolvedPath) == NULL) {
            perror("realpath");
        }
        return resolvedPath;
    }

    string sanitizedPath = pwd_internal() + "/" + path;
    // cout << "SP11 : " << sanitizedPath << endl;
    char resolvedPath[PATH_MAX];
    if (realpath(sanitizedPath.c_str(), resolvedPath) == NULL) {
        perror("realpath");
    }
    return resolvedPath;
}

int ls_command(bool isA,bool isL){
    //TODO - here we need to set complete absolute path from tokenizer
    string path = "";

    path = pwd_internal();

    //: Vector for storing all filenames
    DIR *dp = NULL;
    struct dirent *entry;
    vector <string> filelist;
    //: Opening Dir with given path
    dp = opendir((char *)path.c_str());
    if (dp == NULL) {
        perror("opendir");
        return 1;
    }
    while ((entry = readdir(dp))) {
        filelist.push_back(entry->d_name);
    }

    // sorting filelist based on filenames
    sort(filelist.begin(), filelist.end());


    //: Iterating all files one by one
    for(auto it: filelist){
        struct stat file_stat;
        string filename = it;

        //: Setting absolute / relative path from ./a.out
        string pathForFile = "./" + filename;

        //: Abstacting data of file using stat call
        if (stat((char *)pathForFile.c_str(), &file_stat) != 0) {
            perror("stat");
            return 1;
        }
        
        //: Reading permissions of files, eg. dr-xr--rw-
        string permissions = "";
        (S_ISDIR(file_stat.st_mode)) ? permissions.push_back('d') : permissions.push_back('-');
        // Owner's permissions
        (file_stat.st_mode & S_IRUSR) ? permissions.push_back('r') : permissions.push_back('-');
        (file_stat.st_mode & S_IWUSR) ? permissions.push_back('w') : permissions.push_back('-');
        (file_stat.st_mode & S_IXUSR) ? permissions.push_back('x') : permissions.push_back('-');

        // Group's permissions
        (file_stat.st_mode & S_IRGRP) ? permissions.push_back('r') : permissions.push_back('-');
        (file_stat.st_mode & S_IWGRP) ? permissions.push_back('w') : permissions.push_back('-');
        (file_stat.st_mode & S_IXGRP) ? permissions.push_back('x') : permissions.push_back('-');

        // Others' permissions
        (file_stat.st_mode & S_IROTH) ? permissions.push_back('r') : permissions.push_back('-');
        (file_stat.st_mode & S_IWOTH) ? permissions.push_back('w') : permissions.push_back('-');
        (file_stat.st_mode & S_IXOTH) ? permissions.push_back('x') : permissions.push_back('-');

        //: -l flag
        if(isL){
            if(isA || (filename != "." && filename != "..")) {
                time_t modifiedTime = file_stat.st_mtime;
                struct tm* timeinfo = localtime(&modifiedTime);
                char buffer[128];
                strftime(buffer, 128, "%c", timeinfo);
                string last_modified = buffer;

                cout << permissions << " ";
                cout << file_stat.st_nlink << " ";
                cout << getpwuid(file_stat.st_uid)->pw_name << " ";
                cout << getgrgid(file_stat.st_gid)->gr_name << " ";
                cout << setw(9) << file_stat.st_size << " ";
                cout << last_modified.substr(4, 12) << " ";
            }   
        }

        //: -a flag
        if(isA || (filename != "." && filename != "..")) {
            cout 
            << ((permissions.find("x") != string::npos) ? "\033[1;32m" : "")
            << ((permissions.find("d") != string::npos) ? "\033[1;34m" : "") 
            << filename
            << ((permissions.find("d") != string::npos || permissions.find("x") != string::npos) ? "\033[1;0m" : "")  << " ";
            if(isL) cout << endl;
        }
    }
    if(!isL) cout << endl;
    
    closedir(dp); 

    return 0;
}

int ls_multiple_files(bool isA,bool isL, vector <string> files_vector){
    //TODO - here we need to set complete absolute path from tokenizer
    for(auto it: files_vector){
        cout << "SP : " << sanitizePath(it) << endl;
        struct stat file_stat;
        string pathForFile = sanitizePath(it);
        cout << "PFF : " << pathForFile << endl;
        if (stat((char *)pathForFile.c_str(), &file_stat) != 0) {
            perror("stat");
            return 1;
        }
        //: If path is directory
        if(S_ISDIR(file_stat.st_mode)){
            // : Changing current directory (temporary - for ls_command()) to given directory path
            string curDir = pwd_internal();
            if (chdir((char *)pathForFile.c_str()) != 0) {
                perror("chdir");
                return 1;
            }
            char cwd[2048];
            if(getcwd(cwd, sizeof(cwd)) != nullptr) {
                setCurrentDirectory(cwd);
            } else {
                perror("getcwd");
                return 1;
            }

            //: ls of directories
            if(files_vector.size() > 1 ) cout << endl << it << endl;
            ls_command(isA, isL);

            //: Changing currentDirectory to actual current directory
            if (chdir((char *)curDir.c_str()) != 0) {
                perror("chdir");
                return 1;
            }
            if(getcwd(cwd, sizeof(cwd)) != nullptr) {
                setCurrentDirectory(cwd);
            } else {
                perror("getcwd");
                return 1;
            }
        }
        //: If path is file
        else {
            //: Reading permissions of files, eg. dr-xr--rw-
            string permissions = "";
            (S_ISDIR(file_stat.st_mode)) ? permissions.push_back('d') : permissions.push_back('-');
            // Owner's permissions
            (file_stat.st_mode & S_IRUSR) ? permissions.push_back('r') : permissions.push_back('-');
            (file_stat.st_mode & S_IWUSR) ? permissions.push_back('w') : permissions.push_back('-');
            (file_stat.st_mode & S_IXUSR) ? permissions.push_back('x') : permissions.push_back('-');

            // Group's permissions
            (file_stat.st_mode & S_IRGRP) ? permissions.push_back('r') : permissions.push_back('-');
            (file_stat.st_mode & S_IWGRP) ? permissions.push_back('w') : permissions.push_back('-');
            (file_stat.st_mode & S_IXGRP) ? permissions.push_back('x') : permissions.push_back('-');

            // Others' permissions
            (file_stat.st_mode & S_IROTH) ? permissions.push_back('r') : permissions.push_back('-');
            (file_stat.st_mode & S_IWOTH) ? permissions.push_back('w') : permissions.push_back('-');
            (file_stat.st_mode & S_IXOTH) ? permissions.push_back('x') : permissions.push_back('-');

            //: -l flag
            string filename = pathForFile;
            size_t lastSlashPos = pathForFile.find_last_of('/');
            if (lastSlashPos != string::npos) {
                filename = pathForFile.substr(lastSlashPos + 1);
            }
            if(isL){
                if(isA || (filename != "." && filename != "..")) {
                    time_t modifiedTime = file_stat.st_mtime;
                    struct tm* timeinfo = localtime(&modifiedTime);
                    char buffer[128];
                    strftime(buffer, 128, "%c", timeinfo);
                    string last_modified = buffer;
                    cout << endl;
                    cout << permissions << " ";
                    cout << file_stat.st_nlink << " ";
                    cout << getpwuid(file_stat.st_uid)->pw_name << " ";
                    cout << getgrgid(file_stat.st_gid)->gr_name << " ";
                    cout << setw(9) << file_stat.st_size << " ";
                    cout << last_modified.substr(4, 12) << " ";
                }   
            }
            //: -a flag
            if(isA || (filename != "." && filename != "..")) {
                cout 
                << ((permissions.find("x") != string::npos) ? "\033[1;32m" : "")
                << ((permissions.find("d") != string::npos) ? "\033[1;34m" : "") 
                << it
                << ((permissions.find("d") != string::npos || permissions.find("x") != string::npos) ? "\033[1;0m" : "")  << " ";
                if(isL) cout << endl;
            }
        }
    }

    return 0;
}


void ls_handler(vector <string> tokens){
    vector <string> files;
    bool isA=false, isL=false;
    
    for(auto it: tokens){
        if(it == "-a"){
            isA = true;
        } 
        else if(it == "-l"){
            isL = true;
        }
        else if(it == "-al" || it == "-la"){
            isL = true;
            isA = true;
        }
        else {
            files.push_back(it);
        }
    }

    if(files.size() == 1){
        int result = ls_command(isA, isL);
        if(result != 0) {
            cout << "Some error occured" << endl;
        }
        // cout << "Res1 : " << result << endl;
    } else {
        files.erase(files.begin());
        int result = ls_multiple_files(isA, isL, files);
        if(result != 0) {
            cout << "Some error occured" << endl;
        }
        // cout << "Res2 : " << result << endl;
    }
}