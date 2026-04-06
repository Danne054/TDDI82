#include <iostream>

int main(int argc, char* argv[]){
    int i{};
    while (std::cout << argv[i]){ std::cout << std::endl; ++i; }
            
    return 0;
}