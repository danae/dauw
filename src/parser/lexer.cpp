#include "lexer.h"

#include "../debug.h"
#include "../utils.h"

//-----------------------------------------------------------------------------

// Constructor for a rule
dauw::LexerRule::LexerRule(const char* name, std::regex pattern, std::function<std::string(std::smatch)> replacement)
{
  this->name = name;
  this->pattern = pattern;
  this->replacement = replacement;
}
dauw::LexerRule::LexerRule(const char* name, std::regex pattern, int replacement)
  : LexerRule(name, pattern, [=](std::smatch match)->std::string { return match[replacement].str(); })
{
}
dauw::LexerRule::LexerRule(const char* name, std::regex pattern)
  : LexerRule(name, pattern, [=](std::smatch match)->std::string { return ""; })
{
}

//-----------------------------------------------------------------------------

// Constructor for a token
dauw::LexerToken::LexerToken(const char* name, std::string value, dauw::Location location)
{
  this->name = name;
  this->value = value;
  this->location = location;
}
dauw::LexerToken::LexerToken(const char* name, dauw::Location location)
 : LexerToken(name, "", location)
{
}

//-----------------------------------------------------------------------------

// Constructor for a lexer
dauw::Lexer::Lexer()
{
  // Add rules for parentheses
  _rules.push_back(LexerRule("parenthesis_left", std::regex("\\(")));
  _rules.push_back(LexerRule("parenthesis_right", std::regex("\\)")));
  _rules.push_back(LexerRule("square_bracket_left", std::regex("\\[")));
  _rules.push_back(LexerRule("square_bracket_right", std::regex("\\]")));
  _rules.push_back(LexerRule("curly_bracket_left", std::regex("\\{")));
  _rules.push_back(LexerRule("curly_bracket_right", std::regex("\\}")));

  // Add rules for symbols
  _rules.push_back(LexerRule("symbol_colon", std::regex(":")));
  _rules.push_back(LexerRule("symbol_comma", std::regex(",")));
  _rules.push_back(LexerRule("symbol_dot", std::regex("\\.")));

  // Add rules for operators
  _rules.push_back(LexerRule("operator_length", std::regex("#")));
  _rules.push_back(LexerRule("operator_string", std::regex("\\$")));
  _rules.push_back(LexerRule("operator_multiply", std::regex("\\*")));
  _rules.push_back(LexerRule("operator_divide", std::regex("/")));
  _rules.push_back(LexerRule("operator_floor_divide", std::regex("//")));
  _rules.push_back(LexerRule("operator_modulo", std::regex("%")));
  _rules.push_back(LexerRule("operator_add", std::regex("\\+")));
  _rules.push_back(LexerRule("operator_subtract", std::regex("-")));
  _rules.push_back(LexerRule("operator_range", std::regex("\\.\\.")));
  _rules.push_back(LexerRule("operator_compare", std::regex("<=>")));
  _rules.push_back(LexerRule("operator_less", std::regex("<")));
  _rules.push_back(LexerRule("operator_less_equal", std::regex("<=")));
  _rules.push_back(LexerRule("operator_greater", std::regex(">")));
  _rules.push_back(LexerRule("operator_greater_equal", std::regex(">=")));
  _rules.push_back(LexerRule("operator_equal", std::regex("==")));
  _rules.push_back(LexerRule("operator_not_equal", std::regex("!=")));
  _rules.push_back(LexerRule("operator_match", std::regex("~")));
  _rules.push_back(LexerRule("operator_logical_not", std::regex("not")));
  _rules.push_back(LexerRule("operator_logical_and", std::regex("and")));
  _rules.push_back(LexerRule("operator_logical_or", std::regex("or")));
  _rules.push_back(LexerRule("operator_assign", std::regex("=")));

  // Add rules for keywords
  _rules.push_back(LexerRule("keyword_break", std::regex("break")));
  _rules.push_back(LexerRule("keyword_continue", std::regex("continue")));
  _rules.push_back(LexerRule("keyword_discard", std::regex("discard")));
  _rules.push_back(LexerRule("keyword_do", std::regex("do")));
  _rules.push_back(LexerRule("keyword_else", std::regex("else")));
  _rules.push_back(LexerRule("keyword_false", std::regex("false")));
  _rules.push_back(LexerRule("keyword_for", std::regex("for")));
  _rules.push_back(LexerRule("keyword_if", std::regex("if")));
  _rules.push_back(LexerRule("keyword_in", std::regex("in")));
  _rules.push_back(LexerRule("keyword_nothing", std::regex("nothing")));
  _rules.push_back(LexerRule("keyword_return", std::regex("return")));
  _rules.push_back(LexerRule("keyword_save", std::regex("save")));
  _rules.push_back(LexerRule("keyword_then", std::regex("then")));
  _rules.push_back(LexerRule("keyword_true", std::regex("true")));
  _rules.push_back(LexerRule("keyword_until", std::regex("until")));
  _rules.push_back(LexerRule("keyword_while", std::regex("while")));
  _rules.push_back(LexerRule("keyword_yield", std::regex("yield")));

  // Add rules for identfiers
  _rules.push_back(LexerRule("identifier", std::regex("[A-Za-z_][A-Za-z0-9_]*|`([^`]+)`"), [=](std::smatch match)->std::string { return match.str(1) != "" ? match.str(1) : match.str(); }));

  // Add rules for literals
  _rules.push_back(LexerRule("int", std::regex("0[Xx][0-9A-Fa-f][0-9A-Fa-f_]*|-?(?:0|[1-9][0-9_]*)"), 0));
  _rules.push_back(LexerRule("real", std::regex("-?(?:0|[1-9][0-9_]*)(?:\\.[0-9][0-9_]*(?:[Ee][+-]?(?:0|[1-9][0-9_]*))?|[Ee][+-]?(?:0|[1-9][0-9_]*))"), 0));
  _rules.push_back(LexerRule("regex", std::regex("/((?:[^\\\\/]|\\\\.)*)/[A-Za-z]*"), 0));
  _rules.push_back(LexerRule("string", std::regex("\"((?:[^\\\\\"]|\\\\.)*)\""), 1));

  // Create the regex patterns for comments, newlines and whitespaces
  _commentPattern = std::regex("(?:--[ \\t]*(.*))|(?:\"((?:[^\\\\\"]|\\\\.)*)\")");
  _newlinePattern = std::regex("\\r?\\n");
  _whitespacePattern = std::regex("[ \\t]+");
}

