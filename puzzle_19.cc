#include <cassert>
#include <iostream>
#include <regex>

int main()
{
    std::string line;
    std::getline(std::cin, line);
    auto ip_pos = stol(line.substr(4));

    std::cout <<
R"(
#include <array>
#include <iostream>

using Registers = std::array<int64_t, 6>;

void addr(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] + regs[B]; }
void addi(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] + B; }
void mulr(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] * regs[B]; }
void muli(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] * B; }
void banr(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] & regs[B]; }
void bani(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] & B; }
void borr(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] | regs[B]; }
void bori(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] | B; }
void setr(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A]; }
void seti(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = A; }
void gtir(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = A > regs[B]; }
void gtri(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] > B; }
void gtrr(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] > regs[B]; }
void eqir(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = A == regs[B]; }
void eqri(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] == B; }
void eqrr(Registers& regs, int64_t A, int64_t B, int64_t C) { regs[C] = regs[A] == regs[B]; }

int64_t solution(Registers& regs) {
    for(;;) {
        switch(regs[)" << ip_pos << "]) {\n";

    unsigned l = 0;
    while ( std::getline(std::cin, line) ) {
        static const std::regex RX{"([^ ]+) (\\d+) ?(\\d+)? ?(\\d+)?$"};
        std::smatch m;
        if (!std::regex_search(line, m, RX)) {
            std::cout << "Invalid input >" << line << "<\n";
            exit(1);
        }
        assert(m.size() == 5);
        int64_t a = m.str(2).empty() ? 0 : std::stoll(m.str(2));
        int64_t b = m.str(3).empty() ? 0 : std::stoll(m.str(3));
        int64_t c = m.str(4).empty() ? 0 : std::stoll(m.str(4));
        std::cout << "case " << l << ": " << m.str(1) << "(regs, " << a << ", " << b << ", " << c << "); break;\n";
        ++l;
    }

    std::cout << R"(
        default:
            return regs[0];
        }
        ++regs[)" << ip_pos << R"(];
    }
    return -1;
}

int main() {
    Registers regs1{};
    std::cout << "1 Result=" << solution(regs1) << "\n";

    Registers regs2{1, 0, 0, 0, 0, 0};
    std::cout << "2 Result=" << solution(regs2) << "\n";

    return 0;
}
)";

    return 0;
}
