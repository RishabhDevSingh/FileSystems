# filesystem

This is a C++ project I am currently working on. It emulates a filesystem. 

Users can create a hierarchical structure of dictionaries and move up and down between them with terminal commands.

Currently available commands are:
mkdir *dir* - creates a directory (in the current directory) with name *dir*

cd *dir* - moves to directory with name *dir*. Must be a subdirectory of the current directory.
cd .. - moves up a directory

dir - prints items in current directory

rmdir *dir* - deletes directory with name *dir* (located in current directory)
rm -r * - recursively deletes everything in current directory
rm -r - recursively deletes everything in current directory including current directory
rm -r *dir* - recursively deletes everything in directory with name *dir* (located in current directory)

bye - ends the program


Planned for the future: 
-creating files with text content and writing to/reading from them
-optional save current state of the filesystem to a file, optional read from file when starting the program
-GUI
-better code organization
