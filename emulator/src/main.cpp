#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <core.h>
#include <exception>
#include <iostream>
#include <rom.h>
#include <stdexcept>

namespace po = boost::program_options;

po::options_description main_opts("Main options");
po::options_description hidden_opts("Hidden options");

void show_help(char *prog_name) {
  std::cerr << "Usage: " << prog_name << " <rom>\n\n";
  std::cerr << main_opts;
}

int main(int argc, char **argv) {
  std::string rom = "";

  main_opts.add_options()("help,h", "shows this message");
  hidden_opts.add_options()("rom", po::value<std::string>(&rom),
                            "input ROM file");

  // do all the magic shit with boost
  po::options_description all_opts;
  all_opts.add(main_opts).add(hidden_opts);

  po::positional_options_description p;
  p.add("rom", 1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv).options(all_opts).positional(p).run(),
      vm);
  po::notify(vm);

  // if --help, show the help
  if (vm.count("help")) {
    show_help(argv[0]);
    return 0;
  }

  // if no rom, show the help and an error
  if (!vm.count("rom")) {
    std::cerr << "No ROM file was specified.\n";
    show_help(argv[0]);
    return 1;
  }

  omam8::Core::init();

  try {
    omam8::Core::load_rom(omam8::ROM::get_rom(rom));
  } catch (std::logic_error exc) {
    std::cerr << "Failed to load ROM: " << exc.what() << "\n";
    return 1;
  }

  try {
    omam8::Core::start_loop();
  } catch (std::logic_error exc) {
    std::cerr << "Execution failed: " << exc.what() << "\n";
    return 1;
  }
  return 0;
}
