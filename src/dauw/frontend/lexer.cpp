#include "lexer.hpp"

namespace dauw
{
  // Initialize the regex patterns for the lexer
  regex_t Lexer::comment_pattern_("(?:[ \\t]*--[ \\t]*(.*))|(?:\"((?:[^\\\\\"]|\\\\.)*)\")");
  regex_t Lexer::whitespace_pattern_("[ \\t]+");
  regex_t Lexer::identifier_pattern_("[A-Za-z_][A-Za-z0-9_]*|`((?:[^\\\\`]|\\\\.)+)`");
  regex_t Lexer::int_pattern_("0[Xx][0-9A-Fa-f][0-9A-Fa-f_]*|-?(?:0|[1-9][0-9_]*)");
  regex_t Lexer::real_pattern_("-?(?:0|[1-9][0-9_]*)(?:\\.[0-9][0-9_]*(?:[Ee][+-]?(?:0|[1-9][0-9_]*))?|[Ee][+-]?(?:0|[1-9][0-9_]*))");
  regex_t Lexer::rune_pattern_("'((?:[^\\\\']|\\\\.)*)'");
  regex_t Lexer::string_pattern_("\"((?:[^\\\\\"]|\\\\.)*)\"");
  regex_t Lexer::regex_pattern_("/((?:[^\\\\/]|\\\\.)*)/[A-Za-z]*");

  // Rule functions for the lexer
  Lexer::rule_function_type Lexer::identifier_function_ = [](match_t match)->string_t {
    return match.length(1) < 0 ? match.str(1) : match.str(0);
  };

