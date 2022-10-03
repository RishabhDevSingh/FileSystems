#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>

using namespace std;

class SubDir;
class File;

class Directory {
    protected:
        string name;
        SubDir *parent;
    public:
        virtual string getName() = 0;
        virtual void setName(string aName) = 0;
        virtual vector<SubDir*> getChild() = 0;
        virtual vector<File*> getFiles() = 0;
        virtual void addChild(SubDir * child) = 0;
        virtual void removeChild(SubDir * child) = 0;
        virtual void addFile(File * file) = 0;
        virtual void removeFile(File * file) = 0;
        virtual SubDir* getParent() = 0;
        virtual int getSize() = 0;
};

class SubDir: public Directory {
    protected:
        vector<SubDir*> children;
        vector<File*> files;
        int size;
    public:
        SubDir(string aName, SubDir *aParent = nullptr)
        {
            name = aName;
            parent = aParent;
            
        };
        
        //directory has to be empty!
        
        string getName() {return name;}
        void setName(string aName) {this->name = aName;}
        vector<SubDir*> getChild() {return children;}
        vector<File*> getFiles() {return files;}
        void addChild(SubDir * child) {
            children.push_back(child);
            size++;
        }
        void removeChild(SubDir* child){
            children.erase(remove(children.begin(), children.end(), child), children.end());
            size--;
        }
        void addFile(File * file) {
            files.push_back(file);
            size++;
            }
        void removeFile(File * file){
            files.erase(remove(files.begin(), files.end(), file), files.end());
            size--;
        }
        SubDir* getParent(){return parent;}
        int getSize(){return size;}
};

class File: public Directory {
    private: 
        string content;

    public:
        File(string aName, SubDir *aParent = nullptr)
        {
            name = aName;
            parent = aParent;
            
        };
    
        string getName() {return name;}
        void setName(string aName) {this->name = aName;}
        vector<SubDir*> getChild() {
            vector<SubDir*> v;
            return v;
            }
        vector<File*> getFiles(){
            vector<File*> v;
            return v;
        }
        void addChild(SubDir * child){name = name;}
        void addFile(File * file){name = name;}
        void removeChild(SubDir * child){name = name;}
        void removeFile(File * file){name = name;}
        SubDir* getParent(){return parent;}
        int getSize(){return 0;}

};