#pragma once

#include <dauw/common.hpp>

#include <pcre2.h>


namespace dauw::utils
{
  // Forward declarations
  class Regex;
  class RegexMatch;
  class RegexGroup;
  class RegexException;


  // Enum that defines flags for a regular expression
  enum RegexFlags : int
  {
    REGEX_NONE            = 0,
    REGEX_ASCII           = 1 << 1,   // Make \d, \w, etc. perform ASCII-only matching instead of full Unicode matching.
    REGEX_IGNORECASE      = 1 << 2,   // Perform case-insensitive matching; expressions like [A-Z] will also match lowercase letters.
    REGEX_MULTILINE       = 1 << 3,   // Make the '^' character match at the beginning of each line and the '$' character match at the end of each line.
    REGEX_DOTALL          = 1 << 4,   // Make the '.' character match any character at all, including a newline; without this flag, '.' will match anything except a newline.
    REGEX_EXTENDED        = 1 << 5,   // Ignore whitespace and comments preceded by '#'.
  };

  // Enum that defines flags for a regular expression match
  enum RegexMatchFlags : int
  {
    REGEX_MATCH_NONE      = 0,
    REGEX_MATCH_AT_BEGIN  = 1 << 1,   // Pattern can match only at the beginning of the subject string
    REGEX_MATCH_AT_END    = 1 << 2,   // Pattern can match only at the end of the subject string
  };


  // Class that defines a regular expression
  class Regex
  {
    public:
      // Type definitions for PCRE2 types
      using char_type = uint8_t;
      using options_type = uint32_t;
      using pattern_type = pcre2_code*;
      using match_type = pcre2_match_data*;


    private:
      // The pattern of the regular expression
      string_t pattern_;

      // The named capture groups of the regular expression mapped to the corresponding numbered group
      std::unordered_map<string_t, size_t> indexes_;

      // The compiled PCRE2 pattern of the regular expression
      pattern_type code_;


      // Convert pattern and match flags to PCRE2 options
      static options_type pcre2_options_(RegexFlags pattern_flags);
      static options_type pcre2_options_(RegexMatchFlags match_flags);

      // Convert a PCRE2 error code to a string
      static string_t pcre2_error_(int error_code);


    public:
      // Constructor
      Regex(string_t pattern, RegexFlags flags = REGEX_NONE);

      // Copy constructor
      Regex(const Regex& other);

      // Destructor
      ~Regex();

      // Return the pattern of the regular expression
      string_t pattern();

      // Return the index of the numbered group corresponding to a named group
      size_t index(string_t name);

      // Search for the regular expression in a string
      RegexMatch match(string_t subject, size_t pos = 0, RegexMatchFlags flags = REGEX_MATCH_NONE);

      // Search for all non-overlapping occurrences of the regular expression in a string
      std::vector<RegexMatch> match_all(string_t subject, size_t pos = 0);

      // Substitute all non-overlapping occurrences of the regular expression in a string
      string_t substitute(string_t subject, string_t replacement);

      // Split a string by the occurrences of the regular expression
      std::vector<string_t> split(string_t subject);
  };


  // Class that defines a capture group in a regular expression match
  class RegexGroup
  {
    private:
      // The regular expression of the capture group
      Regex* pattern_;

      // The subject of the capture group
      string_t subject_;

      // The start offset of the substring matched by the capture group
      size_t start_;

      // The end offset of the substring matched by the capture group
      size_t end_;


    public:
      // Constructor
      RegexGroup(Regex* pattern, string_t subject, size_t start, size_t end);

      // Return the regular expression of the capture group
      Regex* pattern();

      // Return the subject of the capture group
      string_t subject();

      // Return if the capture group has matched anything
      bool success();

      // Return the properties of the substring matched by the capture group
      size_t start();
      size_t end();
      size_t length();
      string_t value();
  };


  // Class that defines a regular expression match
  class RegexMatch
  {
    private:
      // The regular expression of the regular expression match
      Regex* pattern_;

      // The subject of the regular expression match
      string_t subject_;

      // The capture groups of the regular expression match
      std::vector<RegexGroup> groups_;


    public:
      // Constructor
      RegexMatch(Regex* pattern, string_t subject, std::vector<RegexGroup> groups);

      // Return the regular expression of the regular expression match
      Regex* pattern();

      // Return the subject of the regular expression match
      string_t subject();

      // Return if the regular expression match has matched anything
      bool success();

      // Return a capture group of the regular expression match
      RegexGroup group(int index = 0);
      RegexGroup group(string_t name);

      // Return the prefix and suffix of the regular expression match
      RegexGroup prefix();
      RegexGroup suffix();

      // Return the properties of substring matched by the regular expression match
      inline size_t start() { return group().start(); }
      inline size_t end() { return group().end(); }
      inline size_t length() { return group().length(); }
      inline string_t value() { return group().value(); }
  };


  // Exception thrown when processing a regular expression fails
  class RegexException : public Exception
  {
    public:
      inline RegexException(string_t message, Exception* previous) : Exception(message, previous) {}
      inline RegexException(string_t message) : Exception(message, nullptr) {}
  };
}