  // Initialize the rules for the lexer
  std::vector<Lexer::rule_type> Lexer::rules_({
    // Delimiters
    std::make_tuple(TokenKind::PARENTHESIS_LEFT, regex_t("\\("), rule_replacement_type()),
    std::make_tuple(TokenKind::PARENTHESIS_RIGHT, regex_t("\\)"), rule_replacement_type()),
    std::make_tuple(TokenKind::SQUARE_BRACKET_LEFT, regex_t("\\["), rule_replacement_type()),
    std::make_tuple(TokenKind::SQUARE_BRACKET_RIGHT, regex_t("\\]"), rule_replacement_type()),
    std::make_tuple(TokenKind::CURLY_BRACKET_LEFT, regex_t("\\{"), rule_replacement_type()),
    std::make_tuple(TokenKind::CURLY_BRACKET_RIGHT, regex_t("\\}"), rule_replacement_type()),

    // Symbols
    std::make_tuple(TokenKind::SYMBOL_COLON, regex_t(":"), rule_replacement_type()),
    std::make_tuple(TokenKind::SYMBOL_COMMA, regex_t(","), rule_replacement_type()),
    std::make_tuple(TokenKind::SYMBOL_DOT, regex_t("\\."), rule_replacement_type()),
    std::make_tuple(TokenKind::SYMBOL_BACKSLASH, regex_t("\\\\"), rule_replacement_type()),

    // Operators
    std::make_tuple(TokenKind::OPERATOR_MAYBE, regex_t("\\?"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_INTERSECTION, regex_t("&"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_UNION, regex_t("\\|"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_LENGTH, regex_t("#"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_STRING, regex_t("\\$"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_EXPONENT, regex_t("\\^"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_MULTIPLY, regex_t("\\*"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_DIVIDE, regex_t("/"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_QUOTIENT, regex_t("//"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_REMAINDER, regex_t("%"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_ADD, regex_t("\\+"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_SUBTRACT, regex_t("-"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_RANGE, regex_t("\\.\\."), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_COMPARE, regex_t("<=>"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_LESS, regex_t("<"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_LESS_EQUAL, regex_t("<="), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_GREATER, regex_t(">"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_GREATER_EQUAL, regex_t(">="), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_MATCH, regex_t("=~"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_NOT_MATCH, regex_t("!~"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_EQUAL, regex_t("=="), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_NOT_EQUAL, regex_t("!="), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_IDENTICAL, regex_t("==="), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_NOT_IDENTICAL, regex_t("!=="), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_LOGIC_NOT, regex_t("not"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_LOGIC_AND, regex_t("and"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_LOGIC_OR, regex_t("or"), rule_replacement_type(0)),
    std::make_tuple(TokenKind::OPERATOR_ASSIGN, regex_t("="), rule_replacement_type(0)),

    // Keywords
    std::make_tuple(TokenKind::KEYWORD_DEF, regex_t("def"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_DO, regex_t("do"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_ECHO, regex_t("echo"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_ELSE, regex_t("else"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_FALSE, regex_t("false"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_FOR, regex_t("for"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_IF, regex_t("if"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_IN, regex_t("in"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_NOTHING, regex_t("nothing"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_THEN, regex_t("then"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_TRUE, regex_t("true"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_UNTIL, regex_t("until"), rule_replacement_type()),
    std::make_tuple(TokenKind::KEYWORD_WHILE, regex_t("while"), rule_replacement_type()),

    // Identifiers
    std::make_tuple(TokenKind::IDENTIFIER, identifier_pattern_, rule_replacement_type(identifier_function_)),

    // Literals
    std::make_tuple(TokenKind::LITERAL_INT, int_pattern_, rule_replacement_type(0)),
    std::make_tuple(TokenKind::LITERAL_REAL, real_pattern_, rule_replacement_type(0)),
    std::make_tuple(TokenKind::LITERAL_RUNE, rune_pattern_, rule_replacement_type(1)),
    std::make_tuple(TokenKind::LITERAL_STRING, string_pattern_, rule_replacement_type(1)),
    std::make_tuple(TokenKind::LITERAL_REGEX, regex_pattern_, rule_replacement_type(0)),
  });


  // Constructor for the lexer
  Lexer::Lexer(string_t source, string_t source_name, ErrorReporter* reporter)
    : source_(source), source_name_(source_name), reporter_(reporter)
  {
  }

  // Convert a string into a deque of tokens
  Lexer::token_list_type Lexer::tokenize()
  {
    // Initialize a deque to store the tokens
    token_list_type tokens;

    // Initialize a stack to store the indentation levels
    std::deque<int> indents({0});

    // ReInitialize a location for the lexer
    Location location;

    // Iterate over the lines in the source
    for (auto line : dauw::common::str_split_lines(source_))
    {
      // Check for a shebang at the start of the source
      if (line.rfind("#!", 0) == 0)
      {
        // If this is the first line of the source, then ignore the shebang, otherwise report an error
        if (location.line() != 0)
        {
          reporter_->report_syntax_error(location, "Syntax error: A shebang is only allowed at the first line of the source");
          return token_list_type({});
        }

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
        {
          reporter_->report_syntax_error(location, "Syntax error: The first line of the source should never be indented");
          return token_list_type({});
        }

        // If the indent is bigger than the last indent, then add an indent token
        indents.push_back(indent);
        tokens.push_back(Token(TokenKind::INDENT, location));
      }

      while (indent < indents.back())
      {
        // If the indent is smaller than the last indent, then add as much dedent tokens until the indent matches one on the stack
        indents.pop_back();
        tokens.push_back(Token(TokenKind::DEDENT, location));
      }

     // If the indent doesn't match the current indentation level, then report an error
      if (indent != indents.back())
      {
        reporter_->report_syntax_error(location, "Syntax error: The indentation does not match any outer indentation level");
        return token_list_type({});
      }

      location.increase_col_(indent);

      // Iterate over the line
      while (location.col() < line.length())
      {
        // Check for comments at the current position
        match_t comment_match = dauw::common::regex_match(comment_pattern_, line, location.col());
        if (!comment_match.empty() && comment_match.str(1) != "")
        {
          tokens.push_back(Token(TokenKind::COMMENT, comment_match.str(1), location));
          location.increase_col_(comment_match.length());
          continue;
        }

        // Check for whitespaces at the current position
        match_t whitespace_match = dauw::common::regex_match(whitespace_pattern_, line, location.col());
        if (!whitespace_match.empty())
        {
          location.increase_col_(whitespace_match.length());
          continue;
        }

        // Iterate over the rules to see if they match
        std::vector<matched_token_type> matched_tokens;
        for (auto rule : rules_)
        {
          match_t rule_match = dauw::common::regex_match(std::get<1>(rule), line, location.col());
          if (!rule_match.empty())
          {
            auto rule_replace = std::get<2>(rule);
            string_t replacement;
            if (std::holds_alternative<int>(rule_replace))
              replacement = rule_match.str(std::get<int>(rule_replace));
            else if (std::holds_alternative<rule_function_type>(rule_replace))
              replacement = std::get<rule_function_type>(rule_replace)(rule_match);
            matched_tokens.push_back(std::make_tuple(Token(std::get<0>(rule), replacement, location), rule_match.length()));
          }
        }

        // If there are no matched tokens, then we've encountered a syntax Error
        if (matched_tokens.empty())
        {
          reporter_->report_syntax_error(location, fmt::format("Syntax error: Invalid character '{}'", line.substr(location.col(), 1)));
          return token_list_type({});
        }

        // if there is more than one matched token, then sort the current matched tokens
        if (matched_tokens.size() > 1)
        {
          std::sort(matched_tokens.begin(), matched_tokens.end(), [](matched_token_type a, matched_token_type b)->bool {
            return std::get<0>(a) < std::get<0>(b) || std::get<1>(a) > std::get<1>(b);
          });
        }

        // Add the first token and increase the position to past the token
        auto token = *matched_tokens.begin();
        tokens.push_back(std::get<0>(token));
        location.increase_col_(std::get<1>(token));
      }

      // Add a newline at the end of the line and update the location to the next line
      tokens.push_back(Token(TokenKind::NEWLINE, location));
      location.increase_line_();
    }

    // Add dedent tokens for the remaining indents
    while (indents.back() > 0)
    {
      indents.pop_back();
      tokens.push_back(Token(TokenKind::DEDENT, location));
    }

    // Add an end token
    tokens.push_back(Token(TokenKind::END, location));

    // Return the tokens
    return tokens;
  }
}
