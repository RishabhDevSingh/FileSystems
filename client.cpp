#include <iostream>
//#include "DFactory.cpp"
#include "objects.cpp"
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>

using namespace std;

SubDir* initialize();
vector<string> parse_line(string line);
void print(vector<SubDir*>);
void print(vector<File*>);
SubDir *find_dir(SubDir* current_dir, string name);
File *find_file(SubDir* current_dir, string name);
SubDir *move_dir(SubDir* current_dir, string name);
vector<string> getUserInfo();
void printUserInfo(vector<string>);
SubDir* remove_dir(vector<string> args, SubDir *current_dir, bool isHome, vector<string> & path);
int remove_recursive(SubDir*);
int remove_one(SubDir* current_dir, string name);

int main()
{
    SubDir *home = initialize();
    SubDir *current_dir = home;
    vector<string> path = getUserInfo();
    bool running = 1;

    do {
        printUserInfo(path);

        string input;
        getline(cin, input);
        //cout << "Your command: " << input << endl;
        vector<string> args = parse_line(input);
        string command = args[0];

        //make new directory
        if(command.compare("mkdir")==0){
            if(args.size()>1){
                SubDir *dir = new SubDir(args[1],current_dir);
                current_dir->addChild(dir);
                if (dir) {cout << "Name of your new directory: " << dir->getName() << endl;} 
                else {cout << "Failed to create directory." << endl;}
            } else {
                cout << "Must enter a name." << endl;
            }
        }
        
        //show contents of current directory
        else if(command.compare("dir")==0){
            cout << "Current directory contains: " << endl;
            vector<SubDir*> children = current_dir->getChild();
            print(children);
        }

        //move directory
        else if(command.compare("cd")==0){
            if(args.size()>1){
                if(args[1].compare("..") == 0){
                    if(current_dir == home){
                        cout << "In home directory." << endl;
                    } else {
                        current_dir = current_dir->getParent();
                        path.pop_back();
                    }
                } else {
                    current_dir = move_dir(current_dir,args[1]);
                    if(current_dir->getName().compare(args[1])==0){
                        path.push_back(args[1]);
                    } else {
                        cout << "No such directory." << endl;
                    }
                }   
            } else {
                cout << "Must specify directory." << endl;
            }
        }

        //delete directory
        else if(command.compare("rm")==0 || command.compare("rmdir")==0){
            current_dir = remove_dir(args, current_dir, current_dir==home, path);
            
        }
        
        //end the program
        else if(command.compare("bye")==0){
            running = 0;
        }

        //unknown command
        else {
            cout << "Invalid command." << endl;
        }

    } while (running);

}

SubDir* initialize(){
    SubDir *home = new SubDir("home",NULL);
    return home;
}

vector<string> parse_line(string line){
    string space_delimiter = " ";
    vector<string> words;

    size_t pos = 0;
    while ((pos = line.find(space_delimiter)) != string::npos) {
        words.push_back(line.substr(0, pos));
        line.erase(0, pos + space_delimiter.length());
    }
    words.push_back(line);
    return words;

}

void print(vector<SubDir*> dir){
    vector<SubDir*>::iterator iter;
    for(iter = dir.begin();iter!=dir.end();++iter){
        cout << (*iter)->getName() << endl;
    }
}

void print(vector<File*> dir){
    vector<File*>::iterator iter;
    for(iter = dir.begin();iter!=dir.end();++iter){
        cout << (*iter)->getName() << endl;
    }
}


SubDir *find_dir(SubDir* current_dir,string name){
    vector<SubDir*> children = current_dir->getChild();
    vector<SubDir*>::iterator iter;
    for(int i = 0;i<children.size();++i){
        if(children[i]->getName()==name) {
            return children[i];
        }
    }
    return current_dir;
}

File *find_file(SubDir* current_dir,string name){
    vector<File*> files = current_dir->getFiles();
    vector<File*>::iterator iter;
    for(int i = 0;i<files.size();++i){
        if(files[i]->getName()==name) {
            return files[i];
        }
    }
    return nullptr;
}

