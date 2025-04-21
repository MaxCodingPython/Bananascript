#pragma once 
#include <string>


enum class TokenType 
{

    ID,
    PRINT,
    EXIT,
    VAR,

    INT,
    FLOAT,
    STRING,
    
    EQ,
    SEMI,
    L_PAREN,
    R_PAREN,

    END,

};


class Token 
{

public:
    TokenType type;
    std::string value;

    Token() : type(TokenType::END), value("") {}

    Token(TokenType type, std::string value) : type(type), value(value) {}

};