#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include "./nodes.hpp"


class Generator {

private:
    std::vector<Stmt*> stmts;

    std::stringstream file;

    std::stringstream data_section;
    std::stringstream start_section;
    int num_strs;
    
public:
    

    Generator(const std::vector<Stmt*>& stmts) : stmts(stmts) 
        { this->num_strs = 0;}

    std::string generate() 
    {

        this->start_section << "\n\nsection .text\n    global _start\n_start:\n";
        this->data_section << "section .data\n";

        for (auto* stmt : this->stmts) 
        { 
            if (auto* exitstmt = dynamic_cast<ExitStmt*>(stmt)) 
            {
                this->generate_exit(exitstmt);
            }

            if (auto* printstmt = dynamic_cast<PrintStmt*>(stmt))
            {
                this->generate_print(printstmt);
            }

            if (auto* vardefstmt = dynamic_cast<VarDefStmt*>(stmt)) 
            {
                this->generate_var_def(vardefstmt);
            }
        }

        this->file << data_section.str();

        this->file << start_section.str();

        return this->file.str();

    }

    void generate_print(PrintStmt* printstmt) 
    {
        if (auto* expr = dynamic_cast<StringExpr*>(printstmt->expr)) 
        {
            this->num_strs++;

            this->data_section << "    str_" << std::to_string(this->num_strs) << " db \"" << expr->value << "\", 10\n";
            this->data_section << "    str_" << std::to_string(this->num_strs) << "_len equ $ - str_" << std::to_string(this->num_strs) << "\n";

            this->start_section << "    mov rax, 1\n";
            this->start_section << "    mov rdi, 1\n";
            this->start_section << "    mov rsi, str_" << std::to_string(this->num_strs) << "\n";
            this->start_section << "    mov rdx, str_" << std::to_string(this->num_strs) << "_len\n";
            this->start_section << "    syscall\n\n";
        }

        if (auto* expr = dynamic_cast<NumberExpr*>(printstmt->expr)) 
        {
            this->num_strs++;

            
            std::visit([&](auto&& val) {
            
                this->data_section << "    str_" << std::to_string(this->num_strs) << " db \"" << std::to_string(val) << "\", 10\n";
            
            }, expr->value);
            
            this->data_section << "    str_" << std::to_string(this->num_strs) << "_len equ $ - str_" << std::to_string(this->num_strs) << "\n";

            this->start_section << "    mov rax, 1\n";
            this->start_section << "    mov rdi, 1\n";
            this->start_section << "    mov rsi, str_" << std::to_string(this->num_strs) << "\n";
            this->start_section << "    mov rdx, str_" << std::to_string(this->num_strs) << "_len\n";
            this->start_section << "    syscall\n\n";
        }

        if (auto* expr = dynamic_cast<VarAccessExpr*>(printstmt->expr))
        {
            this->start_section << "    mov rax, 1\n";
            this->start_section << "    mov rdi, 1\n";
            this->start_section << "    mov rsi, " << expr->name << "\n";
            this->start_section << "    mov rdx, " << expr->name << "_len\n";
            this->start_section << "    syscall\n\n";
        }

    }

    void generate_var_def(VarDefStmt* vardefstmt) 
    {
        if (auto* expr = dynamic_cast<StringExpr*>(vardefstmt->var_value))
        {
            this->data_section << "    " << vardefstmt->var_name << " db \"" << expr->value << "\", 10\n";
            this->data_section << "    " << vardefstmt->var_name << "_len equ $ - " << vardefstmt->var_name << "\n";
        }

        if (auto* expr = dynamic_cast<NumberExpr*>(vardefstmt->var_value)) 
        {
            
            std::visit([&](auto&& val) {

                this->data_section << "    " << vardefstmt->var_name << " dw " << std::to_string(val) << "\n";
                
            }, expr->value);
            
            
        }
    }

    void generate_exit(ExitStmt* exitstmt) 
    {
        
        if (auto* expr = dynamic_cast<NumberExpr*>(exitstmt->expr)) 
        {  
            this->start_section << "    mov rax, 60\n";
            std::visit([&](auto&& val) {
                this->start_section << "    mov rdi, " << std::to_string(val) << "\n";
            }, expr->value);
            
            this->start_section << "    syscall\n\n";
        }
        else 
        {
            std::cerr << "Unknown expression\n";
            exit(1);
        }

    }

};