SubDir *move_dir(SubDir* current_dir,string name){
    current_dir = find_dir(current_dir, name);
    return current_dir;
}

vector<string> getUserInfo(){
    vector<string> path;
    cout << "Enter your username: ";
    string name;
    getline(cin,name);
    cout << "Enter your machine's name: ";
    string machine_name;
    getline(cin,machine_name);
    path.push_back(name);
    path.push_back(machine_name);
    return path;
}

void printUserInfo(vector<string> path){
    vector <string>::iterator iter;
    cout << path[0] << "@" << path[1] << " ~";
    for(iter=path.begin() + 2; iter!=path.end(); ++iter){
        cout << "/" << *iter;
    }    
    cout << " % ";
}

SubDir* remove_dir(vector<string> args, SubDir * current_dir, bool isHome, vector<string> & path){
    //rm command
    if (args[0].compare("rm") == 0){
        if(!(args.size() > 1)){
            cout << "Please specify arguments." << endl;
            return current_dir; //no arguments specified, exit failure
        } 
        //-r switch (deletes everything in a directory, including subdirectories)
        else if(args[1].compare("-r") == 0 || args[1].compare("-R") == 0){
            cout << "Are you sure? This will delete all folders and files in this directory.\n" 
            << "Please confirm yes/no >> ";
            string confirm;
            getline(cin,confirm);
            if(confirm.compare("yes") == 0){
                if(!(args.size()>2)){
                    //no second argument -> remove current directory and contents
                    if(isHome){
                        cout << "Cannot delete home directory. " << endl;
                        return current_dir;
                    }
                    int exit = remove_recursive(current_dir);
                    string name = current_dir->getName();
                    current_dir = current_dir->getParent();
                    path.pop_back();
                    print(current_dir->getChild());
                    exit = remove_one(current_dir,name);
                    print(current_dir->getChild());
                    return current_dir;
                } else if(args[2].compare("*")==0){
                    //remove all in current directory
                    int exit = remove_recursive(current_dir);
                    return current_dir;
                } else {
                    //remove all in specified directory
                    SubDir *to_delete = find_dir(current_dir,args[2]);
                    if(to_delete==current_dir){
                        cout << "No such directory." << endl;
                        return current_dir;
                    } else {
                        int exit = remove_recursive(to_delete);
                        return current_dir;
                    }
                } 
            } else {
                //user typed "no" (or anything else that is not "yes")
                cout << "Execution stopped." << endl;
                return current_dir;
            }
        } 

    //rmdir command
    } else if (args[0].compare("rmdir")==0){
        if(!(args.size()>1)){
            cout << "Please specify arguments." << endl;
            return current_dir;
        } else {
            int exit = remove_one(current_dir, args[1]);
            return current_dir;
        }
    } else {
        cout << "Unexpected value." << endl;
        return current_dir;
    }
    return current_dir;
}

int remove_recursive(SubDir *dir){
    cout << "Removing all items from directory: " << dir->getName() << endl;
    if(dir->getSize()==0){
        return 0;
    }
    vector<SubDir*> children = dir->getChild();
    vector<SubDir*>::iterator iter;
    for(iter=children.begin();iter!=children.end();++iter){
        remove_recursive(*iter);
        (*iter)->~SubDir();
        dir->removeChild(*iter);
    }
    vector<File*> files = dir->getFiles();
    vector<File*>::iterator file_iter;
    for(file_iter=files.begin();file_iter!=files.end();++file_iter){
        (*file_iter)->~File();
        dir->removeFile(*file_iter);
    }
    cout << dir->getSize() << endl;
    return 0;
}

int remove_one(SubDir *current_dir, string name){
    //remove one empty directory
    vector<SubDir*> children = current_dir->getChild();
    vector<SubDir*>::iterator iter;
    SubDir* to_delete;
    for(iter = children.begin(); iter!=children.end(); ++iter){
        if((*iter)->getName()==name && (*iter)->getSize()==0){
            to_delete = *iter;
            current_dir->removeChild(to_delete);
            to_delete->~SubDir();
        }
    }
    if(!to_delete){
        cout << "Removed" << endl;
        return 0;
    }
    return 1;
}