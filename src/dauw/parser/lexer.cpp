#include "lexer.h"

namespace dauw
{
  // Return the name of the rule
  string_t LexerRule::name()
  {
    return name_;
  }

  // Return the pattern of the rule
  regex_t LexerRule::pattern()
  {
    return pattern_;
  }

  // Replace a match using this rule
  string_t LexerRule::replace(match_t match)
  {
    return replacement_(match);
  }


  // Constructor for the lexer
  Lexer::Lexer(string_t name)
    : name_(name)
  {
    // Initialize the regex patterns
    comment_pattern_ = regex_t("(?:--[ \\t]*(.*))|(?:\"((?:[^\\\\\"]|\\\\.)*)\")");
    whitespace_pattern_ = regex_t("[ \\t]+");

    // Initialize the rules
    rules_.push_back(LexerRule("parenthesis_left", regex_t("\\(")));
    rules_.push_back(LexerRule("parenthesis_right", regex_t("\\)")));
    rules_.push_back(LexerRule("square_bracket_left", regex_t("\\[")));
    rules_.push_back(LexerRule("square_bracket_right", regex_t("\\]")));
    rules_.push_back(LexerRule("curly_bracket_left", regex_t("\\{")));
    rules_.push_back(LexerRule("curly_bracket_right", regex_t("\\}")));

    rules_.push_back(LexerRule("symbol_colon", regex_t(":")));
    rules_.push_back(LexerRule("symbol_comma", regex_t(",")));
    rules_.push_back(LexerRule("symbol_dot", regex_t("\\.")));
    rules_.push_back(LexerRule("symbol_lambda", regex_t("\\\\")));

    rules_.push_back(LexerRule("operator_length", regex_t("#"), 0));
    rules_.push_back(LexerRule("operator_string", regex_t("\\$"), 0));
    rules_.push_back(LexerRule("operator_multiply", regex_t("\\*"), 0));
    rules_.push_back(LexerRule("operator_divide", regex_t("/"), 0));
    rules_.push_back(LexerRule("operator_floor_divide", regex_t("//"), 0));
    rules_.push_back(LexerRule("operator_modulo", regex_t("%"), 0));
    rules_.push_back(LexerRule("operator_add", regex_t("\\+"), 0));
    rules_.push_back(LexerRule("operator_subtract", regex_t("-"), 0));
    rules_.push_back(LexerRule("operator_range", regex_t("\\.\\."), 0));
    rules_.push_back(LexerRule("operator_threeway", regex_t("<=>"), 0));
    rules_.push_back(LexerRule("operator_less", regex_t("<"), 0));
    rules_.push_back(LexerRule("operator_less_equal", regex_t("<="), 0));
    rules_.push_back(LexerRule("operator_greater", regex_t(">"), 0));
    rules_.push_back(LexerRule("operator_greater_equal", regex_t(">="), 0));
    rules_.push_back(LexerRule("operator_divisible", regex_t("%%"), 0));
    rules_.push_back(LexerRule("operator_equal", regex_t("=="), 0));
    rules_.push_back(LexerRule("operator_not_equal", regex_t("<>"), 0));
    rules_.push_back(LexerRule("operator_match", regex_t("~"), 0));
    rules_.push_back(LexerRule("operator_logical_not", regex_t("not"), 0));
    rules_.push_back(LexerRule("operator_logical_and", regex_t("and"), 0));
    rules_.push_back(LexerRule("operator_logical_or", regex_t("or"), 0));
    rules_.push_back(LexerRule("operator_assign", regex_t("="), 0));

    rules_.push_back(LexerRule("keyword_do", regex_t("do")));
    rules_.push_back(LexerRule("keyword_else", regex_t("else")));
    rules_.push_back(LexerRule("keyword_false", regex_t("false")));
    rules_.push_back(LexerRule("keyword_for", regex_t("for")));
    rules_.push_back(LexerRule("keyword_func", regex_t("func")));
    rules_.push_back(LexerRule("keyword_if", regex_t("if")));
    rules_.push_back(LexerRule("keyword_in", regex_t("in")));
    rules_.push_back(LexerRule("keyword_nothing", regex_t("nothing")));
    rules_.push_back(LexerRule("keyword_return", regex_t("return")));
    rules_.push_back(LexerRule("keyword_then", regex_t("then")));
    rules_.push_back(LexerRule("keyword_true", regex_t("true")));
    rules_.push_back(LexerRule("keyword_until", regex_t("until")));
    rules_.push_back(LexerRule("keyword_while", regex_t("while")));

    rules_.push_back(LexerRule("identifier", regex_t("[A-Za-z_][A-Za-z0-9_]*|`((?:[^\\\\`]|\\\\.)+)`"), &replace_identifier_));

    rules_.push_back(LexerRule("int", regex_t("0[Xx][0-9A-Fa-f][0-9A-Fa-f_]*|-?(?:0|[1-9][0-9_]*)"), 0));
    rules_.push_back(LexerRule("real", regex_t("-?(?:0|[1-9][0-9_]*)(?:\\.[0-9][0-9_]*(?:[Ee][+-]?(?:0|[1-9][0-9_]*))?|[Ee][+-]?(?:0|[1-9][0-9_]*))"), 0));
    rules_.push_back(LexerRule("rune", regex_t("'((?:[^\\\\']|\\\\.)*)'"), 1));
    rules_.push_back(LexerRule("string", regex_t("\"((?:[^\\\\\"]|\\\\.)*)\""), 1));
    rules_.push_back(LexerRule("regex", regex_t("/((?:[^\\\\/]|\\\\.)*)/[A-Za-z]*"), 0));
  }

  // Replacement function for an identifier
  string_t Lexer::replace_identifier_(match_t match)
  {
    return match.str(1) != "" ? match.str(1) : match.str(0);
  }

  // Convert a string into a deque of tokens
  std::deque<Token> Lexer::tokenize(string_t source)
  {
    // Initialize a deque to store the tokens
    std::deque<Token> tokens;

    // Initialize a stack to store the indentation levels
    std::deque<int> indents;
    indents.push_back(0);

    // Reset the state of the lexer
    Location location(name_);

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
        location.increase_line_();
        continue;
      }

      // Skip the line if it is empty or only contains whitespace
      if (line.empty() || line.find_first_not_of(" \t\r\n") == string_t::npos)
      {
        location.increase_line_();
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

      location.increase_col_(indent);

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
            location.increase_col_(match.length());
            continue;
          }
        }

        // Check for whitespaces at the current position
        auto whitespace_match = regex_match(whitespace_pattern_, line, location.col());
        if (whitespace_match.has_value())
        {
          auto match = whitespace_match.value();
          location.increase_col_(match.length());
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
          std::sort(matched_tokens.begin(), matched_tokens.end());

        // Add the first token and increase the position to past the token
        auto token = *matched_tokens.begin();
        tokens.push_back(token);
        location.increase_col_(token.length());
      }

      // Add a newline at the end of the line and update the location to the next line
      tokens.push_back(Token("newline", location));
      location.increase_line_();
    }

    // Add dedent tokens for the remaining indents
    while (indents.back() > 0)
    {
      indents.pop_back();
      tokens.push_back(Token("dedent", location));
    }

    // Add eof token
    tokens.push_back(Token("eof", location));

    // Return the tokens
    return tokens;
  }
}
