#include <boost/container_hash/hash.hpp>

#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#define BOUNDARY 1

enum {
    NOTHING = 0,
    CLIMBING = 1,
    TORCH = 2,
};

enum {
    ROCKY = 0,
    WET = 1,
    NARROW = 2,
};

struct Cell;

struct Point {
    long x = 0, y = 0;
    long prev_x = 0, prev_y = 0;
    unsigned int duration = 0;
    int holds = TORCH;

    Point left() const noexcept {
        return {x - 1, y, x, y};
    }

    Point right() const noexcept {
        return {x + 1, y, x, y};
    }

    Point up() const noexcept {
        return {x, y - 1, x, y};
    }

    Point down() const noexcept {
        return {x, y + 1, x, y};
    }

    bool operator==(Point const& o) const noexcept { return x == o.x && y == o.y; }
    bool operator!=(Point const& o) const noexcept { return ! operator==(o); }

    std::string to_string() const {
        std::string s;
        s.append("x=")
            .append(std::to_string(x))
            .append(", y=")
            .append(std::to_string(y))
            .append(", d=")
            .append(std::to_string(duration))
            .append(", holds=");
        switch(holds) {
        case NOTHING:
            s.append("NOTHING"); break;
        case CLIMBING:
            s.append("CLIMBING"); break;
        case TORCH:
            s.append("TORCH"); break;
        };
        return s;
    }
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
    long erosion_level = 0;
    int type = 0;

    unsigned int duration = 0;

    std::string to_string() const {
        std::string s;
        switch(type) {
        case ROCKY:
            s.append("ROCKY"); break;
        case WET:
            s.append("WET"); break;
        case NARROW:
            s.append("NARROW"); break;
        };
        s.append(", d=").append(std::to_string(duration));
        return s;
    }
};

struct World
{
    World(long const depth, Point _target) : max_x{_target.x+BOUNDARY}, max_y{_target.y+BOUNDARY}, target(std::move(_target))
    {
        for (long y = 0; y < max_y; ++y) {
            for (long x = 0; x < max_x; ++x) {
                Cell& c = get({x, y});
                uint64_t geo_index = 0;
                if (y == 0 && x == 0) {
                    // leave at 0
                    continue;
                } else if (y == 0) {
                    geo_index = x * 16807;
                } else if (x == 0) {
                    geo_index = y * 48271;
                } else {
                    //X-1,Y and X,Y-1
                    geo_index = at({x - 1, y}).erosion_level * at({x, y - 1}).erosion_level;
                }
                c.erosion_level = (geo_index + depth) % 20183;
                c.type = c.erosion_level % 3;
            }
        }
        at(target).type = ROCKY;
    }

    long risk_level() const {
        long l = 0;
        for (long y = 0; y <= target.y; ++y) {
            for (long x = 0; x <= target.x; ++x) {
                l += at({x, y}).type;
            }
        }
        return l;
    }

    // bool can_move(Point p) {
    //     Cell const& c0 = at(p);
    //     if (c0.type == ROCKY && p.holds == NOTHING) {
    //         // std::cout << "Can't move there\n";
    //         return false;
    //     }
    //     if (c0.type == WET && p.holds == TORCH) {
    //         // std::cout << "Can't move there\n";
    //         return false;
    //     }
    //     if (c0.type == NARROW && p.holds == CLIMBING) {
    //         // std::cout << "Can't move there\n";
    //         return false;
    //     }
    //     if ((c0.duration && c0.duration <= p.duration) || (shortest < p.duration)) {
    //         // std::cout << "Slow to move there\n";
    //         return false;
    //     }
    //     return true;
    // }

    // void step(Point p0, unsigned level) {
    //     Cell& c0 = at(p0);
    //     const Point prev{p0.prev_x, p0.prev_y};

    //     // std::cout << "@ " << p0.to_string() << ", cell=" << c0.to_string() << ", level=" << level << "\n";

    //     if ((c0.duration && c0.duration <= p0.duration) || (shortest < p0.duration)) {
    //         // std::cout << "took longer\b";
    //         // our path is longer
    //         return;
    //     }
    //     c0.duration = p0.duration;
    //     // dump_dur();

    //     if (p0 == target) {
    //         shortest = std::min(shortest, c0.duration + 7); // switch to torch
    //     }

    //     // try all combinations
    //     int other_tool = -1;
    //     if (c0.type == ROCKY) {
    //         assert(p0.holds == TORCH || p0.holds == CLIMBING);
    //         other_tool = (p0.holds == TORCH ? CLIMBING : TORCH);

    //     } else if (c0.type == WET) {
    //         assert(p0.holds == CLIMBING || p0.holds == NOTHING);
    //         other_tool = (p0.holds == NOTHING ? CLIMBING : NOTHING);

    //     } else if (c0.type == NARROW) {
    //         assert(p0.holds == TORCH || p0.holds == NOTHING);
    //         other_tool = (p0.holds == NOTHING ? TORCH : NOTHING);
    //     }
    //     assert(other_tool >= NOTHING);
    //     assert(other_tool <= TORCH);

