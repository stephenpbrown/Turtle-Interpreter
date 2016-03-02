#include "Parser.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

void Parser::match(Token tok) {
  	if (tok != lookahead_) 
	{
    	std::stringstream ss;
    	ss << "Unexpected token '" << tokenToString(lookahead_) << "', ";
    	ss << "Expecting '" << tokenToString(tok) << "'";
    	throw std::runtime_error(ss.str());
  	}
  	lookahead_ = scanner_.nextToken(attribute_, lineno_);
}

void Parser::parse() 
{
  	lookahead_ = scanner_.nextToken(attribute_, lineno_);
  	try {
    	prog();
  	} catch(const std::exception& error) 
	{
    	std::stringstream ss;
    	ss << lineno_ << ": " << error.what();
    	throw std::runtime_error(ss.str());
  	}
}

void Parser::prog() 
{
  	stmt_seq();
  	match(Token::EOT);
}

void Parser::stmt_seq() 
{
  	do
	{
    	Stmt *s = block();
    	AST_.push_back(s);
  	}
	while (lookahead_ != Token::EOT);
}

Stmt *Parser::stmt() 
{
   	if (lookahead_ == Token::IDENT)
    	return assign();
	if(lookahead_ == Token::WHILE)
		return while_stmt();
	if(lookahead_ == Token::IF)
		return if_stmt();
  	else
    	return action();
}

Stmt *Parser::assign() 
{
    std::string name = attribute_.s;
    match(Token::IDENT);
    match(Token::ASSIGN);
    Expr *e = expr();
    return new AssignStmt(name, e);
}

// ?
Stmt *Parser::block() // Put in whatever a block may be, OD, ELSIF, ELSE, FI
{
	//std::cout << "In block(): " << tokenToString(lookahead_) << std::endl; // DEBUG
	std::vector<Stmt*> stmts;
	// What could it be? Any turtle things
  	while(lookahead_ == Token::WHILE    ||
		  lookahead_ == Token::IF       ||
		  lookahead_ == Token::IDENT    ||
		  lookahead_ == Token::HOME     ||
		  lookahead_ == Token::PENDOWN  ||
		  lookahead_ == Token::PENUP    ||
		  lookahead_ == Token::FORWARD  ||
		  lookahead_ == Token::RIGHT    ||
		  lookahead_ == Token::LEFT     ||
		  lookahead_ == Token::PUSHSTATE||
		  lookahead_ == Token::POPSTATE 
		  )
	{
		Stmt *s = stmt();
		stmts.push_back(s);
	}
	return new BlockStmt(stmts);
}

Stmt *Parser::while_stmt() 
{
	//std::cout << "In while_stmt: " << tokenToString(lookahead_) << std::endl; // DEBUG

	match(Token::WHILE);
	Expr *cond = bool_();
 	match(Token::DO);
  	Stmt *body = block();
  	match(Token::OD);
  	return new WhileStmt(cond, body);
}

// ?
Stmt *Parser::else_part() 
{
	switch(lookahead_)
	{
		case Token::ELSIF:
		{
			match(Token::ELSIF);
			Expr *cond = bool_();
			match(Token::THEN);
  			Stmt *body = block();
			Stmt *elsePart = else_part();
			return new IfStmt(cond, body, elsePart);
		}
		case Token::ELSE:
		{
			match(Token::ELSE);
			Stmt *body = block();
			match(Token::FI);
			return body;
		}
		case Token::FI:
		{
			match(Token::FI);
			return nullptr;
		}
		default:
			throw std::runtime_error("Expecting turtle else_part statement!");
	}
}

// ?
Stmt *Parser::if_stmt() 
{
	//std::cout << "In if_stmt: " << tokenToString(lookahead_) << std::endl; // DEBUG
	match(Token::IF);
	Expr *cond = bool_(); // How to handle OR?
	//std::cout << "In if_stmt: " << tokenToString(lookahead_) << std::endl; // DEBUG 
 	match(Token::THEN);
  	Stmt *body = block();
	Stmt *elsePart = else_part();
  	return new IfStmt(cond, body, elsePart);
}

Stmt *Parser::action() 
{
	switch(lookahead_) 
	{
		case Token::HOME:    match(Token::HOME); 	return new HomeStmt();
		case Token::PENUP:   match(Token::PENUP); 	return new PenUpStmt();
		case Token::PENDOWN: match(Token::PENDOWN); return new PenDownStmt();
		case Token::FORWARD: match(Token::FORWARD); return new ForwardStmt(expr());
		case Token::LEFT:    match(Token::LEFT); 	return new LeftStmt(expr());
		case Token::RIGHT:   match(Token::RIGHT); 	return new RightStmt(expr());
		case Token::PUSHSTATE: 
			match(Token::PUSHSTATE); return new PushStateStmt();
		case Token::POPSTATE:
			match(Token::POPSTATE); return new PopStateStmt();
		default:
		    throw std::runtime_error("Expecting turtle action statement!");
	}
}

