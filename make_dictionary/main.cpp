#include "bip39/bip39.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <array>
#include <cstring>

#ifdef __cpp_lib_filesystem

#include <filesystem>
namespace fs = std::filesystem;

#else

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#endif

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Missing word list directory as first argument" << std::endl;
        return 1;
    }
    const fs::path word_lists_dir(argv[1]);
    if (argc < 3) {
        std::cout << "Missing code generation output directory as second argument" << std::endl;
        return 1;
    }
    const fs::path code_gen_dir(argv[2]);
    fs::create_directories(code_gen_dir);

    static const std::map<const char* const, BIP39::language> language_map = {
        { "english", BIP39::language::en },
        { "spanish", BIP39::language::es },
        { "japanese", BIP39::language::ja },
        { "italian", BIP39::language::it },
        { "french", BIP39::language::fr },
        { "korean", BIP39::language::ko },
        { "chinese_simplified", BIP39::language::zh_Hans, },
        { "chinese_traditional", BIP39::language::zh_Hant, }
    };

    // read the dictionary files into memory
    std::map<BIP39::language, std::array<std::string, BIP39::NUM_BIP39_WORDS>> word_lists;
    for (const auto& file : fs::directory_iterator(word_lists_dir)) {
        auto it = std::find_if(language_map.begin(), language_map.end(), [&file] (const auto& l) {
            return strcmp(l.first, file.path().stem().string().c_str()) == 0;
        });
        if (it == language_map.end()) {
            std::cout << "Unsupported language '" << file.path().stem() << "'" << std::endl;
            return 2;
        }
        const auto lang = it->second;
        std::ifstream file_stream(file.path().string());
        for (auto i = 0; file_stream && i < BIP39::NUM_BIP39_WORDS; ++i) {
            file_stream >> word_lists[lang][i];
        }
    }

    // process dictionaries and generate code
    for (const auto& lang : word_lists) {
        auto it = std::find_if(language_map.begin(), language_map.end(), [lang](const auto& l) {
            return lang.first == l.second;
        });
        if (it == language_map.end()) {
            std::cout << "Language not found in map" << std::endl;
            return 3;
        }
        // generate the literal strings for the words
        fs::path lang_str_h(code_gen_dir / fs::path(std::string(it->first) + "_str.h"));
        std::cout << "Writing file " << lang_str_h.string() << std::endl;
        std::ofstream lang_str_h_stream(lang_str_h.string());
        auto upper_filename = lang_str_h.filename().string();
        std::transform(upper_filename.begin(), upper_filename.end(), upper_filename.begin(), toupper);
        std::replace(upper_filename.begin(), upper_filename.end(), '.', '_');
        lang_str_h_stream << "#ifndef BIP39_" << upper_filename << '\n';
        lang_str_h_stream << "#define BIP39_" << upper_filename << '\n';
        lang_str_h_stream << '\n';
        lang_str_h_stream << "#include \"util.h\"\n";
        lang_str_h_stream << '\n';
        auto word_index = 0u;
        for (const auto& word : lang.second) {
            lang_str_h_stream << "const char " << it->first << '_' << word_index++ << "_str[] PROGMEM = u8\"" << word << "\";\n";
        }
        lang_str_h_stream << '\n';
        lang_str_h_stream << "#endif";

        // generate the string table for the words
        fs::path lang_h(code_gen_dir / fs::path(std::string(it->first) + ".h"));
        std::cout << "Writing file " << lang_h.string() << std::endl;
        std::ofstream lang_h_stream(lang_h.string());
        upper_filename = lang_h.filename().string();
        std::transform(upper_filename.begin(), upper_filename.end(), upper_filename.begin(), toupper);
        std::replace(upper_filename.begin(), upper_filename.end(), '.', '_');
        lang_h_stream << "#ifndef BIP39_" << upper_filename << '\n';
        lang_h_stream << "#define BIP39_" << upper_filename << '\n';
        lang_h_stream << '\n';
        lang_h_stream << "#include " << lang_str_h.filename() << "\n";
        lang_h_stream << '\n';
        lang_h_stream << "const char* const " << it->first << "_table[] PROGMEM = {\n";
        for (word_index = 0; word_index < BIP39::NUM_BIP39_WORDS; ++word_index) {
            lang_h_stream << "\t" << it->first << '_' << word_index << "_str,\n";
        }
        lang_h_stream << "};\n";
        lang_h_stream << '\n';
        lang_h_stream << "#endif";
    }

    return 0;
}
