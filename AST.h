#ifndef AST_H
#define AST_H

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include "Env.h"

//
// Abstract base class for all expressions.
//
class Expr {
public:
  virtual ~Expr() {}
  virtual float eval(Env& env) const = 0;
};

//
// Abstract base class for all statements.
//
class Stmt {
public:
  virtual ~Stmt() {};
  virtual void execute(Env& env) = 0;
};

//
// XXXX
// AST's expressions and statements go here.
//

#endif // AST_H
