#pragma once

#include <variant>
#include <string>


class Expr {
public:
    virtual ~Expr() = default;
};


class NumberExpr : public Expr {
public:
    std::variant<int, float> value;
    NumberExpr(std::variant<int, float> value): value(value) {}
};

class StringExpr : public Expr {
public:
    std::string value;
    StringExpr(std::string value) : value(value) {}
};

class VarAccessExpr : public Expr {
public:
    std::string name;
    VarAccessExpr(std::string name) : name(name) {}
};

class Stmt {
public:
    virtual ~Stmt() = default;
};

class PrintStmt : public Stmt {
public:
    Expr* expr;
    PrintStmt(Expr* expr) : expr(expr) {}
};

class ExitStmt : public Stmt {
public:
    Expr* expr;
    ExitStmt(Expr* expr) : expr(expr) {}
};

class VarDefStmt : public Stmt {
public:
    std::string var_name;
    Expr* var_value;
    VarDefStmt(std::string name, Expr* value)  : var_value(value), var_name(name) {}
};