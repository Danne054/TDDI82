#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <map>
#include <iomanip>
#include <exception>
#include <numeric>

// TODO: Komplettering: Använd inte '.' som avskiljare när ni bekräftar txt-filen. det är viktigt att koden är skalbar, så att man ex. kan använda ../files/short.txt som input.
//
// TODO: Komplettering: För många förekomster av for_each(), försöka at använda andra algoritmer.
//
// TODO: Komplettering: Bättre felhantering. Användaren måste förstå vilket argument det var som inte gick igenom.
//
// TODO: Komplettering: Utskriften av --table matchar inte det givna exemplet.
//
// TODO: Komplettering: Kolla över era utskrifter. hur lätt är det att läsa utskriften om användaren ex. skriver "--print --table --frequency"

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
    std::transform(text.begin(), text.end(), std::inserter(table, table.end()),
    [&table](std::string const& word) {
        return std::make_pair(word, ++table[word]);
    });

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
    std::cout << std::endl;
}

void table_func(std::vector<std::string> const& text){
    std::map<std::string, int> table { };
    size_t max_len { add_to_map(text, table) };

    // vi pratade med en assistent om att använda ostringstream 
    std::transform(table.begin(), table.end(), std::ostream_iterator<std::string>(std::cout, "\n"),
    [max_len](const auto& p) {
        std::ostringstream oss;
        oss << p.first
            << std::setw(max_len - p.first.size() + 2)
            << p.second;

        return oss.str();
    });
    std::cout << std::endl;
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

    //använder transform på samma sätt som på table
    std::transform(word_count.begin(), word_count.end(), std::ostream_iterator<std::string>(std::cout, "\n"),
    [max_len](const auto& p) {
        std::ostringstream oss;
        oss << std::setw(max_len) << p.first << ' ' << p.second;

        return oss.str();
    });
  
}

void substitute(std::vector<std::string> & text, std::string const& argument){
    std::string replace_with { find_operation(argument, true) };

    if (std::count(replace_with.begin(), replace_with.end(), '+') == 0){
        std::cout << "No '+' found in argument "<< argument <<", use: --substitute=<old>+<new>" << std::endl;
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
            std::cout << "No matching flag or operator found for " << argument << std::endl; 
        }
    });
}

std::vector<std::string> get_flags_operators(int argc, char* argv[]){
    std::vector<std::string> arguments(argc - 2);
    std::copy( &argv[2] , &argv[argc], arguments.begin() );

    return arguments;
}

bool is_a_file(int const argc, std::string const& first_arg){
    if (argc >= 2 && first_arg.size() >= 4){
        return  std::string { first_arg.end() - 4 , first_arg.end() } == ".txt";
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
        std::cout << argv[1] << " is not a valid file name or type" << std::endl; //fixa, kanske ska skrivas ut
    }

    return 0;
}