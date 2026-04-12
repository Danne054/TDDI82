#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <map>
#include <iomanip>

std::string find_operation(std::string const& argument, bool const end_part){
    std::string temp { argument };
    auto it = std::find(argument.begin(), argument.end(), '=');

    if (end_part){
        return std::string { it + 1, argument.end() };
    }

    return std::string { argument.begin() , it };
}

std::vector<std::string> get_file(std::string const& file_name){
    std::ifstream              ifs{};
    std::vector<std::string> words{};

    ifs.open(file_name);
    if (ifs.fail()){ std::cout << file_name << " not found" << std::endl; }
    
    std::copy(std::istream_iterator<std::string>{ ifs }, 
              std::istream_iterator<std::string>{ }, 
              std::back_inserter(words));

    ifs.close();
    return words;
}

void add_to_map(std::vector<std::string> const& text, std::map<std::string, int> & table, size_t & max_len){
    std::for_each(text.begin(), text.end(), [&table, &max_len](const std::string &word){
        ++table[word];
       
        if(word.size() > max_len ){ max_len = word.size(); }
    });
}

void print (std::vector<std::string> const& text){
    std::copy(text.begin(), text.end(),
    std::ostream_iterator<std::string>{ std::cout, " " });  
}

void table(std::vector<std::string> const& text){
    std::map<std::string, int> table { };
    size_t max_len { };
 
    add_to_map(text, table, max_len);

    std::for_each(table.begin(), table.end(), [max_len](std::pair<std::string, int> const a){ 
        std::cout << std::setw(max_len) << a.first << ' ' << a.second << std::endl;
    });
}

void frequency(std::vector<std::string> const& text){
    std::vector<std::pair<std::string, int> > word_count { }; 
    std::map<std::string, int> table { };
    size_t max_len { };

    add_to_map(text, table, max_len);

    copy(table.begin(), table.end(), back_inserter(word_count));
    sort(word_count.begin(), word_count.end(), 
        [](std::pair<std::string, int> const a, std::pair<std::string, int> const b){ 
        return a.second > b.second;
    });

    std::for_each(word_count.begin(), word_count.end(), [max_len](std::pair<std::string, int> const a){ 
        std::cout << std::setw(max_len) << a.first << ' ' << a.second << std::endl;
    });
}

void substitute(std::vector<std::string> & text, std::string const& argument){
    std::string replace_with { find_operation(argument, true) };

    auto it = std::find(replace_with.begin(), replace_with.end(), '+');

    std::string old_word { replace_with.begin(), it };
    std::string new_word { it + 1, replace_with.end() };

    std::replace(text.begin(), text.end(), old_word, new_word);
}

void remove(std::vector<std::string> & text, std::string const& argument){
    std::string remove_word { find_operation(argument, true) };

    text.erase( std::remove(text.begin(), text.end(), remove_word), text.end() );
}

void execute_flags_operators(std::vector<std::string> const& arguments, std::string const& file_name){
    std::vector<std::string> text { get_file (file_name) };
    
    std::for_each(arguments.begin(), arguments.end(), [&text](const std::string &argument) {
        if(argument == "--print"){
            print(text);
        }
        else if(argument == "--table"){
            table(text);
        }
        else if(argument == "--frequency"){
            frequency(text);
        }
        else if(find_operation(argument, false) == "--substitute" ){
            substitute(text, argument);
        }
        else if(find_operation(argument, false) == "--remove"){
            remove(text, argument);
        }

    });
}

std::vector<std::string> get_flags_operators(int argc, char* argv[]){
    std::vector<std::string> arguments(argc - 2);
    std::copy( &argv[2] , &argv[argc], arguments.begin() );

    return arguments;
}

bool is_a_file(int const argc, std::string const& first_arg){
    if (argc >= 1){
        return  std::string { std::find(first_arg.begin(), first_arg.end(), '.'), first_arg.end() } == ".txt";
    }

    return false;
}

int main(int argc, char* argv[]){
    std::vector<std::string> text      { };
    std::vector<std::string> arguments { };
    
    if (is_a_file(argc, argv[1])){
        arguments = get_flags_operators(argc, argv);
    }
    else{
        std::cout << "file name not valid or type" << std::endl; //fixa, kanske ska skrivas ut
        return 0;
    }

    execute_flags_operators (arguments , argv[1]);

    return 0;
}
    