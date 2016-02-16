#include "Parser.h"
#include <sstream>
#include <stdexcept>

void Parser::match(Token tok) {
  if (tok != lookahead_) {
    std::stringstream ss;
    ss << "Unexpected token '" << tokenToString(lookahead_) << "', ";
    ss << "Expecting '" << tokenToString(tok) << "'";
    throw std::runtime_error(ss.str());
  }
  lookahead_ = scanner_.nextToken(attribute_, lineno_);
}

void Parser::parse() {
  lookahead_ = scanner_.nextToken(attribute_, lineno_);
  try {
    prog();
  } catch(const std::exception& error) {
    std::stringstream ss;
    ss << lineno_ << ": " << error.what();
    throw std::runtime_error(ss.str());
  }
}

void Parser::prog() {
  stmt_seq();
  match(Token::EOT);
}

void Parser::stmt_seq() {
  while (lookahead_ != Token::EOT) {
    Stmt *s = block();
    AST_.push_back(s);
  }
}

Stmt *Parser::stmt() {
  // XXX
}

Stmt *Parser::assign() {
  // XXX
}

Stmt *Parser::block() {
  // XXX
}

Stmt *Parser::while_stmt() {
  match(Token::WHILE);
  Expr *cond = bool_();
  match(Token::DO);
  Stmt *body = block();
  match(Token::OD);
  return new WhileStmt(cond, body);
}

Stmt *Parser::elsePart() {
  // XXX
}

Stmt *Parser::if_stmt() {
  // XXX
}

Stmt *Parser::action() {
  // XXX
}

Expr *Parser::expr() {
  Expr *e = term();
  while (lookahead_ == Token::PLUS ||
	 lookahead_ == Token::MINUS) {
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

Expr *Parser::term() {
  // XXX
}

Expr *Parser::factor() {
  // XXX
}

Expr *Parser::bool_() {
  // XXX
}

Expr *Parser::bool_term() {
  // XXX
}

Expr *Parser::bool_factor() {
  // XXX
}

Expr *Parser::cmp() {
  // XXX
}


