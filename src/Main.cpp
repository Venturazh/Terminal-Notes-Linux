#include <fstream>
#include <iostream>
#include <ostream>
#include <system_error>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <string>

void ShowKeyStrokes();

void CreateFolder(){

    const char* home = std::getenv("HOME");
    if (!home) return;

    std::filesystem::path dir = std::filesystem::path(home) / "SavedNotes";

    if(std::filesystem::create_directories(dir)){
        std::cout << "Directory: " << dir << " Has Been Created" << std::endl;
    }else{
        std::cout << "Directory " << dir << " already exists or could not be created." << std::endl;
    }
}

void DisplayNotes(){
    const char* home = std::getenv("HOME");

    std::error_code err;

    std::filesystem::path dir = std::filesystem::path(home) / "SavedNotes";

    for(const auto& entry : std::filesystem::directory_iterator(dir, err)){
        std::cout << entry.path().filename().string() << std::endl;
    }
}

void OpenFile(){
    const char* home = std::getenv("HOME");

    std::filesystem::path dir = std::filesystem::path(home) / "SavedNotes";
    std::error_code err;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    DisplayNotes();

    ShowKeyStrokes();
    std::string FileName;
    std::cout << "Please enter a name for the file you want to open: ";
    std::getline(std::cin, FileName);

    std::filesystem::path Note = dir / FileName;

    std::cout << Note;

    std::string command;

    try{
        command = "nvim \"" + Note.string() + "\"";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return;
    }
    std::system(command.c_str());
}

void CreateNewFile(){
    const char* home = std::getenv("HOME");

    std::error_code err;

    std::filesystem::path dir = std::filesystem::path(home) / "SavedNotes";

    ShowKeyStrokes();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string FileName;
    std::cout << "Please enter a name for the file: ";
    std::getline(std::cin, FileName);

    for(const auto& entry : std::filesystem::directory_iterator(dir, err)){
       if(entry.path().filename().string() == FileName){
            std::cout << "File Already Exist" << std::endl;
            return;
       }
    }

    std::filesystem::path Note = dir / FileName;
    std::ofstream CreatedNote(Note);

    std::string command;

    try{
        command = "nvim \"" + Note.string() + "\"";
    }
    catch(const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return;
    }
    std::system(command.c_str());
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
    std::cout << "Chosen Option: ";
    std::cin >> Input;

    switch(Input){
        case 1:
            std::cout << "Choosen New File" << std::endl;
            CreateNewFile();
            break;
        case 2:
            std::cout << "Choosen Open File" << std::endl;
            OpenFile();
            break;
        case 3:
            std::cout << "Choosen Display Files" << std::endl;
            DisplayNotes();
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