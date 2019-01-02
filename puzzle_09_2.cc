#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstring>

using Index = unsigned int;

// const Index max_players = 425;
// const Index max_balls = 70848;

const Index max_players = 425;
const Index max_balls = 7084800;

// const Index max_players = 9;
// const Index max_balls = 27;

// const Index max_players = 10;
// const Index max_balls = 1618;

using Pole = std::array<Index, max_balls + 1>;
Index last = 0;

// too big for a stack
Pole pole;


void dump(const Pole& pole, Index cur) {
    // std::cout << "last: " << last << " max: " << pole.size() << "\n";
    for (Index i = 0; i < last; ++i) {
        if (i == cur) {
            std::cout << " (" << pole.at(i) << ")";
        } else {
            std::cout << std::setw(3) << pole.at(i);
        }
    }
    std::cout << "\n";
}

Index clockwise(const Pole& pole, Index pos)
{
    ++pos;
    if (pos == last) {
        pos = 0;
    }
    return pos;
}

Index counter_clockwise(const Pole& pole, Index pos, int step = 1)
{
    for (; step > 0; --step) {
        if (pos == 0) {
            pos = last;
        }
        --pos;
    }
    return pos;
}

Index insert_after(Pole& pole, Index pos, Index val)
{
    ++pos;
    if (pos != last) {
        std::memmove(&pole[pos+1], &pole[pos], sizeof(Index)*(last-pos));
        pole[pos] = val;
    } else {
        pole[last] = val;
    }
    ++last;
    return pos;
}

Index remove_at(Pole& pole, Index pos)
{
    auto result = pole.at(pos);
    std::memmove(&pole[pos], &pole[pos+1], sizeof(Index)*(last - pos - 1));
    last--;
    return result;
}

int main()
{
    pole[0] = 0;
    pole[1] = 1;
    last = 2;

    std::array<Index, max_players> players;
    for (auto& p : players) {
        p = 0;
    }
    Index player = 1;
    Index current = 1;

    for (Index num = 2; num <= max_balls; ++num) {
        if (num % 23 == 0) {
            current = counter_clockwise(pole, current, 7);
            players[player] += (num + remove_at(pole, current));
        } else {
            auto after = clockwise(pole, current);
            current = insert_after(pole, after, num);
        }
        ++player;
        if (player == max_players) {
            player = 0;
        }

        // dump(pole, current);

        if (num % 100000 == 0) {
            std::cout << num << " (" << ((float)num / max_balls * 100) << "%)\n";
        }
    }
    std::cout << max_balls << " (" << ((float)max_balls / max_balls * 100) << "%)\n";

    std::sort(players.begin(), players.end());
    std::cout << *players.rbegin() << "\n";

    return 0;
}

#if 0
[-] (0)
[1]  0 (1)
[2]  0 (2) 1 
[3]  0  2  1 (3)
[4]  0 (4) 2  1  3 
[5]  0  4  2 (5) 1  3 
[6]  0  4  2  5  1 (6) 3 
[7]  0  4  2  5  1  6  3 (7)
[8]  0 (8) 4  2  5  1  6  3  7 
[9]  0  8  4 (9) 2  5  1  6  3  7 
[1]  0  8  4  9  2(10) 5  1  6  3  7 
[2]  0  8  4  9  2 10  5(11) 1  6  3  7 
[3]  0  8  4  9  2 10  5 11  1(12) 6  3  7 
[4]  0  8  4  9  2 10  5 11  1 12  6(13) 3  7 
[5]  0  8  4  9  2 10  5 11  1 12  6 13  3(14) 7 
[6]  0  8  4  9  2 10  5 11  1 12  6 13  3 14  7(15)
[7]  0(16) 8  4  9  2 10  5 11  1 12  6 13  3 14  7 15 
[8]  0 16  8(17) 4  9  2 10  5 11  1 12  6 13  3 14  7 15 
[9]  0 16  8 17  4(18) 9  2 10  5 11  1 12  6 13  3 14  7 15 
[1]  0 16  8 17  4 18  9(19) 2 10  5 11  1 12  6 13  3 14  7 15 
[2]  0 16  8 17  4 18  9 19  2(20)10  5 11  1 12  6 13  3 14  7 15 
[3]  0 16  8 17  4 18  9 19  2 20 10(21) 5 11  1 12  6 13  3 14  7 15 
[4]  0 16  8 17  4 18  9 19  2 20 10 21  5(22)11  1 12  6 13  3 14  7 15 
[5]  0 16  8 17  4 18(19) 2 20 10 21  5 22 11  1 12  6 13  3 14  7 15 
[6]  0 16  8 17  4 18 19  2(24)20 10 21  5 22 11  1 12  6 13  3 14  7 15 
[7]  0 16  8 17  4 18 19  2 24 20(25)10 21  5 22 11  1 12  6 13  3 14  7 15
#endif
