#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <map>
#include <iomanip>
#include <exception>
#include <filesystem>
#include <numeric>

std::string find_operation(std::string const& argument, bool const end_part){
    std::string temp { argument };
    auto it = std::find(argument.begin(), argument.end(), '=');

    if (end_part){
        return std::string { it + 1, argument.end() };
    }

    return std::string { argument.begin() , it };
}

std::vector<std::string> get_file(std::string const& file_name){
    std::ifstream              ifs{ };
    std::vector<std::string> words{ };

    ifs.open(file_name);
    if (ifs.fail()){ std::cout << file_name << " not found" << std::endl; }
    
    std::copy(std::istream_iterator<std::string>{ ifs }, 
              std::istream_iterator<std::string>{ }, 
              std::back_inserter(words));

    ifs.close();
    return words;
}

size_t add_to_map(std::vector<std::string> const& text, std::map<std::string, int> & table){
    //std::for_each(text.begin(), text.end(), [&table](const std::string &word){
     //     ++table[word];
    //});
    //hittade accumulate som är till för att sumera värden "vika värden" och hitta ett exempel på ett forum då de gjorde en map
    //std::ref(table) är till för att den inte ska kopieras varje varv
    std::accumulate(text.begin(), text.end(), std::ref(table),
    [](auto& tbl, const std::string& word) -> auto& {
        ++tbl.get()[word];
        return tbl;
    });

    std::string word_max_len{ };

    word_max_len = *std::max_element(text.begin(), text.end(),[] (std::string const a, std::string const b){
        return a.size() < b.size();
    });

    return word_max_len.size();
}

void print (std::vector<std::string> const& text){
    std::copy(text.begin(), text.end(),
    std::ostream_iterator<std::string>{ std::cout, " " });  
}

void table_func(std::vector<std::string> const& text){
    std::map<std::string, int> table { };
    size_t max_len { add_to_map(text, table) };
    
   // std::for_each(table.begin(), table.end(), [max_len](std::pair<std::string, int> const a){ 
    //   std::cout << a.first << std::setw(max_len - a.first.size() + 2) << a.second << std::endl;
    //});

    // vi pratade med en assistent om att använda ostringstream 
    std::transform(table.begin(), table.end(),
    std::ostream_iterator<std::string>(std::cout, "\n"),
    [max_len](const auto& p) {
        std::ostringstream oss;
        oss << p.first
            << std::setw(max_len - p.first.size() + 2)
            << p.second;
        return oss.str();
    });
}

void frequency(std::vector<std::string> const& text){
    std::vector<std::pair<std::string, int> > word_count { }; 
    std::map<std::string, int> table { };
    size_t max_len { add_to_map(text, table) };

    copy(table.begin(), table.end(), back_inserter(word_count));
    sort(word_count.begin(), word_count.end(), 
        [](std::pair<std::string, int> const a, std::pair<std::string, int> const b){ 
        return a.second > b.second;
    });

   // std::for_each(word_count.begin(), word_count.end(), [max_len](std::pair<std::string, int> const a){ 
     //   std::cout << std::setw(max_len) << a.first << ' ' << a.second << std::endl;
    //});
    //använder transform på samma sätt som på table
    std::transform(word_count.begin(), word_count.end(),
    std::ostream_iterator<std::string>(std::cout, "\n"),
    [max_len](const auto& p) {
        std::ostringstream oss;
        oss << std::setw(max_len) << p.first << ' ' << p.second;
        return oss.str();
    });
}

void substitute(std::vector<std::string> & text, std::string const& argument){
    std::string replace_with { find_operation(argument, true) };

    if (std::count(replace_with.begin(), replace_with.end(), '+') == 0){
        std::cout << "no '+' found in substitute argument, use: --substitute=<old>+<new>" << std::endl;
        
    }
    else{
        auto it = std::find(replace_with.begin(), replace_with.end(), '+');

        std::string old_word { replace_with.begin(), it };
        std::string new_word { it + 1, replace_with.end() };

        std::replace(text.begin(), text.end(), old_word, new_word);
    }
}

void remove(std::vector<std::string> & text, std::string const& argument){
    std::string remove_word { find_operation(argument, true) };

    text.erase( std::remove(text.begin(), text.end(), remove_word), text.end() );
}

void execute_flags_operators(std::vector<std::string> const& arguments, std::vector<std::string> & file_text){
    std::for_each(arguments.begin(), arguments.end(), [&file_text](const std::string &argument) {
        if(argument == "--print"){
            print(file_text);
        }
        else if(argument == "--table"){
            table_func(file_text);
        }
        else if(argument == "--frequency"){
            frequency(file_text);
        }
        else if(find_operation(argument, false) == "--substitute" ){
            substitute(file_text, argument);
        }
        else if(find_operation(argument, false) == "--remove"){
            remove(file_text, argument);
        }
        else{ 
            std::cout << "No matching flag or operator found" << std::endl; 
        }
    });
}

std::vector<std::string> get_flags_operators(int argc, char* argv[]){
    std::vector<std::string> arguments(argc - 2);
    std::copy( &argv[2] , &argv[argc], arguments.begin() );

    return arguments;
}

bool is_a_file(int const argc, std::string const& first_arg){
    //har ändrat så vi använder Filsystem så den kollar fört längst nere på trädet sedan efter .txt
    if (argc >= 2){ // ändrade till två istället för ett för tror den också gav fel
        return std::filesystem::path(first_arg).extension() == ".txt";
    }

    return false;
}

int main(int argc, char* argv[]){
    std::vector<std::string> arguments { };
    std::vector<std::string> file_text { };

    if (is_a_file(argc, argv[1])){
        file_text = get_file (argv[1]);
        arguments = get_flags_operators(argc, argv);

        execute_flags_operators (arguments, file_text);
    }
    else{
        std::cout << "file name not valid or type" << std::endl; //fixa, kanske ska skrivas ut
    }

    return 0;
}