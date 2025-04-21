#pragma once 

#include "./token.hpp"
#include <vector>


class Lexer 
{

private:
    std::string src;
    int i;
    char current_char;

    void advance()
    {
        if (this->current_char != '\0' || this->i < this->src.length())
        {
            this->i++;
            this->current_char = this->src[this->i];
        }
    }

    Token make_id() 
    {
        std::string ident = "";

        while (std::isalnum(this->current_char))
        {
            ident += this->current_char;
            this->advance();
        }

        if (ident == "exit")
        {
            return Token(TokenType::EXIT, ident);
        }

        if (ident == "print") 
        {
            return Token(TokenType::PRINT, ident);
        }

        if (ident == "var" || ident == "let")
        {
            return Token (TokenType::VAR, ident);
        }

        return Token(TokenType::ID, ident);

    }

    Token make_string() 
    {
        std::string str = "";

        this->advance();

        while (this->current_char != '"') {
            str += this->current_char;
            this->advance();
        }

        this->advance();

        return Token(TokenType::STRING, str);
    }

    Token make_number() 
    {
        std::string num = "";

        while (std::isdigit(this->current_char) || this->current_char == '.')
        {
            num += this->current_char;
            this->advance();
        }

        int dot_count = 0;
        for (char c : num)
        {
            if (c == '.')
            {
                dot_count++;
            }
        }

        if (dot_count > 1) 
        {
            std::cerr << "Too many dots in a number: " << num << "\n";
            exit(1);
        }

        if (dot_count == 0)
        {   
            return Token(TokenType::INT, num);
        }

        return Token(TokenType::FLOAT, num);
    }

public:
    Lexer(const std::string& src) 
    {
        this->src = src;
        this->i = 0;
        this->current_char = this->src[this->i];
    }

    std::vector<Token> get_tokens() 
    {
        std::vector<Token> tokens;

        while (this->current_char != '\0' || this->i < this->src.length())
        {
            while (this->current_char == ' ' || this->current_char == '\n' || this->current_char == '\t')
            {
                this->advance();
            }

            if (std::isalpha(this->current_char))
            {
                tokens.push_back(this->make_id());
                continue;
            }

            if (std::isdigit(this->current_char)) 
            {
                tokens.push_back(this->make_number());
                continue;
            }

            if (this->current_char == '"')
            {
                tokens.push_back(this->make_string());
                continue;
            }


            switch (this->current_char) 
            {
                case ';': 
                    tokens.push_back(Token(TokenType::SEMI, ";"));
                    break;

                case '(':
                    tokens.push_back(Token(TokenType::L_PAREN, "("));
                    break;

                case ')':
                    tokens.push_back(Token(TokenType::R_PAREN, ")"));
                    break;

                case '=':
                    tokens.push_back(Token(TokenType::EQ, "="));
                    break;
                
                default:
                    std::cerr << "Unknown character: '" << this->current_char << "'\n";
                    exit(1);
            }

            this->advance();
            
        }

        tokens.push_back(Token(TokenType::END, ""));

        return tokens;
    }

};