#include <boost/container_hash/hash.hpp>

#include <cassert>
#include <deque>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

struct Point {
    long x = 0, y = 0;

    bool operator==(Point const& o) const noexcept { return x == o.x && y == o.y; }
    bool operator!=(Point const& o) const noexcept { return ! operator==(o); }
};

Point operator+(Point lhs, Point const& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Point operator-(Point lhs, Point const& rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

namespace std {

template <>
struct hash<Point>
{
    size_t operator()(Point const& p) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        return seed;
    }
};

}  // namespace std


struct Cell {
    Cell(char c) : content{c} {}

    char content;
};


struct World {
   std::unordered_map<Point, Cell> mapa;

   void insert(Point p, Cell c) {
       mapa.insert(std::make_pair(std::move(p), std::move(c)));
   }

   auto measure()
   {
       long min_x = std::numeric_limits<long>::max();
       long min_y = std::numeric_limits<long>::max();
       long max_x = std::numeric_limits<long>::min();
       long max_y = std::numeric_limits<long>::min();
       for ( auto const& [p, _] : mapa ) {
           min_x = std::min(min_x, p.x);
           min_y = std::min(min_y, p.y);
           max_x = std::max(max_x, p.x);
           max_y = std::max(max_y, p.y);
       }
       --min_x;
       ++max_x;
       return std::make_tuple(min_x, max_x, min_y, max_y);
   }


   void dump() {
       unsigned min_x, max_x, min_y, max_y;
       std::tie(min_x, max_x, min_y, max_y) = measure();

       for (unsigned y = min_y; y <= max_y; ++y ) {
           std::string line;
           line.reserve(max_x - min_y + 1);
           for ( unsigned x = min_x; x <= max_x; ++x ) {
               line += check({x, y});
           }
           std::cout << line << "\n";
       }
   }


   Cell& get(Point const& p) noexcept {
       return mapa.insert(std::make_pair(p, Cell{'.'})).first->second;
   }

   char check(Point const& p) noexcept {
       auto it = mapa.find(p);
       if (it == mapa.end()) {
           return '.';
       }
       return it->second.content;
   }

   auto begin() { return mapa.begin(); }
   auto end() { return mapa.end(); }
   auto begin() const { return mapa.begin(); }
   auto end() const { return mapa.end(); }
   auto cbegin() const { return mapa.begin(); }
   auto cend() const { return mapa.end(); }
};


void solution(World world) {

    unsigned min_x, max_x, min_y, max_y;
    std::tie(min_x, max_x, min_y, max_y) = world.measure();

    const Point DOWN{0, 1};
    const Point LEFT{-1, 0};
    const Point RIGHT{1, 0};

    std::deque<Point> q;
    q.push_front({500, 0});

    for ( ; ! q.empty(); ) {
        Point p0{q.front()};
        Cell& cell = world.get(p0);
        cell.content = '~';

        // down
        Point pd = p0 + DOWN;
        {
            if (pd.y > max_y) {
                q.pop_front();
                continue;
            }
            auto const& cd = world.get(pd);
            if (cd.content == '.') {
                q.push_front(std::move(pd));
                continue;
            }
        }
        // left
        {
            Point pl = p0 + LEFT;
            if (pl.x < min_x) {
                q.pop_front();
                continue;
            }
            Point pd = p0 + DOWN;
            auto const& cl = world.get(pl);
            auto const& cd = world.get(pd);
            if (cl.content == '.' && (cd.content == '#' || cd.content == '=')) {
                q.push_front(std::move(pl));
                continue;
            }
        }
        // right
        {
            Point pr = p0 + RIGHT;
            if (pr.x > max_x) {
                q.pop_front();
                continue;
            }
            Point pd = p0 + DOWN;
            auto const& cr = world.get(pr);
            auto const& cd = world.get(pd);
            if (cr.content == '.' && (cd.content == '#' || cd.content == '=')) {
                q.push_front(std::move(pr));
                continue;
            }
        }

        // backtrack
        bool left_wall = false, right_wall = false;
        Point pxl, pxr;
        for (pxl = p0 + LEFT; pxl.x > min_x; pxl = pxl + LEFT) {
            if (world.check(pxl) == '#') {
                left_wall = true;
                break;
            }
            if (world.check(pxl) == '.') {
                break;
            }
        }
        for (pxr = p0 + RIGHT; pxr.x <= max_x; pxr = pxr + RIGHT) {
            if (world.check(pxr) == '#') {
                right_wall = true;
                break;
            }
            if (world.check(pxr) == '.') {
                break;
            }
        }

        if (left_wall && right_wall) {
            for (pxl = pxl + RIGHT; pxl != pxr; pxl = pxl + RIGHT) {
                world.get(pxl).content = '=';
            }
        }
        do {
            q.pop_front();
        } while(!q.empty() && world.get(q.front()).content == '=');
    }

    //count watter
    unsigned cnt_watter = 0, cnt_still = 0;
    for (auto const& [p, c] : world) {
        if (p.y < min_y) {
            continue;
        }
        if (c.content == '~') {
           ++cnt_watter;
        } else if(c.content == '=') {
            ++cnt_still;
        }
    }

    std::cout << "1 Result=" << (cnt_still+cnt_watter) << "\n";
    std::cout << "2 Result=" << cnt_still << "\n";
}



// Positions                 1     2     3     4             5
static const std::regex RX{"(.)=(\\d+), (.)=(\\d+)\\.?\\.?(\\d+)?"};

int main()
{
    World world;

    std::string line;
    while ( std::getline(std::cin, line) ) {
        if ( line.empty() ) {
            break;
        }

        std::smatch m;
        if (std::regex_match(line, m, RX)) {
            assert(m.size() == 6);
            if (m.str(1) == "x") {
                unsigned x = std::stoi(m.str(2));
                unsigned y_from = std::stoi(m.str(4));
                unsigned y_to = std::stoi(m.str(5));
                for (unsigned y = y_from; y <= y_to; ++y) {
                    world.insert({x, y}, Cell{'#'});
                }
            } else {
                unsigned y = std::stoi(m.str(2));
                unsigned x_from = std::stoi(m.str(4));
                unsigned x_to = std::stoi(m.str(5));
                for (unsigned x = x_from; x <= x_to; ++x) {
                    world.insert({x, y}, Cell{'#'});
                }
            }
        } else {
            std::cout << "ERROR parsing: " << line << "\n";
            exit(1);
        }
    }

    solution(world);

    return 0;
}
