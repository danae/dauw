#pragma once

#include <dauw/common.hpp>

#include <pcre2.h>


namespace dauw::utils
{
  // Forward declarations
  class RegexPattern;
  class RegexMatch;
  class RegexGroup;


  // Type definitions for pointers
  using regex_pattern_ptr = std::shared_ptr<RegexPattern>;
  using regex_match_ptr = std::shared_ptr<RegexMatch>;

  // Type definitions for PCRE2 types
  using regex_pcre2_char_t = uint8_t;
  using regex_pcre2_flags_t = uint32_t;
  using regex_pcre2_pattern_t = pcre2_code*;
  using regex_pcre2_match_t = pcre2_match_data*;


  // Convert a PCRE2 error code to a string
  string_t regex_convert_error(int error_code);


  // Enum that defines flags for a regular expression pattern
  enum RegexPatternFlags : int
  {
    REGEX_NONE        = 0,
    REGEX_ASCII       = 1 << 1,   // Make \d, \w, etc. perform ASCII-only matching instead of full Unicode matching.
    REGEX_IGNORECASE  = 1 << 2,   // Perform case-insensitive matching; expressions like [A-Z] will also match lowercase letters.
    REGEX_MULTILINE   = 1 << 3,   // Make the '^' character match at the beginning of each line and the '$' character match at the end of each line.
    REGEX_DOTALL      = 1 << 4,   // Make the '.' character match any character at all, including a newline; without this flag, '.' will match anything except a newline.
    REGEX_EXTENDED    = 1 << 5,   // Ignore whitespace and comments preceded by '#'.
  };

  // Class that defines a regular expression pattern
  class RegexPattern
  {
    private:
      // The pattern of the regular expression
      string_t pattern_;

      // The flags of the regular expression
      RegexPatternFlags flags_;

      // The named capture groups of the regular expression mapped to the corresponding numbered group
      std::unordered_map<string_t, size_t> indexes_;

      // The PCRE2 data of the regular expression
      regex_pcre2_pattern_t pcre2_pattern_;
      regex_pcre2_flags_t pcre2_flags_;


    public:
      // Create a shared pointer to a regular expression
      static regex_pattern_ptr create(string_t pattern, RegexPatternFlags flags = REGEX_NONE);


      // Constructor
      RegexPattern(string_t pattern, RegexPatternFlags flags = REGEX_NONE);

      // Destructor
      ~RegexPattern();

      // Return the pattern of the regular expression
      string_t pattern();

      // Return the flags of the regular expression
      RegexPatternFlags flags();


      // Return the index of the numbered group corresponding to a named group
      size_t index(string_t name);

      // Search for or match the regular expression in a string
      regex_match_ptr search(string_t subject, size_t pos = 0);
      regex_match_ptr match(string_t subject, size_t pos = 0);
      regex_match_ptr fullmatch(string_t subject, size_t pos = 0);

      // Replace non-overlapping occurrences of the regular expression in a string
      string_t replace(string_t replacement, string_t subject);

      // Split a string by the occurrences of the regular expression
      std::vector<string_t> split(string_t subject);


    // Friend classes
    friend class RegexMatch;
  };


  // Class that defines a capture group in a regular expression match
  class RegexGroup
  {
    private:
      // The regular expreesion match of the capture group
      RegexMatch* match_;

      // Indicate if the rcapture group has matched anything
      bool success_;

      // The start offset of the substring matched by the capture group
      size_t start_;

      // The end offset of the substring matched by the capture group
      size_t end_;


    public:
      // Constructor
      RegexGroup(RegexMatch* match, bool success, size_t start, size_t end);

      // Return the regular expreesion match of the capture group
      RegexMatch match();

      // Return if the capture group has matched anything
      bool success();

      // Return the start offset of the substring matched by the capture group
      size_t start();

      // Return the end offset of the substring matched by the capture group
      size_t end();

      // Return the length of the substring matched by the capture group
      size_t length();

      // Return the substring matched by the capture group
      string_t value();
  };


  // Enum that defines flags for a regular expression match
  enum RegexMatchFlags : int
  {
    REGEX_MATCH_NONE          = 0,
    REGEX_MATCH_ANCHOR_BEGIN  = 1 << 1,   // Pattern can match only at the beginning of the subject string
    REGEX_MATCH_ANCHOR_END    = 1 << 2,   // Pattern can match only at the end of the subject string
  };

  // Class that defines a regular expression match
  class RegexMatch
  {
    private:
      // The regular expression of the regular expression match
      RegexPattern* pattern_;

      // The subject of the regular expression match
      string_t subject_;

      // The flags of the regular expression match
      RegexMatchFlags flags_;

      // The position where to start matching of the regular expression match
      size_t pos_;

      // Indicate if the regular expression match has matched anything
      bool success_;

      // The capture groups of the regular expression match
      std::vector<RegexGroup> groups_;

      // The PCRE2 data for the regular expression match
      regex_pcre2_match_t pcre2_match_;
      regex_pcre2_flags_t pcre2_flags_;


    public:
      // Create a shared pointer to a regular expression match
      static regex_match_ptr create(RegexPattern* pattern, string_t subject, size_t pos = 0, RegexMatchFlags flags = REGEX_MATCH_NONE);


      // Constructor
      RegexMatch(RegexPattern* pattern, string_t subject, size_t pos = 0, RegexMatchFlags flags = REGEX_MATCH_NONE);

      // Destructor
      ~RegexMatch();

      // Return the regular expression of the regular expression match
      RegexPattern pattern();

      // Return the subject of the regular expression match
      string_t subject();

      // Return the position where to start matching of the regular expression match
      size_t pos();

      // Return the flags of the regular expression match
      RegexMatchFlags flags();

      // Return if the regular expression match has matched anything
      bool success();

      // Return a capture group of the regular expression match
      RegexGroup group(int index);
      RegexGroup group(string_t name);

      // Return the start offset of the regular expression match
      inline size_t start() { return group(0).start(); }

      // Return the end offset of the regular expression match
      inline size_t end() { return group(0).end(); }

      // Return the length of the regular expression match
      inline size_t length() { return group(0).length(); }

      // Return the substring matched by the regular expression match
      inline string_t value() { return group(0).value(); }
  };


  // Exception thrown when processing a regular expression fails
  class RegexException : public Exception
  {
    public:
      inline RegexException(string_t message, Exception* previous) : Exception(message, previous) {}
      inline RegexException(string_t message) : Exception(message, nullptr) {}
  };
}
