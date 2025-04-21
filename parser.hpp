#pragma once 

#include <vector>
#include <iostream>

#include "./token.hpp"
#include "./nodes.hpp"


class Parser {
private:
    int i;
    std::vector<Token> tokens;
    Token current_token;

    ExitStmt* parse_exit() 
    {
        this->next_token();
        
        this->except_token(TokenType::L_PAREN);

        Expr* expr = this->parse_expr();
        this->next_token();

        this->except_token(TokenType::R_PAREN);

        this->except_token(TokenType::SEMI);

        return new ExitStmt(expr);
    }

    VarDefStmt* parse_var_def() 
    {
        this->next_token();

        this->except_token(TokenType::ID);
        std::string var_name = this->prev_token().value;
        
        this->except_token(TokenType::EQ);
        
        Expr* expr = this->parse_expr();
        this->next_token();

        this->except_token(TokenType::SEMI);

        return new VarDefStmt(var_name, expr);
    }

    PrintStmt* parse_print()
    {
        this->next_token();

        this->except_token(TokenType::L_PAREN);

        Expr* expr = this->parse_expr();
        this->next_token();

        this->except_token(TokenType::R_PAREN);

        this->except_token(TokenType::SEMI);

        return new PrintStmt(expr);
    }

    Token prev_token() {
        return this->tokens[this->i - 1];
    }

    Expr* parse_expr() 
    {
        switch (this->current_token.type) 
        {
            case TokenType::INT:
                return this->parse_int();

            case TokenType::FLOAT:
                return this->parse_float();

            case TokenType::STRING:
                return this->parse_string();

            case TokenType::ID:
                return this->parse_variable();

            default:
                std::cerr << "Unexcepted expr: " << this->current_token.value << "\n";
                exit(1);
        }
    }

    NumberExpr* parse_int() 
    {
        return new NumberExpr(std::stoi(this->current_token.value));
    }

    StringExpr* parse_string()
    {
        return new StringExpr(this->current_token.value);
    }

    VarAccessExpr* parse_variable() 
    {
        return new VarAccessExpr(this->current_token.value);
    }

    NumberExpr* parse_float() 
    {
        return new NumberExpr(std::stof(this->current_token.value));
    }

    void except_token(TokenType type) 
    {
        if (this->current_token.type == type)  
        {   
            this->next_token();
        } 
        else
        {
            std::cerr << "Unexcepted token: " << this->current_token.value << "\n";
            exit(1);
        }
    }

    void next_token() 
    {
        if (this->current_token.type != TokenType::END)
        {
            this->i++;
            this->current_token = this->tokens[this->i];
        }
    }

public:
    Parser(const std::vector<Token>& tokens) 
    {
        this->tokens = tokens;
        this->i = 0;
        this->current_token = tokens[0];
    }

    std::vector<Stmt*> generate_ast() 
    {
        std::vector<Stmt*> stmts;

        while (this->current_token.type != TokenType::END)
        {
            switch (this->current_token.type) 
            {
                case TokenType::EXIT:
                    stmts.push_back(this->parse_exit());
                    break;

                case TokenType::PRINT:
                    stmts.push_back(this->parse_print());
                    break;

                case TokenType::VAR:
                    stmts.push_back(this->parse_var_def());
                    break;
            }
        }

        return stmts;
    }
};