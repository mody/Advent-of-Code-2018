#include <array>
#include <iomanip>
#include <iostream>
#include <unordered_set>

using RegType = uint64_t;

using Registers = std::array<RegType, 6>;

void addr(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] + regs[B]; }
void addi(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] + B; }
void mulr(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] * regs[B]; }
void muli(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] * B; }
void banr(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] & regs[B]; }
void bani(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] & B; }
void borr(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] | regs[B]; }
void bori(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] | B; }
void setr(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A]; }
void seti(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = A; }
void gtir(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = A > regs[B]; }
void gtri(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] > B; }
void gtrr(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] > regs[B]; }
void eqir(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = A == regs[B]; }
void eqri(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] == B; }
void eqrr(Registers& regs, RegType A, RegType B, RegType C) { regs[C] = regs[A] == regs[B]; }

RegType solution(Registers& regs) {
    RegType last_r5 = 0;
    std::unordered_set<RegType> r5;

    for(;;) {
        // std::cout << std::setw(5) << regs[1] << ": ";
        // for (auto const& x : regs) {
        //     std::cout << std::setw(10) << x << ", ";
        // }
        // std::cout << "\n";
        switch(regs[1]) {
        case 0: seti(regs, 123, 0, 5); break;
        case 1: bani(regs, 5, 456, 5); break;
        case 2: eqri(regs, 5, 72, 5); break;
        case 3: addr(regs, 5, 1, 1); break;
        case 4: seti(regs, 0, 0, 1); break;
        case 5: seti(regs, 0, 4, 5); break;
        case 6: bori(regs, 5, 65536, 3); break;
        case 7: seti(regs, 521363, 7, 5); break;
        case 8: bani(regs, 3, 255, 4); break;
        case 9: addr(regs, 5, 4, 5); break;
        case 10: bani(regs, 5, 16777215, 5); break;
        case 11: muli(regs, 5, 65899, 5); break;
        case 12: bani(regs, 5, 16777215, 5); break;
        case 13: gtir(regs, 256, 3, 4); break;
        case 14: addr(regs, 4, 1, 1); break;
        case 15: addi(regs, 1, 1, 1); break;
        case 16: seti(regs, 27, 1, 1); break;
        case 17: seti(regs, 0, 2, 4); break;
        case 18: addi(regs, 4, 1, 2); break;
        case 19: muli(regs, 2, 256, 2); break;
        case 20: gtrr(regs, 2, 3, 2); break;
        case 21: addr(regs, 2, 1, 1); break;
        case 22: addi(regs, 1, 1, 1); break;
        case 23: seti(regs, 25, 2, 1); break;
        case 24: addi(regs, 4, 1, 4); break;
        case 25: seti(regs, 17, 3, 1); break;
        case 26: setr(regs, 4, 2, 3); break;
        case 27: seti(regs, 7, 1, 1); break;
        case 28: {
                 if (!r5.insert(regs[5]).second) {
                     return last_r5;
                 }
                 last_r5 = regs[5];
                 eqrr(regs, 5, 0, 4);
                 break;
                 }
        case 29: addr(regs, 4, 1, 1); break;
        case 30: seti(regs, 5, 8, 1); break;

        default:
            return last_r5;
        }
        ++regs[1];
    }
    return -1;
}

int main() {
    Registers regs2{0, 0, 0, 0, 0, 0};
    auto result2 = solution(regs2);
    std::cout << "2 Result=" << result2 << "\n";

    return 0;
}