Expr *Parser::expr() 
{
	Expr *e = term();
	while (	lookahead_ == Token::PLUS ||
			lookahead_ == Token::MINUS) 
	{
	    const auto op = lookahead_;
	    match(lookahead_);
	    Expr *t = term();
	    if (op == Token::PLUS)
    		e = new AddExpr(e, t);
    	else
    		e = new SubExpr(e, t);
  	}
  	return e;
}

Expr *Parser::term() 
{
	Expr *e = factor();
	while (	lookahead_ == Token::MULT ||
			lookahead_ == Token::DIV) 
	{
    	Token op = lookahead_;
    	match(lookahead_);
    	Expr *t = factor();
    	if (op == Token::MULT)
    		e = new MulExpr(e, t);
    	else
    		e = new DivExpr(e, t);
  	}
  	return e;
}

Expr *Parser::factor() 
{
	//std::cout << "In factor(): " << tokenToString(lookahead_) << std::endl; // DEBUG

    switch(lookahead_) 
	{
  		case Token::PLUS:   match(Token::PLUS); return factor();
  		case Token::MINUS:  match(Token::MINUS); return new NegExpr(factor());
  		case Token::LPAREN:
    	{
      		match(Token::LPAREN);
      		Expr *e = expr();
      		match(Token::RPAREN);
      		return e;
    	}
  		case Token::IDENT:
    	{
      		const std::string name = attribute_.s;
      		match(Token::IDENT);
      		return new VarExpr(name);
    	}
  		case Token::REAL:
    	{
      		const float val = attribute_.f;
      		match(Token::REAL);
      		return new ConstExpr(val);
    	}
  		default:
    		throw std::runtime_error("Expecting factor!");
  	}
}


Expr *Parser::bool_() 
{
	//std::cout << "In bool_(): " << tokenToString(lookahead_) << std::endl; // DEBUG

	Expr *_expr = bool_term();
	while(lookahead_ == Token::OR)
	{
		match(Token::OR);
		Expr *t = bool_term();
		_expr = new ORbool(_expr, t); // Make a new class
	}
	return _expr;
}

Expr *Parser::bool_term() 
{
	Expr *_expr = bool_factor();
	while(lookahead_ == Token::AND)
	{
		match(Token::AND);
		Expr *t = bool_factor();
		_expr = new ANDbool(_expr, t); // Make a new class
	}
	return _expr;
}

Expr *Parser::bool_factor() 
{
	//std::cout << "In bool_factor(): " << tokenToString(lookahead_) << std::endl; // DEBUG

	switch(lookahead_)
	{
		case Token::NOT:
		{
			match(Token::NOT);
			Expr *_bool_factor = bool_factor();
			return new NOTbool(_bool_factor); // New function
		}
		case Token::LPAREN:
		{
			match(Token::LPAREN);
			Expr *_expr = bool_();
			match(Token::RPAREN);
			return _expr;
		}
		case Token::IDENT:
		{
			Expr *_cmp = cmp(); // Issue here
			return _cmp;
		}
		default:
			throw std::runtime_error("Expecting bool_factor statement!");
	}
}

Expr *Parser::cmp() 
{
	//std::cout << "In cmp(): " << tokenToString(lookahead_) << std::endl; // DEBUG

	Expr *_expr1 = expr(); // Catch and save IDENT

	switch(lookahead_)
	{
		case Token::EQ:
		{
			match(Token::EQ);
			Expr *_expr2 = expr();
			return new boolEQ(_expr1, _expr2); // Make new class 
		}
		case Token::NE:
		{
			match(Token::NE);
			//Expr *_expr1 = expr();
			Expr *_expr2 = expr();
			return new boolNE(_expr1, _expr2); // Make new class
		}
		case Token::LT:
		{
			match(Token::LT);
			Expr *_expr2 = expr();
			return new boolLT(_expr1, _expr2); // Make new class
		}
		case Token::LE:
		{
			match(Token::LE); // Match 
			Expr *_expr2 = expr(); // Catch and save second IDENT
			return new boolLE(_expr1, _expr2); // Make new class
		}
		case Token::GT:
		{
			match(Token::GT);
			Expr *_expr2 = expr();
			return new boolGT(_expr1, _expr2); // Make new class
		}
		case Token::GE:
		{
			match(Token::GE);
			Expr *_expr2 = expr();
			return new boolGE(_expr1, _expr2); // Make new class
		}
		default:
			throw std::runtime_error("Error in cmp function!");
		}
}
