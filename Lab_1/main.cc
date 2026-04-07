#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <map>
#include <set>
#include <iomanip>
#include <utility>
#include <sstream>

// std::for_each(arguments.begin(), arguments.end(), [&v](const std::string &argument) {  
//     auto it = std::find_if(argument.begin(), argument.end(), [](char a) { return a == '='; });
//     v.push_back( std::string{it + 1 , argument.end()} );
// });

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
    std::map<std::string, int> table{ };
    std::vector<std::pair<std::string, int> > word_count; 
    size_t max_len{ };

    std::for_each(text.begin(), text.end(), [&table, &max_len](const std::string &word){
       ++table[word];

        if(word.size() > max_len ){ 
            max_len = word.size();
        }
    });

    copy(table.begin(), table.end(), back_inserter(word_count));
    sort(word_count.begin(), word_count.end(), [](auto const a, auto const b){ 
        return a.second > b.second;
    });

    std::for_each(word_count.begin(), word_count.end(), [max_len](auto const a){ 
        std::cout << std::setw(max_len) << a.first << ' ' << a.second << std::endl;
    });
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
    }
    else{
        throw std::logic_error("Not a txt file"); //fixa, kanske ska skrivas ut
    }

    execute_flags (arguments , argv[1]);

    return 0;
}
    