#include "source.hpp"

namespace dauw::frontend
{
  // Constructor for a source file
  Source::Source(string_t file, string_t source)
    : file_(file), source_(source)
  {
  }

  // Return the file the source is contained in
  string_t& Source::file()
  {
    return file_;
  }

  // Return the actual source
  string_t& Source::source()
  {
    return source_;
  }

  // Format a location in the source
  string_t Source::format(Location location)
  {
    auto lines = utils::string::split_lines(source_);
    if (location.line() >= lines.size())
      throw SourceException(fmt::format("line {} is not present in {}", location.line() + 1, file_));

    auto line = lines[location.line()];
    if (location.col() > line.length())
      throw SourceException(fmt::format("line {}, col {} is not present in {}", location.line() + 1, location.col() + 1, file_));

    string_t format;
    format.append(fmt::format("{}, {}\n", file_, location));
    format.append(fmt::format("{:>4d} │ {}\n", location.line() + 1, lines[location.line()]));
    format.append(fmt::format("     │ {}^", utils::repeat(" ", location.col())));
    return format;
  }

  // Read a source from a file in the file system
  Source* Source::read(string_t file)
  {
    if (!std::filesystem::exists(file))
      throw SourceException(fmt::format("The file '{}' does not exist", file));

    auto stream = std::ifstream(file);
    auto source = string_t(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    return new Source(std::filesystem::canonical(file), source);
  }
}
