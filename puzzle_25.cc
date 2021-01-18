#include <boost/container_hash/hash.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <vector>

static std::regex RX{"^([0-9-]+),([0-9-]+),([0-9-]+),([0-9-]+)"};

struct Point {
    // Point() = default;
    // Point(int64_t _i, int64_t _j, int64_t _k, int64_t _l) noexcept : i(_i), j(_j), k(_k), l(_l) {}

    int64_t i = 0, j = 0, k = 0, l = 0;
};

int64_t star_distance(Point const& a, Point const& b) noexcept {
    return std::abs(a.i - b.i) + std::abs(a.j - b.j) + std::abs(a.k - b.k) + std::abs(a.l - b.l);
}

bool operator< (Point const& a, Point const& b) noexcept {
    return star_distance(a, {}) < star_distance(b, {});
}

bool operator== (Point const& a, Point const& b) noexcept {
    return a.i == b.i && a.j == b.j && a.k == b.k && a.l == b.l;
}

std::string to_string(Point const& p) {
    return "(" + std::to_string(p.i).append(",") + std::to_string(p.j).append(",") + std::to_string(p.k).append(",") +
           std::to_string(p.l) + ")";
}

namespace std {

template <>
struct hash<Point>
{
    size_t operator()(Point const& p) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.i);
        boost::hash_combine(seed, p.j);
        boost::hash_combine(seed, p.k);
        boost::hash_combine(seed, p.l);
        return seed;
    }
};

}  // namespace std

using Group = std::vector<Point>;
using Constellation = std::unordered_set<Point>;

auto measure(Group const& g)
{
    long min_i = std::numeric_limits<long>::max();
    long min_j = std::numeric_limits<long>::max();
    long min_k = std::numeric_limits<long>::max();
    long min_l = std::numeric_limits<long>::max();
    long max_i = std::numeric_limits<long>::min();
    long max_j = std::numeric_limits<long>::min();
    long max_k = std::numeric_limits<long>::min();
    long max_l = std::numeric_limits<long>::min();
    for ( auto const& b : g ) {
        min_i = std::min(min_i, b.i);
        min_j = std::min(min_j, b.j);
        min_k = std::min(min_k, b.k);
        min_l = std::min(min_l, b.l);
        max_i = std::max(max_i, b.i);
        max_j = std::max(max_j, b.j);
        max_k = std::max(max_k, b.k);
        max_l = std::max(max_l, b.l);
    }
    ++max_i;
    ++max_j;
    ++max_k;
    ++max_l;
    return std::make_tuple(min_i, max_i, min_j, max_j, min_k, max_k, min_l, max_l);
}

int main()
{
    Group stars;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) break;
        std::smatch m;
        if (!std::regex_match(line, m, RX)) {
            std::cout << "ERROR on line " << line << "\n";
            exit(1);
        }
        stars.push_back(Point{std::stoll(m.str(1)), std::stoll(m.str(2)), std::stoll(m.str(3)), std::stoll(m.str(4))});
    }

    std::vector<Constellation> constellations;

    for(Point p0 : stars) {
        bool added = false;
        for (auto& c : constellations) {
            if ( std::any_of(c.begin(), c.end(), [&p0](Point const& oth) { return star_distance(p0, oth) <= 3; }) ) {
                c.insert(p0);
                added = true;
            }
        }
        if (!added) {
            constellations.push_back({});
            constellations.back().insert(std::move(p0));
        }
    }

    for (auto i1 = constellations.begin(); i1 != std::prev(constellations.end()); ++i1) {
        for (auto i2 = std::next(i1); i2 != constellations.end(); ) {
            Constellation g0 = *i1;
            std::copy(i2->begin(), i2->end(), std::inserter(g0, g0.end()));
            if ( g0.size() < (i1->size() + i2->size()) ) {
                *i1 = std::move(g0);
                constellations.erase(i2);
                i2 = std::next(i1);
            } else {
                ++i2;
            }
        }
    }

    std::cout << "1 Result=" << constellations.size() << "\n";

    return 0;
}
