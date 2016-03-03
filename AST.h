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


class WhileStmt : public Stmt {
protected:
	Expr *cond_;
	Stmt *body_;
public:
	WhileStmt(Expr *c, Stmt *b) : cond_{c}, body_{b} {}
	void execute(Env& env) 
	{
	while (cond_->eval(env) != 0)
		body_->execute(env);
	}
	~WhileStmt() {delete cond_; delete body_;}
};	

// list instead of vector, mayhaps?
// ?
class BlockStmt : public Stmt {
protected:
	std::vector<Stmt*> _s;
public:
	BlockStmt(const std::vector<Stmt*>& s) : _s{s} {} // Is this right?
	virtual void execute(Env& env)
	{
		for(auto i : _s)
			i->execute(env);
	}
};

// ?
class IfStmt : public Stmt {
protected:
	Expr *cond_;
	Stmt *body_;
	Stmt *else_part_;
public:
	IfStmt(Expr *c, Stmt *b, Stmt *e) : cond_{c}, body_{b}, else_part_{e} {}
	virtual void execute(Env& env)
	{
		if(cond_->eval(env) != 0)
			body_->execute(env);
		else if(else_part_ != nullptr)
			else_part_->execute(env);
	}
	~IfStmt() {delete cond_; delete body_; delete else_part_;}
};

class AssignStmt : public Stmt {
protected:
  	const std::string _name;  //l-value
  	Expr *_expr; // r-value
public:
  	AssignStmt(const std::string& n, Expr *e) : _name{n}, _expr{e} {}
  	virtual void execute(Env& env) 
	{
    	env.put(_name, _expr->eval(env));
  	}
	~AssignStmt() {delete _expr;}
};


class HomeStmt : public Stmt {
public:
  	virtual void execute(Env& env) 
	{
    	std::cout << "H" << std::endl;
  	}
};

class PenUpStmt : public Stmt {
public:
  	virtual void execute(Env& env) 
	{
    	std::cout << "U" << std::endl;
  	}
};

class PenDownStmt : public Stmt {
public:
  	virtual void execute(Env& env) 
	{
    	std::cout << "D" << std::endl;
  	}
};

class PushStateStmt : public Stmt {
public:
  	virtual void execute(Env& env) 
	{
    	std::cout << "[" << std::endl;
  	}
};

class PopStateStmt : public Stmt {
public:
  	virtual void execute(Env& env) 
	{
    	std::cout << "]" << std::endl;
  	}
};

class ForwardStmt : public Stmt {
protected:
  	Expr *_dist;
public:
  	ForwardStmt(Expr *e) : _dist{e} {}
  	virtual void execute(Env& env) 
	{
    	const float d = _dist->eval(env);
    	std::cout << "M " << d << std::endl;
  	}
};

class RightStmt : public Stmt {
protected:
  	Expr *_angle;
public:
  	RightStmt(Expr *e) : _angle{e} {}
  	virtual void execute(Env& env) 
	{
	    const float a = _angle->eval(env);
	    std::cout << "R " << -a << std::endl;
  	}
};

class LeftStmt : public Stmt {
protected:
  Expr *_angle;
public:
  	LeftStmt(Expr *e) : _angle{e} {}
  	virtual void execute(Env& env) 
	{
    	const float a = _angle->eval(env);
    	std::cout << "L " << a << std::endl;
  	}
};

class VarExpr : public Expr {
protected:
  const std::string _name;
public:
  	VarExpr(const std::string& n) : _name{n} {}
  	virtual float eval(Env& env) const 
	{
    	return env.get(_name);
  	}
};

class ConstExpr : public Expr {
protected:
  const float _val;
public:
  	ConstExpr(float v) : _val{v} {}
  	virtual float eval(Env& env) const 
	{
    	return _val;
  	}
};

class UnaryExpr : public Expr {
protected:
  	Expr *_expr;
public:
  	UnaryExpr(Expr *e) : _expr{e} {}
};

class NegExpr : public UnaryExpr {
public:
  	NegExpr(Expr *e) : UnaryExpr(e) {}
  	virtual float eval(Env& env) const 
	{
    	return -_expr->eval(env); 
  	}
};

class BinaryExpr : public Expr {
protected:
  	Expr *_left, *_right;
public:
  	BinaryExpr(Expr *l, Expr *r) : _left{l}, _right{r} {}
};

class AddExpr : public BinaryExpr {
public:
  	AddExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
  	virtual float eval(Env& env) const 
	{
    	return _left->eval(env) + _right->eval(env); 
  	}
};

class SubExpr : public BinaryExpr {
public:
  	SubExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
  	virtual float eval(Env& env) const 
	{
    	return _left->eval(env) - _right->eval(env);
  	}
};

class MulExpr : public BinaryExpr {
public:
  	MulExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
  	virtual float eval(Env& env) const 
	{
    	return _left->eval(env) * _right->eval(env);
  	}
};

class DivExpr : public BinaryExpr {
public:
  	DivExpr(Expr *l, Expr *r) : BinaryExpr(l,r) {}
  	virtual float eval(Env& env) const 
	{
    	return _left->eval(env) / _right->eval(env);
	}
};

class ORbool : public BinaryExpr {
public:
	ORbool(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) || _right->eval(env));
	}
};

class ANDbool : public BinaryExpr {
public:
	ANDbool(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) && _right->eval(env));
	}
};

class NOTbool : public UnaryExpr {
public:
	NOTbool(Expr *e1) : UnaryExpr{e1} {}
	virtual float eval(Env& env) const
	{
		return !(_expr->eval(env));
	}
};

class boolEQ : public BinaryExpr {
public:
	boolEQ(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) == _right->eval(env));
	}
};

class boolNE : public BinaryExpr {
public:
	boolNE(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) != _right->eval(env));
	}
};

class boolLT : public BinaryExpr {
public:
	boolLT(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) < _right->eval(env));
	}
};

class boolLE : public BinaryExpr {
public:
	boolLE(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) <= _right->eval(env));
	}
};

class boolGT : public BinaryExpr {
public:
	boolGT(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) > _right->eval(env));
	}
};

class boolGE : public BinaryExpr {
public:
	boolGE(Expr *e1, Expr *e2) : BinaryExpr{e1, e2} {}
	virtual float eval(Env& env) const
	{
		return (_left->eval(env) >= _right->eval(env));
	}
};

#endif // AST_H