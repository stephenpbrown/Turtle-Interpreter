#include "Scanner.h"
#include <locale>
#include <sstream>
#include <map>

Token Scanner::nextToken(Attribute& attr, int& lineno) {

  //
  // XXX
  // Return next token / attribute (track line number).
  //

  throw std::runtime_error("Unknown lexeme.");
}

std::string tokenToString(Token token) {
  static std::map<Token,std::string> tokenMap = {
    {Token::UNKNOWN, "UNKNOWN"},
    {Token::EOT,"EOT"},
    {Token::IDENT, "IDENT"},
    {Token::REAL, "REAL"},
    {Token::ASSIGN, "ASSIGN"},
    {Token::EQ, "EQ"},
    {Token::NE, "NE"},
    {Token::LE, "LE"},
    {Token::LT, "LT"},
    {Token::GE, "GE"},
    {Token::GT,"GT"},
    {Token::LPAREN, "LPAREN"},
    {Token::RPAREN, "RPAREN"},
    {Token::OR, "OR"},
    {Token::AND, "AND"},
    {Token::NOT, "NOT"},
    {Token::PLUS, "PLUS"},
    {Token::MINUS, "MINUS"},
    {Token::MULT, "MULT"},
    {Token::DIV,"DIV"},
    {Token::IF, "IF"},
    {Token::THEN, "THEN"},
    {Token::ELSIF, "ELSIF"},
    {Token::ELSE, "ELSE"},
    {Token::FI,"FI"},
    {Token::WHILE, "WHILE"},
    {Token::DO, "DO"},
    {Token::OD,"OD"},
    {Token::HOME, "HOME"},
    {Token::PENUP, "PENUP"},
    {Token::PENDOWN, "PENDOWN"},
    {Token::FORWARD, "FORWARD"},
    {Token::LEFT, "LEFT"},
    {Token::RIGHT, "RIGHT"},
    {Token::PUSHSTATE, "PUSHSTATE"},
    {Token::POPSTATE,"POPSTATE"}
  };
  auto p = tokenMap.find(token);
  if (p != tokenMap.end())
    return p->second;
  return "?";
}

