#include <iostream>
#include <algorithm>
#include <vector>
//#include <array>
//#include <list>
#include <fstream>
#include <string>
#include <iterator>
#include <map>
#include <set>

// std::for_each(arguments.begin(), arguments.end(), [&v](const std::string &argument) {  
//     auto it = std::find_if(argument.begin(), argument.end(), [](char a) { return a == '='; });
//     v.push_back( std::string{it + 1 , argument.end()} );
// });

// class Parameter{
// public:
//     Parameter(std::vector<std::string> const& parameter, std::vector<std::string> const& text)
//     :parameter{parameter}, text{text} {}

// private:
//     std::vector<std::string> parameter { };
//     std::vector<std::string> text{};
// };

// class Flag{
// public:
//     Flag(std::vector<std::string> const& flag, std::vector<std::string> const& text)
//     :flag{flag}, text{text} {}

//     void print() const{
//         std::copy(text.begin(), text.end(),
//         std::ostream_iterator<std::string>{ std::cout, "\n" });
//     }

//     void frequency() const{

//     }

// private:
//     std::vector<std::string> flag { };
//     std::vector<std::string> text{};
// };
std::string find_operation(std::string const& argument){
    auto it = std::find(argument.begin(), argument.end(), '=');
    return std::string {argument.begin() , it};;
}

std::vector<std::string> get_file(std::string const& file_name){
    std::ifstream ifs{};
    ifs.open(file_name);
    std::vector<std::string> words{};

    std::copy(std::istream_iterator<std::string>{ ifs }, 
              std::istream_iterator<std::string>{ }, 
              std::back_inserter(words));

    return words;
}

void print (std::vector<std::string> text){
    std::copy(text.begin(), text.end(),
    std::ostream_iterator<std::string>{ std::cout, " " });  
}

void frequency(std::vector<std::string> text){
    std::map<std::string, int> table{};
    std::for_each(text.begin(), text.end(), [&table](const std::string &word){
        ++table[word];
    });
    std::set<int> set_of_ints{};

    std::for_each(text.begin(), text.end(), [&table, &set_of_ints](const std::string &word){
        set_of_ints.insert(table[word]);
    });

    int n {set_of_ints.max()};
    std::for_each(text.begin(), text.end(), [&table, &set_of_ints, &n](const std::string &word){
        std::cout << std::endl;
        --n;
    });

    std::copy(text.begin(), text.end(),
    std::ostream_iterator<std::string>{ std::cout, " " });

}

void execute_flags(std::vector<std::string> const& arguments,
                   std::string const& file_name){
    std::vector<std::string> text  { get_file (file_name) };
    
    std::for_each(arguments.begin(), arguments.end(), [&text](const std::string &argument) {  
        if(argument == "--print"){
            print(text);
        }
        else if(argument == "--table"){
            //table();
        }
        else if(argument == "--frequency"){
            frequency(text);
        }
        else if(find_operation(argument) == "--substitute=" ){
            
        }
        else if(find_operation(argument) == "--remove="){

        }
    });
}

// std::map<std::string, bool> read_flags(std::vector<std::string> const& arguments){
//     std::vector<std::string> parameter_list { };
//     std::vector<std::string> flag_list      { };

//     std::map<std::string, bool> temp{ read_each(arguments) }; 
    
//     return temp;
//     // std::copy(parameter.begin(), parameter.end(),
//     // std::ostream_iterator<std::string>{ std::cout, "\n" });  
// }

std::vector<std::string> get_flags(int argc, char* argv[]){
    std::vector<std::string> arguments(argc - 2);
    std::copy( &argv[2] , &argv[argc], arguments.begin() );

    return arguments;
}



bool is_a_file(int const argc, std::string const& first_arg){
    if (argc >= 1){
        auto it = std::find_if(first_arg.begin(), first_arg.end(), [](char a) { return a == '.'; });

        std::string test_end{};
        std::copy(it, first_arg.end(), std::back_inserter (test_end) );

        std::string new_test{};
        std::copy(first_arg.begin(), it, std::back_inserter (new_test) );
        
        if(test_end == ".txt"){
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[]){
    std::vector<std::string> text      {};
    std::vector<std::string> arguments {};

    if (is_a_file(argc, argv[1])){
        arguments = get_flags(argc, argv);
        //text      = get_file (argv[1]);
    }
    else{
        throw std::logic_error("Not a txt file");
    }

    //std::map<std::string, bool> operators_flags { read_flags (arguments) };
    execute_flags (arguments , argv[1]);

    return 0;
}
    