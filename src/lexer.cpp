#include "lexer.h"


namespace dauw
{
  // Return the name of the rule
  std::string LexerRule::name()
  {
    return name_;
  }

  // Return the pattern of the rule
  std::regex LexerRule::pattern()
  {
    return pattern_;
  }

  // Replace a match using this rule
  std::string LexerRule::replace(std::smatch match)
  {
    return replacement_(match);
  }


  // Constructor for the lexer
  Lexer::Lexer()
  {
    // Initialize the regex patterns
    comment_pattern_ = std::regex("(?:--[ \\t]*(.*))|(?:\"((?:[^\\\\\"]|\\\\.)*)\")");
    whitespace_pattern_ = std::regex("[ \\t]+");

    // Initialize the rules
    rules_.push_back(LexerRule("parenthesis_left", std::regex("\\(")));
    rules_.push_back(LexerRule("parenthesis_right", std::regex("\\)")));
    rules_.push_back(LexerRule("square_bracket_left", std::regex("\\[")));
    rules_.push_back(LexerRule("square_bracket_right", std::regex("\\]")));
    rules_.push_back(LexerRule("curly_bracket_left", std::regex("\\{")));
    rules_.push_back(LexerRule("curly_bracket_right", std::regex("\\}")));

    rules_.push_back(LexerRule("symbol_colon", std::regex(":")));
    rules_.push_back(LexerRule("symbol_comma", std::regex(",")));
    rules_.push_back(LexerRule("symbol_dot", std::regex("\\.")));
    rules_.push_back(LexerRule("symbol_lambda", std::regex("\\\\")));

    rules_.push_back(LexerRule("operator_length", std::regex("#")));
    rules_.push_back(LexerRule("operator_string", std::regex("\\$")));
    rules_.push_back(LexerRule("operator_multiply", std::regex("\\*")));
    rules_.push_back(LexerRule("operator_divide", std::regex("/")));
    rules_.push_back(LexerRule("operator_floor_divide", std::regex("//")));
    rules_.push_back(LexerRule("operator_modulo", std::regex("%")));
    rules_.push_back(LexerRule("operator_add", std::regex("\\+")));
    rules_.push_back(LexerRule("operator_subtract", std::regex("-")));
    rules_.push_back(LexerRule("operator_range", std::regex("\\.\\.")));
    rules_.push_back(LexerRule("operator_compare", std::regex("<=>")));
    rules_.push_back(LexerRule("operator_less", std::regex("<")));
    rules_.push_back(LexerRule("operator_less_equal", std::regex("<=")));
    rules_.push_back(LexerRule("operator_greater", std::regex(">")));
    rules_.push_back(LexerRule("operator_greater_equal", std::regex(">=")));
    rules_.push_back(LexerRule("operator_equal", std::regex("==")));
    rules_.push_back(LexerRule("operator_not_equal", std::regex("!=")));
    rules_.push_back(LexerRule("operator_match", std::regex("~")));
    rules_.push_back(LexerRule("operator_logical_not", std::regex("not")));
    rules_.push_back(LexerRule("operator_logical_and", std::regex("and")));
    rules_.push_back(LexerRule("operator_logical_or", std::regex("or")));
    rules_.push_back(LexerRule("operator_assign", std::regex("=")));

    rules_.push_back(LexerRule("keyword_do", std::regex("do")));
    rules_.push_back(LexerRule("keyword_else", std::regex("else")));
    rules_.push_back(LexerRule("keyword_false", std::regex("false")));
    rules_.push_back(LexerRule("keyword_for", std::regex("for")));
    rules_.push_back(LexerRule("keyword_func", std::regex("func")));
    rules_.push_back(LexerRule("keyword_if", std::regex("if")));
    rules_.push_back(LexerRule("keyword_in", std::regex("in")));
    rules_.push_back(LexerRule("keyword_nothing", std::regex("nothing")));
    rules_.push_back(LexerRule("keyword_return", std::regex("return")));
    rules_.push_back(LexerRule("keyword_then", std::regex("then")));
    rules_.push_back(LexerRule("keyword_true", std::regex("true")));
    rules_.push_back(LexerRule("keyword_until", std::regex("until")));
    rules_.push_back(LexerRule("keyword_while", std::regex("while")));

    rules_.push_back(LexerRule("identifier", std::regex("[A-Za-z_][A-Za-z0-9_]*|`((?:[^\\\\`]|\\\\.)+)`"), &replace_identifier_));

    rules_.push_back(LexerRule("int", std::regex("0[Xx][0-9A-Fa-f][0-9A-Fa-f_]*|-?(?:0|[1-9][0-9_]*)"), 0));
    rules_.push_back(LexerRule("real", std::regex("-?(?:0|[1-9][0-9_]*)(?:\\.[0-9][0-9_]*(?:[Ee][+-]?(?:0|[1-9][0-9_]*))?|[Ee][+-]?(?:0|[1-9][0-9_]*))"), 0));
    rules_.push_back(LexerRule("regex", std::regex("/((?:[^\\\\/]|\\\\.)*)/[A-Za-z]*"), 0));
    rules_.push_back(LexerRule("string", std::regex("\"((?:[^\\\\\"]|\\\\.)*)\""), 1));
  }

