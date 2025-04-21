#include <iostream>
#include <fstream>

#include <cstdlib>
#include <sstream>

#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/generator.hpp"

void usage()
{

    std::cerr << "Incorrect usage...\n";
    std::cerr << "correct usage is: bns <file.banana>\n";
    exit(1);

}


int main(int argc, char** argv) 
{

    if (argc < 2) 
    {
        usage();
    }

    std::string output_name;
    if (argc == 3) 
    {
        output_name = argv[2];
    }
    else 
    {
        output_name = "output.asm";
    }

    std::stringstream buffer;
    {
        std::ifstream t(argv[1]);

        if (!t.is_open()) 
        {
            std::cerr << "Unable to open " << argv[1] << "\n";
            return 1;
        }

        buffer << t.rdbuf();
        t.close();
    }

    Lexer lexer(buffer.str());
    std::vector<Token> tokens = lexer.get_tokens();

    Parser parser(tokens);
    std::vector<Stmt*> stmts = parser.generate_ast();

    Generator generator(stmts);
    std::string file_content = generator.generate();

    {
        std::ofstream t("output.asm");

        if (!t.is_open()) 
        {
            std::cerr << "Unable to open output.asm\n";
            return 1;
        }

        t << file_content; 
        t.close();
    }

    std::system("nasm -f elf64 output.asm -o output.o");
    std::string link_command = "ld output.o -o " + output_name;
    std::system(link_command.c_str());
    std::system("rm -rf output.asm");
    std::system("rm -rf output.o"); 

    return 0;

}