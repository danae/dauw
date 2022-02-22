#pragma once

#include <deque>
#include <functional>
#include <regex>
#include <string>
#include <vector>

#include "location.h"
#include "../core/errors.h"

namespace dauw
{
  // Class that defines a rule to use with the lexer
  class LexerRule
  {
  public:
    // The name of the rule
    const char* name;

    // The regex pattern of the rule
    std::regex pattern;

    // The replacement function of this rule
    std::function<std::string(std::smatch)> replacement;


    // Constructor for a rule
    LexerRule(const char* name, std::regex pattern, std::function<std::string(std::smatch)> replacement);
    LexerRule(const char* name, std::regex pattern, int replacement);
    LexerRule(const char* name, std::regex pattern);
  };


  // Class that defines a token to use with the lexer
  class LexerToken
  {
  public:
    // The name and value of the token
    const char* name;
    std::string value;

    // The location of the token
    Location location;

    // Constructor for a token
    LexerToken(const char* name, std::string value, Location location);
    LexerToken(const char* name, Location location);
  };


  // Class that defines a lexer that converts a string into a deque of tokens
  class Lexer
  {
  private:
    // Rules to use while tokenizing
    std::vector<LexerRule> rules_;

  public:
    // Regex patterns for comments, newlines and whitespaces
    static std::regex commentPattern;
    static std::regex newlinePattern;
    static std::regex whitespacePattern;


    // Constructor for a lexer
    Lexer();

    // Convert a string into a deque of tokens
    std::deque<LexerToken> tokenize(std::string source);
  };


  // Class that defines a lexer error
  class LexerError : public Error
  {
  public:
    // The source code in which the error occurred
    std::string source;

    // The location where the error occurred
    Location location;


    // Constructor for a lexer error
    LexerError(std::string message, std::string& source, Location location, Error* previous);
    LexerError(std::string message, std::string& source, Location location);

    // Return the string representation of the error
    std::string what() override;
  };
}
