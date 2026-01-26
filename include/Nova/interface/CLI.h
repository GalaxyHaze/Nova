#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <CLI/CLI.hpp>
#include "../core.h" // assumindo que 'version' está definido aqui

namespace Nova {

    enum class BuildMode {
        debug,
        dev,
        release,
        fast,
        test
    };

    inline BuildMode string_to_build_mode(std::string_view mode_str) {
        if (mode_str == "debug")   return BuildMode::debug;
        if (mode_str == "dev")     return BuildMode::dev;
        if (mode_str == "release") return BuildMode::release;
        if (mode_str == "fast")    return BuildMode::fast;
        if (mode_str == "test")    return BuildMode::test;

        throw CLI::ValidationError("Invalid build mode: must be one of {debug, dev, release, fast, test}");
    }

    struct Options {
        std::string input_file;
        BuildMode mode = BuildMode::debug;
        bool show_version = false;
    };

    class CoreInterfaceCommand {
    public:
        CoreInterfaceCommand() = delete;
        CoreInterfaceCommand(const CoreInterfaceCommand&) = delete;
        CoreInterfaceCommand& operator=(const CoreInterfaceCommand&) = delete;

        static Options parse(const int argc, const char** argv) {
            CLI::App app{"Nova - A low-level general-purpose language"};

            std::string input_file;
            std::string mode_str = "debug"; // padrão
            bool show_version = false;

            // Input file é opcional apenas se --version for usado
            auto* input_opt = app.add_option("input", input_file, "Input source file (.nv)")
                                 ->check(CLI::ExistingFile);

            app.add_option("-m,--mode", mode_str,
                "Build mode: debug, dev, release, fast, test (default: debug)")
               ->transform(CLI::IsMember({"debug", "dev", "release", "fast", "test"}));

            app.add_flag("--version", show_version, "Show version and exit");

            try {
                app.parse(argc, argv);
            } catch (const CLI::ParseError& e) {
                std::exit(app.exit(e));
            }

            if (show_version) {
                std::cout << "Nova version: " << version << '\n';
                std::exit(0);
            }

            // Se não pediu versão, exige arquivo de entrada
            if (input_file.empty()) {
                std::cerr << "Error: No input file provided.\n";
                std::cerr << "Usage: " << (argv[0] ? argv[0] : "nova") << " [OPTIONS] <input_file>\n";
                std::cerr << "Use --help for more information.\n";
                std::exit(1);
            }

            BuildMode mode = string_to_build_mode(mode_str);

            return Options{
                .input_file = std::move(input_file),
                .mode = mode,
                .show_version = false
            };
        }
    };

} // namespace Nova