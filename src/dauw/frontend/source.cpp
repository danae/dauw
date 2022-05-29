#include "source.hpp"

namespace dauw::frontend
{
  // Initialize the regex patterns for the source
  utils::regex_pattern_ptr Source::line_pattern_ = utils::RegexPattern::create("\\r?\\n");

  // Create a shared pointer to a source
  source_ptr Source::create(string_t file, string_t source)
  {
    return std::make_shared<Source>(file, source);
  }

  // Create a shared pointer to a source read from a file
  source_ptr Source::create_read(string_t file)
  {
    if (!std::filesystem::exists(file))
      throw SourceException(fmt::format("The file '{}' does not exist", file));

    auto stream = std::ifstream(file);
    auto source = string_t(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    return create(std::filesystem::canonical(file), source);
  }

  // Constructor for a source file
  Source::Source(string_t file, string_t source)
    : file_(file), source_(source), source_lines_(line_pattern_->split(source_))
  {
  }

  // Return the file of the source
  string_t& Source::file()
  {
    return file_;
  }

  // Return the source string of the source
  string_t& Source::source()
  {
    return source_;
  }

  // Return the lines of the source
  std::vector<string_t> Source::source_lines()
  {
    return source_lines_;
  }

  // Format a location in the source
  string_t Source::format(Location location)
  {
    if (location.line() >= source_lines_.size())
      throw SourceException(fmt::format("line {} is not present in {}", location.line() + 1, file_));

    auto line = source_lines_[location.line()];
    if (location.col() > line.length())
      throw SourceException(fmt::format("line {}, col {} is not present in {}", location.line() + 1, location.col() + 1, file_));

    string_t format;
    format.append(fmt::format("{}, {}\n", file_, location));
    format.append(fmt::format("{:>4d} │ {}\n", location.line() + 1, source_lines_[location.line()]));
    format.append(fmt::format("     │ {}^", utils::repeat(" ", location.col())));
    return format;
  }
}
