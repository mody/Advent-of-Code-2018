#include <deque>
#include <iomanip>
#include <iostream>
#include <vector>

constexpr int NORTH = 1;
constexpr int EAST = 2;
constexpr int SOUTH = 4;
constexpr int WEST = 8;

struct Point {
    long x = 0, y = 0;

    bool operator== (Point const& oth) const noexcept {
        return x == oth.x && y == oth.y;
    }

    Point west() const noexcept {
        return {x-1, y};
    }

    Point east() const noexcept {
        return {x+1, y};
    }

    Point north() const noexcept {
        return {x, y-1};
    }

    Point south() const noexcept {
        return {x, y+1};
    }

    std::string to_string() const {
        std::string s;
        s.append("x=").append(std::to_string(x)).append(", ").append("y=").append(std::to_string(y));
        return s;
    }
};


int main()
{
    std::string line;
    std::getline(std::cin, line);

    constexpr long SIZE = 110; // real input
    // constexpr long SIZE = 10; // test input

    long min_x = -SIZE, max_x = SIZE;
    long min_y = -SIZE, max_y = SIZE;

    std::vector<std::string> mapa;
    for (int row = 0; row < (max_y-min_y); ++row) {
        mapa.push_back(std::string((max_x-min_x), '#'));
    }
    auto get_at = [&mapa, &min_y, &min_x](Point const& p) -> char& {
        return mapa.at(p.y - min_y).at(p.x - min_x);
    };


    std::deque<Point> q;
    Point p{};

    for (auto const& c : line) {
        if (c == '^') {
            continue;
        } else if (c == '$' ) {
            break;
        } else if (c == '(' ) {
            // push position
            q.push_front(p);
            continue;
        } else if (c == '|' ) {
            // pop position
            p = q.front();
            continue;
        } else if (c == ')' ) {
            // pop position
            p = q.front();
            q.pop_front();
            continue;
        }

        std::cout << c;

        switch(c) {
        case 'W': p = p.west(); break;
        case 'E': p = p.east(); break;
        case 'S': p = p.south(); break;
        case 'N': p = p.north(); break;
        }

        get_at(p) = '+';

        switch(c) {
        case 'W': p = p.west(); break;
        case 'E': p = p.east(); break;
        case 'S': p = p.south(); break;
        case 'N': p = p.north(); break;
        }
        get_at(p) = '.';
    }
    get_at({0, 0}) = 'X';

    for (auto const& row : mapa) {
        std::cout << row << "\n";
    }

    size_t longest = 0, rooms_over_1000 = 0;

    q.clear();
    q.push_front({0, 0});

    for (;!q.empty();) {
        Point p = q.front();
        get_at(p) = 'X';

        if (get_at(p.north()) == '+') {
            get_at(p.north()) = 'X';
            q.push_front(p.north().north());
            continue;
        }
        if (get_at(p.east()) == '+') {
            get_at(p.east()) = 'X';
            q.push_front(p.east().east());
            continue;
        }
        if (get_at(p.south()) == '+') {
            get_at(p.south()) = 'X';
            q.push_front(p.south().south());
            continue;
        }
        if (get_at(p.west()) == '+') {
            get_at(p.west()) = 'X';
            q.push_front(p.west().west());
            continue;
        }

        longest = std::max(longest, q.size());
        if (q.size() > 1000) {
            ++rooms_over_1000;
        }
        q.pop_front();
    }

    std::cout << "1 Result=" << longest - 1 << "\n"
              << "2 Result=" << rooms_over_1000 << "\n";
    return 0;
}
