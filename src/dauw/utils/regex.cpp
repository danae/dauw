#include "regex.hpp"

namespace dauw::utils
{
  // Convert a PCRE2 error code to a string
  string_t regex_convert_error(int error_code)
  {
    regex_pcre2_char_t buffer[1024];
    pcre2_get_error_message(error_code, buffer, sizeof(buffer));
    return string_t((char*)buffer);
  }

  // --------------------------------------------------------------------------

  // Create a shared pointer to a regular expression
  regex_pattern_ptr RegexPattern::create(string_t pattern, RegexPatternFlags flags)
  {
    return std::make_shared<RegexPattern>(pattern, flags);
  }

  // Constructor for a regular expression pattern
  RegexPattern::RegexPattern(string_t pattern, RegexPatternFlags flags)
    : pattern_(pattern), flags_(flags)
  {
    // Create the flags of the pattern
    pcre2_flags_ = PCRE2_UCP | PCRE2_UTF;
    if ((flags_ & REGEX_ASCII) != REGEX_ASCII)
      pcre2_flags_ |= PCRE2_UCP;
    if ((flags_ & REGEX_IGNORECASE) == REGEX_IGNORECASE)
      pcre2_flags_ |= PCRE2_CASELESS;
    if ((flags_ & REGEX_MULTILINE) == REGEX_MULTILINE)
      pcre2_flags_ |= PCRE2_MULTILINE;
    if ((flags_ & REGEX_DOTALL) == REGEX_DOTALL)
      pcre2_flags_ |= PCRE2_DOTALL;
    if ((flags_ & REGEX_EXTENDED) == REGEX_EXTENDED)
      pcre2_flags_ |= PCRE2_EXTENDED;

    // Create the pattern
    int result;
    size_t offset;

    pcre2_pattern_ = pcre2_compile((regex_pcre2_char_t*)pattern_.c_str(), pattern_.length(), flags, &result, &offset, nullptr);
    if (pcre2_pattern_ == nullptr)
      throw RegexException(fmt::format("Cannot compile pattern '{}' at offset {}: {}", pattern_, offset, regex_convert_error(result)));

    // Create the named group indexes
    uint32_t name_count, name_entry_size;
    regex_pcre2_char_t* name_table;

    pcre2_pattern_info(pcre2_pattern_, PCRE2_INFO_NAMECOUNT, &name_count);
    pcre2_pattern_info(pcre2_pattern_, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);
    pcre2_pattern_info(pcre2_pattern_, PCRE2_INFO_NAMETABLE, &name_table);

    regex_pcre2_char_t* name_table_ptr = name_table;
    for (auto i = 0; i < name_count; i++)
    {
      size_t group = (name_table_ptr[0] << 8) | name_table_ptr[1];
      string_t name = string_t(name_table_ptr + 2, name_table_ptr + name_entry_size);
      indexes_[name] = group;
      name_table_ptr += name_entry_size;
    }
  }

  // Destructor for a regular expression pattern
  RegexPattern::~RegexPattern()
  {
    // Free the pattern
    pcre2_code_free(pcre2_pattern_);
  }

  // Return the pattern of the regular expression
  string_t RegexPattern::pattern()
  {
    return pattern_;
  }

  // Return the flags of the regular expression
  RegexPatternFlags RegexPattern::flags()
  {
    return flags_;
  }

  // Return the index of the numbered group cossresponding to a named group
  size_t RegexPattern::index(string_t name)
  {
    std::unordered_map<string_t, size_t>::iterator it;
    if ((it = indexes_.find(name)) != indexes_.end())
      return std::get<1>(*it);
    else
      throw RegexException(fmt::format("Cannot find named capture group '{}' in pattern '{}'", name, pattern_));
  }

  // Search for the regular expression in a string
  regex_match_ptr RegexPattern::search(string_t subject, size_t pos)
  {
    return RegexMatch::create(this, subject, pos, REGEX_MATCH_NONE);
  }

  // Match the regular expression at the beginning of a string
  regex_match_ptr RegexPattern::match(string_t subject, size_t pos)
  {
    return RegexMatch::create(this, subject, pos, REGEX_MATCH_ANCHOR_BEGIN);
  }

  // Match the regular expression in a whole string
  regex_match_ptr RegexPattern::fullmatch(string_t subject, size_t pos)
  {
    return RegexMatch::create(this, subject, pos, (RegexMatchFlags)(REGEX_MATCH_ANCHOR_BEGIN | REGEX_MATCH_ANCHOR_END));
  }

