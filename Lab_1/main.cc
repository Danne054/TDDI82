#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <list>
#include <fstream>
#include <string>

void read_flags(int argc, char* argv[]){

    
    std::vector<std::string> vector_args(argc - 2);
    std::copy( &argv[2] , &argv[argc], vector_args.begin() );
    
    int i{};
    while(i < argc - 2 && std::cout << vector_args.at(i) ){ std::cout << std::endl; i++; }
}

int main(int argc, char* argv[]){
    std::string first_arg{ argv[1] };
    std::ifstream ifs;

    if (argc >= 1){
        auto it = std::find_if(first_arg.begin(), first_arg.end(), [](char a) { return a == '.'; });

        std::string test_end{};
        std::copy(it, first_arg.end(), std::back_inserter (test_end) );

        std::string new_test{};
        std::copy(first_arg.begin(), it, std::back_inserter (new_test) );
        
        if(test_end != ".txt"){
            throw std::logic_error("Not a txt file");
        }
        else{
            //ifs.open(argv[1]);
            read_flags(argc, argv);
        }

        // std::cout << test_end << '\n' << new_test <<std::endl;

    }

    return 0;
}
    