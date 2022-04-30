#include "source_lexer.h"


namespace dauw
{
  // Constructor for a lexer rule
  LexerRule::LexerRule(std::string name, std::regex pattern, std::function<std::string(std::smatch)> replacement)
  {
    m_name = name;
    m_pattern = pattern;
    m_replacement = replacement;
  }

  // Constructor for a lexer rule containing the specified match group as the replacement value
  LexerRule::LexerRule(std::string name, std::regex pattern, size_t replacement)
    : LexerRule(name, pattern, [replacement](std::smatch match)->std::string { return match[replacement].str(); }) { }

  // Constructor for a lexer rule containing nothing as the replacement value
  LexerRule::LexerRule(std::string name, std::regex pattern)
    : LexerRule(name, pattern, [](std::smatch match)->std::string { return ""; }) { }


  // Constructor for a lexer token
  LexerToken::LexerToken(std::string name, std::string value, Location location, size_t length)
  {
    m_name = name;
    m_value = value;
    m_location = location;
    m_length = length;
  }

  // Constructor for a lexer token with no value
  LexerToken::LexerToken(std::string name, Location location, size_t length)
    : LexerToken(name, "", location, length) { }


  // Convert a string into a deque of tokens
  std::deque<LexerToken> tokenize(std::string source)
  {
    // Initialize regex patterns for comments, newlines and whitespaces
    std::regex newline_pattern("\\r?\\n");
    std::regex comment_pattern("(?:--[ \\t]*(.*))|(?:\"((?:[^\\\\\"]|\\\\.)*)\")");
    std::regex whitespace_pattern("[ \\t]+");

    // Initialize a vector of rules for the lexer
    std::vector<LexerRule> rules;

    rules.push_back(LexerRule("parenthesis_left", std::regex("\\(")));
    rules.push_back(LexerRule("parenthesis_right", std::regex("\\)")));
    rules.push_back(LexerRule("square_bracket_left", std::regex("\\[")));
    rules.push_back(LexerRule("square_bracket_right", std::regex("\\]")));
    rules.push_back(LexerRule("curly_bracket_left", std::regex("\\{")));
    rules.push_back(LexerRule("curly_bracket_right", std::regex("\\}")));

    rules.push_back(LexerRule("symbol_colon", std::regex(":")));
    rules.push_back(LexerRule("symbol_comma", std::regex(",")));
    rules.push_back(LexerRule("symbol_dot", std::regex("\\.")));
    rules.push_back(LexerRule("symbol_lambda", std::regex("\\\\")));

    rules.push_back(LexerRule("operator_length", std::regex("#")));
    rules.push_back(LexerRule("operator_string", std::regex("\\$")));
    rules.push_back(LexerRule("operator_multiply", std::regex("\\*")));
    rules.push_back(LexerRule("operator_divide", std::regex("/")));
    rules.push_back(LexerRule("operator_floor_divide", std::regex("//")));
    rules.push_back(LexerRule("operator_modulo", std::regex("%")));
    rules.push_back(LexerRule("operator_add", std::regex("\\+")));
    rules.push_back(LexerRule("operator_subtract", std::regex("-")));
    rules.push_back(LexerRule("operator_range", std::regex("\\.\\.")));
    rules.push_back(LexerRule("operator_compare", std::regex("<=>")));
    rules.push_back(LexerRule("operator_less", std::regex("<")));
    rules.push_back(LexerRule("operator_less_equal", std::regex("<=")));
    rules.push_back(LexerRule("operator_greater", std::regex(">")));
    rules.push_back(LexerRule("operator_greater_equal", std::regex(">=")));
    rules.push_back(LexerRule("operator_equal", std::regex("==")));
    rules.push_back(LexerRule("operator_not_equal", std::regex("!=")));
    rules.push_back(LexerRule("operator_match", std::regex("~")));
    rules.push_back(LexerRule("operator_logical_not", std::regex("not")));
    rules.push_back(LexerRule("operator_logical_and", std::regex("and")));
    rules.push_back(LexerRule("operator_logical_or", std::regex("or")));
    rules.push_back(LexerRule("operator_assign", std::regex("=")));

    rules.push_back(LexerRule("keyword_do", std::regex("do")));
    rules.push_back(LexerRule("keyword_else", std::regex("else")));
    rules.push_back(LexerRule("keyword_false", std::regex("false")));
    rules.push_back(LexerRule("keyword_for", std::regex("for")));
    rules.push_back(LexerRule("keyword_func", std::regex("func")));
    rules.push_back(LexerRule("keyword_if", std::regex("if")));
    rules.push_back(LexerRule("keyword_in", std::regex("in")));
    rules.push_back(LexerRule("keyword_nothing", std::regex("nothing")));
    rules.push_back(LexerRule("keyword_return", std::regex("return")));
    rules.push_back(LexerRule("keyword_then", std::regex("then")));
    rules.push_back(LexerRule("keyword_true", std::regex("true")));
    rules.push_back(LexerRule("keyword_until", std::regex("until")));
    rules.push_back(LexerRule("keyword_while", std::regex("while")));

    rules.push_back(LexerRule("identifier", std::regex("[A-Za-z_][A-Za-z0-9_]*|`((?:[^\\\\`]|\\\\.)+)`"),
      [](std::smatch match)->std::string { return match.str(1) != "" ? match.str(1) : match.str(0); }));

    rules.push_back(LexerRule("int", std::regex("0[Xx][0-9A-Fa-f][0-9A-Fa-f_]*|-?(?:0|[1-9][0-9_]*)"), 0));
    rules.push_back(LexerRule("real", std::regex("-?(?:0|[1-9][0-9_]*)(?:\\.[0-9][0-9_]*(?:[Ee][+-]?(?:0|[1-9][0-9_]*))?|[Ee][+-]?(?:0|[1-9][0-9_]*))"), 0));
    rules.push_back(LexerRule("regex", std::regex("/((?:[^\\\\/]|\\\\.)*)/[A-Za-z]*"), 0));
    rules.push_back(LexerRule("string", std::regex("\"((?:[^\\\\\"]|\\\\.)*)\""), 1));

    // Initialize a deque to store the tokens
    std::deque<LexerToken> tokens;

    // Initialize a stack to store the indentation levels
    std::deque<int> indents;
    indents.push_back(0);

    // Reset the state of the lexer
    Location location;


    // Iterate over the lines in the source
    for (auto line : regex_split(newline_pattern, source))
    {
      // Check for a shebang at the start of the source
      if (line.rfind("#!", 0) == 0)
      {
        // If this is the first line of the source, then ignore the shebang, otherwise throw an Error
        if (location.m_line != 0)
          throw LexerError("A shebang is only allowed at the first line of the source", location);

        // Increase the position to past the shebang
        location.m_line ++;
        continue;
      }

      // Skip the line if it is empty or only contains whitespace
      if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos)
      {
        location.m_line ++;
        continue;
      }

      // Calculate the indentation at the start of the line and add tokens accordingly
      int indent = 0;
      for (auto indent_iterator = line.begin(); indent_iterator != line.end() && *indent_iterator == ' '; ++ indent_iterator)
        indent ++;

      if (indent > indents.back())
      {
        // Check if the indent is on the first line, because that's an Error
        if (location.m_line == 0)
          throw LexerError("The first line of the source should never be indented", location);

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
        // If the indent doesn't match the current indentation level, then we have an Error
        throw LexerError("The indentation does not match any outer indentation level", location);
      }

      location.m_col += indent;

      // Iterate over the line
      while (location.m_col < line.length())
      {
        // Check for comments at the current position
        auto comment_match = regex_match(comment_pattern, line, location.m_col);
        if (comment_match.has_value())
        {
          auto match = comment_match.value();
          if (match.str(1) != "")
          {
            tokens.push_back(LexerToken("comment", match.str(1), location, match.length()));
            location.m_col += comment_match.value().length();
            continue;
          }
        }

        // Check for whitespaces at the current position
        auto whitespace_match = regex_match(whitespace_pattern, line, location.m_col);
        if (whitespace_match.has_value())
        {
          auto match = whitespace_match.value();
          location.m_col += match.length();
          continue;
        }

        // Iterate over the rules to see if they match
        std::vector<LexerToken> matched_tokens;
        for (int i = 0; i < rules.size(); i ++)
        {
          auto rule = rules[i];
          auto rule_match = regex_match(rule.m_pattern, line, location.m_col);
          if (rule_match.has_value())
          {
            auto match = rule_match.value();
            matched_tokens.push_back(LexerToken(rule.m_name, rule.m_replacement(match), location, match.length()));
          }
        }

        // If there are no matched tokens, then we've encountered a syntax Error
        if (matched_tokens.empty())
          throw LexerError(fmt::format("Invalid character '{}'", line.substr(location.m_col, 1)), location);

        // if there is more than one matched token, then sort the current matched tokens
        if (matched_tokens.size() > 1)
        {
          std::sort(matched_tokens.begin(), matched_tokens.end(), [](LexerToken a, LexerToken b)->int {
            if (a.m_location.m_line != b.m_location.m_line)
              return a.m_location.m_line - b.m_location.m_line;
            else if (a.m_location.m_col != b.m_location.m_col)
              return a.m_location.m_col - b.m_location.m_col;
            else if (a.m_length != b.m_length)
              return b.m_length - a.m_length;
            else
              return 0;
          });
        }

        // Add the first token and increase the position to past the token
        auto token = *matched_tokens.begin();
        tokens.push_back(token);
        location.m_col += token.m_length;
      }

      // Add a newline at the end of the line and update the location to the next line
      tokens.push_back(LexerToken("newline", location));
      location.m_line ++;
      location.m_col = 0;
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
}
