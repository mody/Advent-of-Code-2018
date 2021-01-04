#include <algorithm>
#include <array>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <regex>
#include <set>
#include <vector>

using Registers = std::array<unsigned long, 4>;
using Instruction = std::function<void(unsigned A, unsigned B, unsigned C)>;

struct CPU {
    CPU() = default;
    CPU(Registers _regs) : regs{_regs} {}

    Registers regs{};

    std::map<unsigned, Instruction> instructions {
        { 100 , /* addr */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] + regs[B]; } },
        { 101 , /* addi */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] + B; } },
        { 102 , /* mulr */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] * regs[B]; } },
        { 103 , /* muli */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] * B; } },
        { 104 , /* banr */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] & regs[B]; } },
        { 105 , /* bani */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] & B; } },
        { 106 , /* borr */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] | regs[B]; } },
        { 107 , /* bori */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] | B; } },
        { 108 , /* setr */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A]; } },
        { 109 , /* seti */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = A; } },
        { 110, /* gtir */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = A > regs[B]; } },
        { 111, /* gtri */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] > B; } },
        { 112, /* gtrr */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] > regs[B]; } },
        { 113, /* eqir */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = A == regs[B]; } },
        { 114, /* eqri */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] == B; } },
        { 115, /* eqrr */ [this](unsigned A, unsigned B, unsigned C) { regs[C] = regs[A] == regs[B]; } }
        };
};

std::vector<unsigned> parse_array(std::string const& line) {
    std::vector<unsigned> result;

    std::stringstream ss{line};
    unsigned i = 0;
    ss >> i;
    result.push_back(i);
    ss >> i;
    result.push_back(i);
    ss >> i;
    result.push_back(i);
    ss >> i;
    result.push_back(i);
    return result;
}

Registers parse_registers(std::string const& line) {
    static const std::regex RX{": +\\[(\\d+), (\\d+), (\\d+), (\\d+)\\]$"};
    std::smatch m;
    if (!std::regex_search(line, m, RX)) {
        std::cout << "Invalid input >" << line << "<\n";
        exit(1);
    }
    assert(m.size() == 5);
    Registers regs;
    regs[0] = std::stol(m.str(1));
    regs[1] = std::stol(m.str(2));
    regs[2] = std::stol(m.str(3));
    regs[3] = std::stol(m.str(4));
    return regs;
}

int main()
{
    unsigned result1 = 0;

    std::map<unsigned, std::set<unsigned>> instr_map;

    std::string line;
    while ( std::getline(std::cin, line) ) {
        if ( line.empty() ) {
            break;
        }

        if (line.substr(0, 6) == "Before") {
            const auto before = parse_registers(line);
            std::getline(std::cin, line);
            const auto cmd = parse_array(line);
            std::getline(std::cin, line);
            const auto after = parse_registers(line);
            std::getline(std::cin, line);

            unsigned cnt = 0;

            CPU cpu;
            for (auto& [k, i] : cpu.instructions) {
                cpu.regs = before;
                i(cmd[1], cmd[2], cmd[3]);
                if (cpu.regs == after) {
                    ++cnt;
                    instr_map.insert({cmd[0], {}}).first->second.insert(k);
                }
            }
            if (cnt > 2) {
                ++result1;
            }
        }
    }

    std::cout << "1 Result=" << result1 << "\n";

    CPU result2;

    for (;!instr_map.empty();) {
        for (auto& [id, s] : instr_map) {
            if (s.size() == 1) {
                unsigned cmd = *s.begin();
                result2.instructions[id] = result2.instructions.at(cmd);
                result2.instructions.erase(cmd);
                for (auto& [id_, s_] : instr_map) {
                    s_.erase(cmd);
                }
                break;
            }
        }
        for (auto it = instr_map.begin(); it != instr_map.end();) {
            if (it->second.size() == 0) {
                it = instr_map.erase(it);
            } else {
                ++it;
            }
        }
    }

    while ( std::getline(std::cin, line) ) {
        if ( line.empty() ) {
            continue;
        }

        const auto cmd = parse_array(line);
        result2.instructions.at(cmd.at(0))(cmd.at(1), cmd.at(2), cmd.at(3));
    }

    std::cout << "2 Result=" << result2.regs[0] << "\n";

    return 0;
}
