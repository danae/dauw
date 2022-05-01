#include <argh.h>

#include <dauw/common.h>
#include <dauw/interpreter/interpreter.h>


// Print the header
void print_header(int argc, const char* argv[])
{
	fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"(                                                       )");
  fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"( oooooooooo.                                           )");
  fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"( `888'   `Y8b                                          )");
  fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"(  888      888  .oooo.   oooo  oooo  oooo oooo    ooo  )");
  fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"(  888      888 `P  )88b  `888  `888   `88. `88.  .8'   )");
  fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"(  888      888  .oP"888   888   888    `88..]88..8'    )");
  fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"(  888     d88' d8(  888   888   888     `888'`888'     )");
  fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"( o888bood8P'   `Y888""8o  `V88V"V8P'     `8'  `8'      )");
	fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", R"(                                                       )");

  fmt::print(fmt::emphasis::bold, "Dauw {} -- written by Danae Nova\n", DAUW_VERSION);
  fmt::print("Interpreter for the Dauw programming language\n");
  fmt::print("Documentation and source available at https://dauw.dev/docs/\n\n");
}

// Print the usage
void print_usage(int argc, const char* argv[])
{
  fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Usage");
	fmt::print("\n\n");
	fmt::print("  $ {} [options] [file]\n\n", argv[0]);

  fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Positional arguments");
	fmt::print("\n\n");
	fmt::print("  file            Evaluate the source code in the specified file. If no file\n");
	fmt::print("                  has been specified, then run the interactive prompt.\n\n");

	fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Optional arguments");
	fmt::print("\n\n");
	fmt::print("  -h, --help      Show this help message and exit.\n\n");
}

// Main function
int main(int argc, const char* argv[])
{
  // Create the interpreter
	auto interpreter = std::make_shared<dauw::Interpreter>();

	// Parse the arguments
	argh::parser cmdl;
	cmdl.add_params({"-h", "--help"});
	cmdl.add_params({"-v", "--version"});
	cmdl.parse(argc, argv, argh::parser::SINGLE_DASH_IS_MULTIFLAG);

	// Handle the parsed arguments
	if (cmdl[{"-h", "--help"}])
	{
		// Show this help message and exit
		print_header(argc, argv);
		print_usage(argc, argv);
		return 0;
	}
	else if (cmdl[{"-v", "--version"}])
	{
		// Show version information and exit
		print_header(argc, argv);
		return 0;
	}
	else if (cmdl(1))
	{
		// Evaluate the source code in the specified file
		return interpreter->interpret_from_file(cmdl(1).str());
	}
	else
	{
		// If no file has been specified, then run the interactive prompt
		print_header(argc, argv);
		fmt::print("Use CTRL+C to exit the interactive prompt\n");

		return interpreter->interpret_from_repl();
	}
}
