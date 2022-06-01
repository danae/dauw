#include "regex.hpp"

namespace dauw::utils
{
  // Convert pattern flags to PCRE2 options
  Regex::options_type Regex::pcre2_options_(RegexFlags pattern_flags)
  {
    options_type flags = PCRE2_UCP | PCRE2_UTF;

    if ((pattern_flags & REGEX_ASCII) != REGEX_ASCII)
      flags |= PCRE2_UCP;
    if ((pattern_flags & REGEX_IGNORECASE) == REGEX_IGNORECASE)
      flags |= PCRE2_CASELESS;
    if ((pattern_flags & REGEX_MULTILINE) == REGEX_MULTILINE)
      flags |= PCRE2_MULTILINE;
    if ((pattern_flags & REGEX_DOTALL) == REGEX_DOTALL)
      flags |= PCRE2_DOTALL;
    if ((pattern_flags & REGEX_EXTENDED) == REGEX_EXTENDED)
      flags |= PCRE2_EXTENDED;

    return flags;
  }

  // Convert match flags to PCRE2 options
  Regex::options_type Regex::pcre2_options_(RegexMatchFlags match_flags)
  {
    options_type flags = 0;

    if ((match_flags & REGEX_MATCH_AT_BEGIN) == REGEX_MATCH_AT_BEGIN)
      flags |= PCRE2_ANCHORED;
    if ((match_flags & REGEX_MATCH_AT_END) == REGEX_MATCH_AT_END)
      flags |= PCRE2_ENDANCHORED;

    return flags;
  }

  // Convert a PCRE2 error code to a string
  string_t Regex::pcre2_error_(int error_code)
  {
    char_type buffer[1024];
    pcre2_get_error_message(error_code, buffer, sizeof(buffer));
    return string_t((char*)buffer);
  }

  // Constructor for a regular expression
  Regex::Regex(string_t pattern, RegexFlags flags)
    : pattern_(pattern)
  {
    // Create the pattern
    auto p_str = (char_type*)pattern_.c_str();
    auto p_length = pattern_.length();
    auto options = pcre2_options_(flags);

    int result;
    size_t offset;
    code_ = pcre2_compile(p_str, p_length, options, &result, &offset, nullptr);
    if (code_ == nullptr)
      throw RegexException(fmt::format("Cannot compile pattern '{}' at offset {}: {}", pattern_, offset, pcre2_error_(result)));

    // Create the named groups index of the pattern
    uint32_t name_count, name_entry_size;
    char_type* name_table;

    pcre2_pattern_info(code_, PCRE2_INFO_NAMECOUNT, &name_count);
    pcre2_pattern_info(code_, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);
    pcre2_pattern_info(code_, PCRE2_INFO_NAMETABLE, &name_table);

    char_type* name_table_ptr = name_table;
    for (auto i = 0; i < name_count; i++)
    {
      size_t group = (name_table_ptr[0] << 8) | name_table_ptr[1];
      string_t name = string_t(name_table_ptr + 2, name_table_ptr + name_entry_size);
      indexes_[name] = group;
      name_table_ptr += name_entry_size;
    }
  }

  // Copy constructor for a regular expression
  Regex::Regex(const Regex& other)
    : pattern_(other.pattern_)
  {
    // Copy the pattern
    code_ = pcre2_code_copy(other.code_);
    if (code_ == nullptr)
      throw RegexException(fmt::format("Cannot copy pattern '{}'", pattern_));
  }

  // Destructor for a regular expression
  Regex::~Regex()
  {
    pcre2_code_free(code_);
  }

  // Return the pattern of the regular expression
  string_t Regex::pattern()
  {
    return pattern_;
  }

  // Return the index of the numbered group cossresponding to a named group
  size_t Regex::index(string_t name)
  {
    std::unordered_map<string_t, size_t>::iterator it;
    if ((it = indexes_.find(name)) != indexes_.end())
      return std::get<1>(*it);
    else
      throw RegexException(fmt::format("Cannot find named capture group '{}' in pattern '{}'", name, pattern_));
  }

  // Search for the regular expression in a string
  RegexMatch Regex::match(string_t subject, size_t pos, RegexMatchFlags flags)
  {
    // Perform the match
    auto s_str = (char_type*)subject.c_str();
    auto s_length = subject.length();
    auto options = pcre2_options_(flags);

    auto match = pcre2_match_data_create_from_pattern(code_, nullptr);
    int error = pcre2_match(code_, s_str, s_length, pos, options, match, nullptr);
    if (error < 0)
    {
      // Throw an error in any case except having found no match
      if (error != PCRE2_ERROR_NOMATCH)
        throw RegexException(fmt::format("Cannot match pattern: {}", pcre2_error_(error)));
    }

    // Fetch the groups of the match
    std::vector<RegexGroup> groups;
    if (error != PCRE2_ERROR_NOMATCH)
    {
      auto ovector = pcre2_get_ovector_pointer(match);
      for (auto i = 0; i < error; i ++)
      {
        size_t start = ovector[2 * i];
        size_t end = ovector[2 * i + 1];
        bool success = start != PCRE2_UNSET && end != PCRE2_UNSET;
        groups.push_back(RegexGroup(this, subject, start, end));
      }
    }

    // Create and return the result
    RegexMatch result(this, subject, groups);
    pcre2_match_data_free(match);
    return result;
  }

