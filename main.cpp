#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
    std::cout<<"Program for calculating surface runoff and underground flow\n";
    std::cout<<"made by Mashenkin Pavel\n";

    std::fstream fin("task.txt");
    
    if (!fin){
         std::cerr<<"file task.txt not found!\n";
         return 1;
    }

    //тут очень длинный комментарий для того что бы проверить функцию пейджера у git div, далее следует понять как грамотно зачитывать команьды из файла и воспользоваться ли какйо нибудь библиотекой например JSON или YAML

    return 0;
}