  // Replace non-overlapping occurrences of the regular expression in a string
  string_t RegexPattern::replace(string_t replacement, string_t subject)
  {
    // TODO: Implement regex replace
    return "";
  }

  // Split a string by the occurrences of the regular expression
  std::vector<string_t> RegexPattern::split(string_t subject)
  {
    auto match = RegexMatch::create(this, subject, 0, REGEX_MATCH_NONE);
    if (!match->success())
      return std::vector<string_t>({subject});

    std::vector<string_t> parts;

    size_t pos = 0;
    do {
      parts.push_back(string_t(subject, pos, match->start() - pos));
      pos = match->end();
      match = RegexMatch::create(this, subject, pos, REGEX_MATCH_NONE);
    } while (match->success());

    return parts;
  }

  // --------------------------------------------------------------------------

  // Constructor
  RegexGroup::RegexGroup(RegexMatch* match, bool success, size_t start, size_t end)
    : match_(match), success_(success), start_(start), end_(end)
  {
  }

  // Return the regular expreesion match of the capture group
  RegexMatch RegexGroup::match()
  {
    return *match_;
  }

  // Return if the capture group has matched anything
  bool RegexGroup::success()
  {
    return success_;
  }

  // Return the start offset of the substring matched by the capture group
  size_t RegexGroup::start()
  {
    return success_ ? start_ : 0;
  }

  // Return the end offset of the substring matched by the capture group
  size_t RegexGroup::end()
  {
    return success_ ? end_ : 0;
  }

  // Return the length of the substring matched by the capture group
  size_t RegexGroup::length()
  {
    return success_ ? (end_ - start_) : 0;
  }

  // Return the substring matched by a capture group
  string_t RegexGroup::value()
  {
    return success_ ? string_t(match_->subject(), start_, end_ - start_) : "";
  }

  // --------------------------------------------------------------------------

  // Create a shared pointer to a regular expression match
  regex_match_ptr RegexMatch::create(RegexPattern* pattern, string_t subject, size_t pos, RegexMatchFlags flags)
  {
    return std::make_shared<RegexMatch>(pattern, subject, pos, flags);
  }

  // Constructor for a regular expression match
  RegexMatch::RegexMatch(RegexPattern* pattern, string_t subject, size_t pos, RegexMatchFlags flags)
    : pattern_(pattern), subject_(subject), pos_(pos), flags_(flags), success_(false)
  {
    // Create the flags
    pcre2_flags_ = 0;
    if ((flags_ & REGEX_MATCH_ANCHOR_BEGIN) == REGEX_MATCH_ANCHOR_BEGIN)
      pcre2_flags_ |= PCRE2_ANCHORED;
    if ((flags_ & REGEX_MATCH_ANCHOR_END) == REGEX_MATCH_ANCHOR_END)
      pcre2_flags_ |= PCRE2_ENDANCHORED;

    // Create the match
    pcre2_match_ = pcre2_match_data_create_from_pattern(pattern_->pcre2_pattern_, nullptr);
    int result = pcre2_match(pattern_->pcre2_pattern_, (regex_pcre2_char_t*)subject_.c_str(), subject_.length(), pos_, pcre2_flags_, pcre2_match_, nullptr);
    if (result < 0 && result != PCRE2_ERROR_NOMATCH)
      throw RegexException(fmt::format("Cannot match pattern: {}", regex_convert_error(result)));

    // Create the groups of the match
    if (result != PCRE2_ERROR_NOMATCH)
    {
      success_ = true;

      auto ovector = pcre2_get_ovector_pointer(pcre2_match_);
      for (auto i = 0; i < result; i ++)
      {
        size_t start = ovector[2 * i];
        size_t end = ovector[2 * i + 1];
        bool success = start != PCRE2_UNSET && end != PCRE2_UNSET;
        groups_.push_back(RegexGroup(this, success, start, end));
      }
    }
  }

  // Destructor for a regular expression match
  RegexMatch::~RegexMatch()
  {
    // Free the match data
    pcre2_match_data_free(pcre2_match_);
  }

  // Return the rregular expression of the regular expression match
  RegexPattern RegexMatch::pattern()
  {
    return *pattern_;
  }

  // Return the subject of the regular expression match
  string_t RegexMatch::subject()
  {
    return subject_;
  }

  // Return the position where to start matching of the regular expression match
  size_t RegexMatch::pos()
  {
    return pos_;
  }

  // Return the flags of the regular expression match
  RegexMatchFlags RegexMatch::flags()
  {
    return flags_;
  }

  // Return if the regular expression match has matched anything
  bool RegexMatch::success()
  {
    return success_;
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
}