  // Replacement function for an identifier
  std::string Lexer::replace_identifier_(std::smatch match)
  {
    return match.str(1) != "" ? match.str(1) : match.str(0);
  }

  // Convert a string into a deque of tokens
  std::deque<Token> Lexer::tokenize(std::string source)
  {
    // Initialize a deque to store the tokens
    std::deque<Token> tokens;

    // Initialize a stack to store the indentation levels
    std::deque<int> indents;
    indents.push_back(0);

    // Reset the state of the lexer
    Location location;

    // Iterate over the lines in the source
    for (auto line : regex_lines(source))
    {
      // Check for a shebang at the start of the source
      if (line.rfind("#!", 0) == 0)
      {
        // If this is the first line of the source, then ignore the shebang, otherwise throw an Error
        if (location.line() != 0)
          throw LexerError("A shebang is only allowed at the first line of the source", location);

        // Increase the position to past the shebang
        location.increase_line();
        continue;
      }

      // Skip the line if it is empty or only contains whitespace
      if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos)
      {
        location.increase_line();
        continue;
      }

      // Calculate the indentation at the start of the line and add tokens accordingly
      int indent = 0;
      for (auto indent_iterator = line.begin(); indent_iterator != line.end() && *indent_iterator == ' '; ++ indent_iterator)
        indent ++;

      if (indent > indents.back())
      {
        // Check if the indent is on the first line, because that's an Error
        if (location.line() == 0)
          throw LexerError("The first line of the source should never be indented", location);

        // If the indent is bigger than the last indent, then add an indent token
        indents.push_back(indent);
        tokens.push_back(Token("indent", location));
      }

      while (indent < indents.back())
      {
        // If the indent is smaller than the last indent, then add as much dedent tokens until the indent matches one on the stack
        indents.pop_back();
        tokens.push_back(Token("dedent", location));
      }

      if (indent != indents.back())
      {
        // If the indent doesn't match the current indentation level, then we have an Error
        throw LexerError("The indentation does not match any outer indentation level", location);
      }

      location.increase_col(indent);

      // Iterate over the line
      while (location.col() < line.length())
      {
        // Check for comments at the current position
        auto comment_match = regex_match(comment_pattern_, line, location.col());
        if (comment_match.has_value())
        {
          auto match = comment_match.value();
          if (match.str(1) != "")
          {
            tokens.push_back(Token("comment", match.str(1), location, match.length()));
            location.increase_col(match.length());
            continue;
          }
        }

        // Check for whitespaces at the current position
        auto whitespace_match = regex_match(whitespace_pattern_, line, location.col());
        if (whitespace_match.has_value())
        {
          auto match = whitespace_match.value();
          location.increase_col(match.length());
          continue;
        }

        // Iterate over the rules to see if they match
        std::vector<Token> matched_tokens;
        for (auto rule : rules_)
        {
          auto rule_match = regex_match(rule.pattern(), line, location.col());
          if (rule_match.has_value())
          {
            auto match = rule_match.value();
            matched_tokens.push_back(Token(rule.name(), rule.replace(match), location, match.length()));
          }
        }

        // If there are no matched tokens, then we've encountered a syntax Error
        if (matched_tokens.empty())
          throw LexerError(fmt::format("Invalid character '{}'", line.substr(location.col(), 1)), location);

        // if there is more than one matched token, then sort the current matched tokens
        if (matched_tokens.size() > 1)
        {
          std::sort(matched_tokens.begin(), matched_tokens.end(), [](Token a, Token b)->int {
            if (a.location().line() != b.location().line())
              return a.location().line() - b.location().line();
            else if (a.location().col() != b.location().col())
              return a.location().col() - b.location().col();
            else if (a.length() != b.length())
              return b.length() - a.length();
            else
              return 0;
          });
        }

        // Add the first token and increase the position to past the token
        auto token = *matched_tokens.begin();
        tokens.push_back(token);
        location.increase_col(token.length());
      }

      // Add a newline at the end of the line and update the location to the next line
      tokens.push_back(Token("newline", location));
      location.increase_line();
    }

    // Add dedent tokens for the remaining indents
    while (indents.back() > 0)
    {
      indents.pop_back();
      tokens.push_back(Token("dedent", location));
    }

    // Return the tokens
    return tokens;
  }
}