    //     for (Point p1 : {p0.right(), p0.down(), p0.left(), p0.up()}) {
    //         if ( p1 == Point{0, 0} ) {
    //             continue;
    //         }
    //         if (p1.x < 0 || p1.x >= max_x || p1.y < 0 || p1.y >= max_y) {
    //             continue;
    //         }
    //         if (p1 == prev) {
    //             // std::cout << "Skip return path\n";
    //             continue;
    //         }
    //         for (int tool : {p0.holds, other_tool}) {
    //             p1.duration = p0.duration + 1 + (tool != p0.holds ? 7 : 0);
    //             p1.holds = tool;
    //             // std::cout << "try " << p1.to_string() << "\n";
    //             if (can_move(p1)) {
    //                 step(p1, level+1);
    //             }
    //         }
    //     }
    // }

    // unsigned long rescue() {
    //     shortest = std::numeric_limits<unsigned int>::max();
    //     step({}, 0);
    //     return shortest;
    // }

    bool can_move2(Point p) {
        Cell const& c0 = at(p);
        if (c0.type == ROCKY && p.holds == NOTHING) {
            // std::cout << "Can't move there\n";
            return false;
        }
        if (c0.type == WET && p.holds == TORCH) {
            // std::cout << "Can't move there\n";
            return false;
        }
        if (c0.type == NARROW && p.holds == CLIMBING) {
            // std::cout << "Can't move there\n";
            return false;
        }
        return true;
    }

    unsigned long rescue2() {
        auto shortest = [](Point const& a, Point const& b) {
            return a.duration > b.duration;
        };
        std::priority_queue<Point, std::vector<Point>, decltype(shortest)> qq(shortest);

        qq.push({});

        std::unordered_set<Point> visited;
        while(!qq.empty()) {
            {
                auto q2 = qq;
                std::cout << "qq: ";
                while(!q2.empty()) {
                    std::cout << q2.top().to_string() << ",";
                    q2.pop();
                }
                std::cout << "\n";
            }
            Point p0{qq.top()};
            qq.pop();
            if (visited.count(p0)) {
                continue;
            }

            if (p0 == target) {
                target.duration = p0.duration + 7;
                break;
            }

            visited.insert(p0);
            Cell& c0 = at(p0);
            c0.duration = p0.duration;

            std::cout << "@ " << p0.to_string() << "\n";
            dump_dur();

            // try all combinations
            int other_tool = -1;
            if (c0.type == ROCKY) {
                assert(p0.holds == TORCH || p0.holds == CLIMBING);
                other_tool = (p0.holds == TORCH ? CLIMBING : TORCH);

            } else if (c0.type == WET) {
                assert(p0.holds == CLIMBING || p0.holds == NOTHING);
                other_tool = (p0.holds == NOTHING ? CLIMBING : NOTHING);

            } else if (c0.type == NARROW) {
                assert(p0.holds == TORCH || p0.holds == NOTHING);
                other_tool = (p0.holds == NOTHING ? TORCH : NOTHING);
            }
            assert(other_tool >= NOTHING);
            assert(other_tool <= TORCH);

            for (Point p1 : {p0.right(), p0.down(), p0.left(), p0.up()}) {
                if (p1.x < 0 || p1.x >= max_x || p1.y < 0 || p1.y >= max_y) {
                    continue;
                }
                // if (visited.count(p1)) {
                //     continue;
                // }
                for (int tool : {p0.holds, other_tool}) {
                    p1.duration = p0.duration + 1 + (tool != p0.holds ? 7 : 0);
                    p1.holds = tool;
                    // std::cout << "try " << p1.to_string() << "\n";
                    if (can_move2(p1)) {
                        std::cout << "? " << p1.to_string() << "\n";
                        qq.push(std::move(p1));
                    }
                }
            }
        }

        return target.duration;
    }



    Cell& get(Point const& p) { return mapa[p]; }

    Cell const& at(Point const& p) const { return mapa.at(p); }
    Cell& at(Point const& p) { return mapa.at(p); }

    void dump() const {
        for (long y = 0; y < max_y; ++y) {
            std::string line;
            line.reserve(max_x + 1);
            for (long x = 0; x < max_x; ++x) {
                if (x == 0 && y == 0) {
                    line.append("X");
                    continue;
                }
                if (target == Point{x, y}) {
                    line.append("T");
                    continue;
                }
                switch(at({x, y}).type) {
                case 0:
                    line.append("."); break;
                case 1:
                    line.append("="); break;
                case 2:
                    line.append("|"); break;
                default:
                    line.append("?");
                }
            }
            std::cout << line << "\n";
        }
    }

    void dump_ero() const {
        for (long y = 0; y < max_y; ++y) {
            for (long x = 0; x < max_x; ++x) {
                std::cout << std::setw(10) << at({x, y}).erosion_level;
            }
            std::cout << "\n";
        }
    }

    void dump_dur() const {
        for (long y = 0; y < max_y; ++y) {
            for (long x = 0; x < max_x; ++x) {
                std::cout << std::setw(5) << at({x, y}).duration;
            }
            std::cout << "\n";
        }
    }

protected:
    std::unordered_map<Point, Cell> mapa;
    long max_x, max_y;
    Point target;
    unsigned int shortest;
};

int main()
{
    // depth: 510
    // target: 10,10
    World mapa(510, {10, 10});
    // mapa.dump_ero();

    // depth: 5355
    // target: 14,796
    // World mapa(5355, {14, 796});

    mapa.dump();

    std::cout << "1 Result=" << mapa.risk_level() << "\n";

    auto shortest = mapa.rescue2();
    std::cout << "2 Result=" << shortest << "\n";

    return 0;
}
