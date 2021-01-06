#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Counts {
    long tree = 0, lumber = 0, ground = 0, oth = 0;

    long& operator[] (char c) noexcept {
        switch ( c ) {
        case '.':
            return ground;
        case '|':
            return tree;
        case '#':
            return lumber;
        default:
            return oth;
        }
    }

    long operator[] (char c) const noexcept {
        switch ( c ) {
        case '.':
            return ground;
        case '|':
            return tree;
        case '#':
            return lumber;
        default:
            return oth;
        }
    }
};

struct Mapa {
    void insert(std::string line) {
        if (_x_size == 0) {
            _x_size = line.size();
        }
        data.append(std::move(line));
        _y_size = data.size() / _x_size;
    }

    char at(long x, long y) const noexcept {
        if (x < 0 || y < 0 || x >= x_size() || y >= y_size()) {
            return ' ';
        }
        long pos = y * x_size() + x;
        return data.at(pos);
    }

    void put(long x, long y, char c) {
        assert(!(x < 0 || y < 0 || x >= x_size() || y >= y_size()));
        long pos = y * x_size() + x;
        data.at(pos) = c;
    }

    long x_size() const noexcept { return _x_size; }
    long y_size() const noexcept { return _y_size; }


    void dump() const {
        for (long y = 0; y < y_size(); ++y) {
            std::cout << data.substr(y*x_size(), x_size()) << "\n";
        }
    }

    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    size_t get_hash() const noexcept {
        return std::hash<std::string>{}(data);
    }

protected:
    std::string data;
    long _x_size = 0, _y_size = 0;
};


void solution(Mapa m, unsigned long t) {

    auto around = [&m](long const x, long const y) {
        Counts counts;
        ++counts[m.at(x - 1, y - 1)];
        ++counts[m.at(x - 1, y    )];
        ++counts[m.at(x - 1, y + 1)];
        ++counts[m.at(x    , y - 1)];
        ++counts[m.at(x    , y + 1)];
        ++counts[m.at(x + 1, y - 1)];
        ++counts[m.at(x + 1, y    )];
        ++counts[m.at(x + 1, y + 1)];
        return counts;
    };

    std::map<size_t, unsigned long> cycle;

    for ( unsigned long i = 0; i < t; ++i ) {
        auto p = cycle.insert({m.get_hash(), i});
        if (!p.second) {
            // going circles
            auto diff = i - p.first->second;
            cycle.clear();
            t = i + ((t - p.first->second) % diff);
        }
        Mapa result = m;
        for ( int y = 0; y < m.y_size(); ++y ) {
            for ( int x = 0; x < m.x_size(); ++x ) {
                char c = m.at(x, y);
                const Counts counts{around(x, y)};

                if (c == '.') {
                    if (counts['|'] >= 3) {
                         c = '|';
                    }
                } else if (c == '|') {
                    if (counts['#'] >= 3) {
                         c = '#';
                    }
                } else if (c == '#') {
                    if (!(counts['#'] >= 1 && counts['|'] >= 1 )) {
                        c = '.';
                    }
                }
                result.put(x, y, c);
            }
        }
        std::swap(result, m);
    }

    unsigned trees = 0, lumber = 0;
    for (auto const& c : m) {
        if (c == '#') {
            ++lumber;
        } else if (c == '|') {
            ++trees;
        }
    }
    std::cout << t << " Result=" << trees * lumber << "\n";
}

int main()
{
    Mapa m;

    std::string line;
    while ( std::getline(std::cin, line) ) {
        if ( line.empty() ) {
            break;
        }
        m.insert(std::move(line));
    }

    solution(m, 10);
    solution(m, 1000000000);

    return 0;
}