// Convert a string into a vector of tokens
std::deque<dauw::LexerToken> dauw::Lexer::tokenize(std::string source)
{
  // Reset the state of the lexer
  Location location;

  // Initialize a deque to store the tokens
  std::deque<LexerToken> tokens;

  // Initialize a stack to store the indentation levels
  std::deque<int> indents;
  indents.push_back(0);

  // Iterate over the lines in the source
  for (auto line : splitRegex(source, _newlinePattern))
  {
    // Skip the line if it is empty or only contains whitespace
    if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos)
    {
      location.line ++;
      continue;
    }

    // Calculate the indentation at the start of the line and add tokens accordingly
    int indent = 0;
    for (auto indentIterator = line.begin(); indentIterator != line.end() && *indentIterator == ' '; ++ indentIterator)
      indent ++;

    if (indent > indents.back())
    {
      // Check if the indent is on the first line, because that's an error
      if (location.line == 0)
        throw LexerError("The first line should never be indented", location);

      // If the indent is bigger than the last indent, then add an indent token
      indents.push_back(indent);
      tokens.push_back(LexerToken("indent", location));
    }

    while (indent < indents.back())
    {
      // If the indent is smaller than the last indent, then add as much dedent tokens until the indent matches one on the stack
      indents.pop_back();
      tokens.push_back(LexerToken("dedent", location));
    }

    if (indent != indents.back())
    {
      // If the indent doesn't match the current indentation level, then we have an error
      throw LexerError("The indentation does not match any outer indentation level", location);
    }

    location.col += indent;

    // Iterate over the line
    while (location.col < line.length())
    {
      // Check for comments at the current position
      std::smatch commentMatch;
      if (std::regex_search(line.cbegin() + location.col, line.cend(), commentMatch, _commentPattern, std::regex_constants::match_continuous) && commentMatch.str(1) != "")
      {
        // Add a comment token
        tokens.push_back(LexerToken("comment", commentMatch.str(1), location));

        // Increase the position to past the comment
        location.col += commentMatch.length();
        continue;
      }

      // Check for whitespaces at the current position
      std::smatch whitespaceMatch;
      if (std::regex_search(line.cbegin() + location.col, line.cend(), whitespaceMatch, _whitespacePattern, std::regex_constants::match_continuous))
      {
        // Increase the position to past the whitespace
        location.col += whitespaceMatch.length();
        continue;
      }

      // Iterate over the rules to see if they match
      std::vector<std::tuple<LexerToken, int> > tuples;
      for (int i = 0; i < _rules.size(); i ++)
      {
        auto rule = _rules[i];
        std::smatch ruleMatch;
        if (std::regex_search(line.cbegin() + location.col, line.cend(), ruleMatch, rule.pattern, std::regex_constants::match_continuous))
          tuples.push_back(std::make_tuple(LexerToken(rule.name, rule.replacement(ruleMatch), location), ruleMatch.length()));
      }

      // If there are no tuples, then we've encountered a syntax error
      if (tuples.empty())
        throw LexerError(fmt::format("Invalid character '{}'", line.substr(location.col, 1)), location);

      // if there is more than one tuple, then sort the current tuples
      if (tuples.size() > 1)
      {
        std::sort(tuples.begin(), tuples.end(), [](std::tuple<LexerToken, int> a, std::tuple<LexerToken, int> b)->int {
          auto aToken = std::get<0>(a);
          auto aLength = std::get<1>(a);

          auto bToken = std::get<0>(b);
          auto bLength = std::get<1>(b);

          if (aToken.location.line != bToken.location.line)
            return aToken.location.line - bToken.location.line;
          else if (aToken.location.col != aToken.location.col)
            return aToken.location.col - bToken.location.col;
          else if (aLength != bLength)
            return bLength - aLength;
          else
            return 0;
        });
      }

      // Add the first token and increase the position to past the token
      auto tuple = *tuples.begin();
      tokens.push_back(std::get<0>(tuple));
      location.col += std::get<1>(tuple);
    }

    // Add a newline at the end of the line and pdate the location to the next line
    tokens.push_back(LexerToken("newline", location));

    location.line ++;
    location.col = 0;
  }

  // Add dedent tokens for the remaining indents
  while (indents.back() > 0)
  {
    indents.pop_back();
    tokens.push_back(LexerToken("dedent", location));
  }

  // Return the tokens
  return tokens;
}

//-----------------------------------------------------------------------------

// Constructor for a lexer error
dauw::LexerError::LexerError(std::string message, dauw::Location location, dauw::Error* previous)
  : Error(message, previous)
{
  this->location = location;
}
dauw::LexerError::LexerError(std::string message, dauw::Location location)
  : LexerError(message, location, NULL)
{
}

// Return the string representation of the error
std::string dauw::LexerError::what()
{
  return fmt::format("LexerError: {} at line {}, col {}", message, location.line + 1, location.col + 1);
}

//-----------------------------------------------------------------------------
