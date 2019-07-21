#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "gen.h"

void print_ns_start(std::ostream& stream, const std::vector<std::string>& ns_vec) {
    for (const std::string& ns_part: ns_vec) {
        stream << "namespace " << ns_part << " {" << std::endl;
    }
}

void print_ns_end(std::ostream& stream, const std::vector<std::string>& ns_vec) {
    for (size_t i = 0; i < ns_vec.size(); i++) {
        stream << "}" << std::endl;
    }
}

void print_var_decl(std::ostream& stream, const std::string& var_name) {
    stream << "const unsigned char " << var_name << "[]";
}

template <typename It, typename Out>
void print_hex_array(It begin, It end, Out& stream, size_t l = 10) {
    size_t n = 0;

    stream << std::hex << std::uppercase << std::setfill('0');
    while (begin != end) {

        stream << "0x" << std::setw(2) << static_cast<unsigned>(static_cast<unsigned char>(*begin));

        ++begin;
        if (begin != end) {
            stream << ",";
        } else {
            break;
        }

        ++n;
        if (n % l == 0) {
            stream << std::endl;
        }
    }
}

bool create_parent_dir(const std::filesystem::path& p) {
    const std::filesystem::path parent = p.parent_path();
    if (parent.empty() || std::filesystem::is_directory(parent)) {
        return true;
    }
    if (std::filesystem::exists(parent)) {
        std::cout << "[ERROR] Output parent path " << parent << " already exists and isn't a directory. Generation skipped" << std::endl ;
        return false;
    }
    if (!std::filesystem::create_directories(parent)) {
        std::cout << "[ERROR] Unable to create parent output directory " << parent << ". Generation skipped" << std::endl;
        return false;
    }
    return true;
}

void generate_header(const std::filesystem::path& output_path, const target& t) {
    std::cout << "Generating header file to " << output_path << "..." << std::endl;

    std::ofstream out(output_path, std::ios_base::out | std::ios_base::trunc);
    for (const auto& ns_entry: t.inputs) {
        print_ns_start(out, ns_entry.first);
        for (const auto& i: ns_entry.second) {
            out << "extern ";
            print_var_decl(out, i.first);
            out << ";" << std::endl;
        }
        print_ns_end(out, ns_entry.first);
    }
}

void generate_cpp(const std::filesystem::path& output_path, const target& t) {
    std::cout << "Generating CPP file to " << output_path << "..." << std::endl;


    std::ofstream out(output_path, std::ios_base::out | std::ios_base::trunc);

    for (const std::filesystem::path& dep: t.dependencies) {
        out << "#include " << dep.lexically_relative(output_path.parent_path()) << std::endl;
    }

    for (const auto& ns_entry: t.inputs) {
        print_ns_start(out, ns_entry.first);
        for (const auto& i: ns_entry.second) {
            std::ifstream in(i.second, std::ios_base::in | std::ios_base::binary);
            if (!in) {
                std::cout << "[ERROR] Unable to open " << i.second << " file. Skipping..." << std::endl;
                continue;
            }

            print_var_decl(out, i.first);
            out << " = {" << std::endl;
            print_hex_array(std::istreambuf_iterator(in), std::istreambuf_iterator<char>(), out);
            out << std::endl << "};" << std::endl;
        }
        print_ns_end(out, ns_entry.first);
    }
}

void generate(const std::filesystem::path& output_path, const target& t) {
    if (!create_parent_dir(output_path)) {
        return;
    }
    switch (t.type) {
        case HEADER:
            generate_header(output_path, t);
            break;

        case CPP:
            generate_cpp(output_path, t);
            break;
    }
}

void generate_files(const target_map& targets) {
    std::cout << "Current working dir is " << std::filesystem::current_path() << std::endl;
    for (const target_map::value_type& pair: targets) {
        generate(pair.first, pair.second);
    }
}