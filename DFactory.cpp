#include <iostream>
#include "objects.cpp"
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>

using namespace std;


class DFactory {
    public:
        static Directory * createDirectory(int type, string name, Directory *parent){
            //Directory *dir = NULL;

            switch(type) {
                case 1:{
                    SubDir *dir = new SubDir(name,parent);
                    return dir;
                }
                case 2:{
                    File *dir = new File(name,parent);
                    return dir;
                }
                default:{
                    cout << "Invalid type" << endl;
                    return NULL;
                }
            }
            //return dir;

        }
};