  // Search for all non-overlapping occurrences of the regular expression in a string
  std::vector<RegexMatch> Regex::match_all(string_t subject, size_t pos)
  {
    std::vector<RegexMatch> matches;

    auto m = match(subject, pos);
    while (m.success())
    {
      matches.push_back(m);
      pos = m.end();
      m = match(subject, pos);
    }

    return matches;
  }

  // Substitute all non-overlapping occurrences of the regular expression in a string
  string_t Regex::substitute(string_t subject, string_t replacement)
  {
    // Perform the substitution
    auto s_str = (char_type*)subject.c_str();
    auto s_length = subject.length();
    auto r_str = (char_type*)replacement.c_str();
    auto r_length = replacement.length();
    auto options = PCRE2_SUBSTITUTE_GLOBAL | PCRE2_SUBSTITUTE_OVERFLOW_LENGTH;

    size_t output_length = 255;
    char_type* output_str = new char_type[output_length + 1];

    int error = pcre2_substitute(code_, s_str, s_length, 0, options, nullptr, nullptr, r_str, r_length, output_str, &output_length);
    if (error < 0)
    {
      // Throw an error in any case except being out of memoty
      if (error != PCRE2_ERROR_NOMEMORY)
        throw RegexException(fmt::format("Cannot perform substitution in pattern '{}': {}", pattern_, pcre2_error_(error)));

      // If out of memory, perform the subtitution again with the newly assigned output size
      delete[] output_str;
      output_str = new char_type[output_length + 1];

      error = pcre2_substitute(code_, s_str, s_length, 0, options, nullptr, nullptr, r_str, r_length, output_str, &output_length);
      if (error < 0)
        throw RegexException(fmt::format("Cannot perform substitution in pattern '{}': {}", pattern_, pcre2_error_(error)));
    }

    // Create and return the result
    string_t result((char*)output_str, (char*)output_str + output_length);
    delete[] output_str;
    return result;
  }

  // Split a string by the occurrences of the regular expression
  std::vector<string_t> Regex::split(string_t subject)
  {
    auto m = match(subject);
    if (!m.success())
      return std::vector<string_t>({subject});

    std::vector<string_t> parts;

    size_t pos = 0;
    while (m.success())
    {
      parts.push_back(string_t(subject, pos, m.start() - pos));
      pos = m.end();
      m = match(subject, pos);
    }

    return parts;
  }

  // --------------------------------------------------------------------------

  // Constructor
  RegexGroup::RegexGroup(Regex* pattern, string_t subject, size_t start, size_t end)
    : pattern_(pattern), subject_(subject), start_(start), end_(end)
  {
  }

  // Return the rregular expression of the capture group
  Regex* RegexGroup::pattern()
  {
    return pattern_;
  }

  // Return the subject of the capture group
  string_t RegexGroup::subject()
  {
    return subject_;
  }

  // Return if the capture group has matched anything
  bool RegexGroup::success()
  {
    return start_ != PCRE2_UNSET && end_ != PCRE2_UNSET;
  }

  // Return the start offset of the substring matched by the capture group
  size_t RegexGroup::start()
  {
    return start_;
  }

  // Return the end offset of the substring matched by the capture group
  size_t RegexGroup::end()
  {
    return end_;
  }

  // Return the length of the substring matched by the capture group
  size_t RegexGroup::length()
  {
    return end_ - start_;
  }

  // Return the substring matched by a capture group
  string_t RegexGroup::value()
  {
    return success() ? string_t(subject_, start_, end_ - start_) : "";
  }

  // --------------------------------------------------------------------------

  // Constructor for a regular expression match
  RegexMatch::RegexMatch(Regex* pattern, string_t subject, std::vector<RegexGroup> groups)
    : pattern_(pattern), subject_(subject), groups_(groups)
  {
  }

  // Return the rregular expression of the regular expression match
  Regex* RegexMatch::pattern()
  {
    return pattern_;
  }

  // Return the subject of the regular expression match
  string_t RegexMatch::subject()
  {
    return subject_;
  }

  // Return if the regular expression match has matched anything
  bool RegexMatch::success()
  {
    return groups_.size() > 0;
  }

  // Return a numbered capture group of the regular expression match
  RegexGroup RegexMatch::group(int index)
  {
    if (index < groups_.size())
      return groups_[index];
    else
      throw RegexException(fmt::format("Cannot find capture group {} in pattern", index));
  }

  // Return a named capture group of the regular expression match
  RegexGroup RegexMatch::group(string_t name)
  {
    return group(pattern_->index(name));
  }

  // Return the prefix of the regular expression match
  RegexGroup RegexMatch::prefix()
  {
    if (success())
      return RegexGroup(pattern_, subject_, 0, group(0).start());
    else
      return RegexGroup(pattern_, subject_, PCRE2_UNSET, PCRE2_UNSET);
  }

  // Return the prefix of the regular expression match
  RegexGroup RegexMatch::suffix()
  {
    if (success())
      return RegexGroup(pattern_, subject_, group(0).end(), subject_.length());
    else
      return RegexGroup(pattern_, subject_, PCRE2_UNSET, PCRE2_UNSET);
  }
}
