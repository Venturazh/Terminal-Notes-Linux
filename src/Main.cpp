#include <fstream>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <string>


void CreateFolder(){

    const char* home = std::getenv("HOME");
    if (!home) return;

    std::filesystem::path dir = std::filesystem::path(home) / "SavedNotes";

    if(std::filesystem::create_directory(dir)){
        std::cout << "Directory: " << dir << " Has Been Created" << std::endl;
    }else{
        std::cout << "Directory " << dir << " already exists or could not be created." << std::endl;
    }
}

void HideKeyStrokes(){
    termios attr;

    tcgetattr(STDIN_FILENO, &attr);

    attr.c_lflag &= ~ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}

void ShowKeyStrokes(){
    termios attr;

    tcgetattr(STDIN_FILENO, &attr);

    attr.c_lflag |= ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}

int DisplayOptions(){
    std::cout << "1)New File" << "\n"
              << "2)Open File" << "\n"
              << "3)Display Files" << std::endl;

    HideKeyStrokes();
    
    int Input;
    std::cin >> Input;

    switch(Input){
        case 1:
            std::cout << "Choose New File" << std::endl;
            break;
        case 2:
            std::cout << "Choose Open File" << std::endl;
            break;
        case 3:
            std::cout << "Choose Display Files" << std::endl;
            break;
        default:
            std::cout << "Not A Option" << std::endl;
            break;

    }

    ShowKeyStrokes();
    return Input;
}


int main(){

    CreateFolder();

    DisplayOptions();

    return 0;
}