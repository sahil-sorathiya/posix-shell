#ifndef CUSTOM_H
#define CUSTOM_H

using namespace std;

string getHomeDirectory();
void setHomeDirectory();

string getCurrentDirectory();
void setCurrentDirectory(string x);

bool getIsTruePath();

void prompt();

int cd_command(vector <string> &v);

int getTokens(vector <string> &tokens, string div, string i);

int echo_command(vector <string> &v);

void pwd_command();
string pwd_internal();

int ls_command(bool isA,bool isL);
int ls_multiple_files(bool isA,bool isL, vector <string> files_vector);
void ls_handler(vector <string> tokens);

int background_process_command(vector <string> tokens);
int foreground_process_command(vector <string> tokens);
void pinfo(vector <string> tokens);

int recursiveSearch(string dirPath, string name);
int recursiveSearchHandler(vector <string> tokens);     

int ioRedirection(vector <string> tokens);

#endif // CUSTOM_H