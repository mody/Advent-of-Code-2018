#include <algorithm>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

static std::regex RX{"^pos=<([0-9-]+),([0-9-]+),([0-9-]+)>, r=(\\d+)$"};

struct Bot {
    Bot() = default;
    Bot(int64_t _x, int64_t _y, int64_t _z, int64_t _r) noexcept
        : x(_x), y(_y), z(_z), r(_r), orig_x(_x), orig_y(_y), orig_z(_z), orig_r(_r)
    {
    }

    Bot(Bot&&) = default;
    Bot(Bot const&) = default;

    Bot& operator=(Bot&&) = default;
    Bot& operator=(Bot const&) = default;

    bool operator== (Bot const& o) const noexcept {
        return x == o.x && y == o.y && z == o.z;
    }
    bool operator!= (Bot const& o) const noexcept {
        return !operator==(o);
    }

    bool operator< (Bot const& o) const noexcept {
        if (x == o.x) {
            if (y == o.y) {
                return z < o.z;
            }
            return y < o.y;
        }
        return x < o.x;
    }

    void scale(int64_t koef) {
        x = orig_x / koef;
        y = orig_y / koef;
        z = orig_z / koef;
        r = orig_r / koef;
    }

    int64_t distance(Bot const& o) const noexcept {
        return std::abs(x - o.x) + std::abs(y - o.y) + std::abs(z - o.z);
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "x=" << x << ",y=" << y << ",z=" << z << ",r=" << r;
        return ss.str();
    }

    int64_t x = 0, y = 0, z = 0, r = 0;
protected:
    int64_t orig_x = 0, orig_y = 0, orig_z = 0, orig_r = 0;
};

using Group = std::vector<Bot>;

auto measure(Group const& g)
{
    long min_x = std::numeric_limits<long>::max();
    long min_y = std::numeric_limits<long>::max();
    long min_z = std::numeric_limits<long>::max();
    long max_x = std::numeric_limits<long>::min();
    long max_y = std::numeric_limits<long>::min();
    long max_z = std::numeric_limits<long>::min();
    for ( auto const& b : g ) {
        min_x = std::min(min_x, b.x);
        min_y = std::min(min_y, b.y);
        min_z = std::min(min_z, b.z);
        max_x = std::max(max_x, b.x);
        max_y = std::max(max_y, b.y);
        max_z = std::max(max_z, b.z);
    }
    ++max_x;
    ++max_y;
    ++max_z;
    return std::make_tuple(min_x, max_x, min_y, max_y, min_z, max_z);
}

int main()
{
    Group bots;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) break;
        std::smatch m;
        if (!std::regex_match(line, m, RX)) {
            std::cout << "ERROR on line " << line << "\n";
            exit(1);
        }
        bots.push_back({std::stoll(m.str(1)), std::stoll(m.str(2)), std::stoll(m.str(3)), std::stoll(m.str(4))});
    }

    Bot strongest;
    for (auto const& b : bots) {
        if (b.r > strongest.r) {
            strongest = b;
        }
    }

    std::cout << "Strongest: " << strongest.to_string() << "\n";

    unsigned in_range = 0;
    for (auto const& b : bots) {
        if (strongest.distance(b) <= strongest.r) {
            ++in_range;
        }
    }

    std::cout << "1 Range=" << in_range << std::endl;

    Bot top_bot;
    Bot center;

    constexpr int64_t start_koef = 10000000;
    constexpr int64_t koef_step = 10;
    for (auto& b : bots) {
        b.scale(start_koef);
    }

    int64_t min_x, max_x, min_y, max_y, min_z, max_z;
    std::tie(min_x, max_x, min_y, max_y, min_z, max_z) = measure(bots);

    for (int64_t koef = start_koef; koef >= 1; koef = koef / koef_step) {
        for (auto& b : bots) {
            b.scale(koef);
        }

        unsigned top_in = 0;
        for (int64_t x = min_x; x < max_x; x += 1) {
            for (int64_t y = min_y; y < max_y; y += 1) {
                for (int64_t z = min_z; z < max_z; z += 1) {
                    const Bot b {x, y, z, 0};
                    auto in = std::count_if(bots.begin(), bots.end(), [&b](auto const& o) {
                            return b.distance(o) <= o.r;
                            });
                    if (in > top_in) {
                        top_bot = b;
                        top_in = in;
                    } else if (in == top_in) {
                        if (center.distance(b) < center.distance(top_bot)) {
                            top_bot = b;
                            top_in = in;
                        }
                    }
                }
            }
        }

        std::cout << "top_bot: " << top_bot.to_string() << " with " << top_in << " hits.\n";
        min_x = (top_bot.x - 2) * koef_step;
        min_y = (top_bot.y - 2) * koef_step;
        min_z = (top_bot.z - 2) * koef_step;

        max_x = (top_bot.x + 2) * koef_step;
        max_y = (top_bot.y + 2) * koef_step;
        max_z = (top_bot.z + 2) * koef_step;
    }
    std::cout << "2 Result: " << top_bot.to_string() << ": " << top_bot.x+top_bot.y+top_bot.z << std::endl;

    return 0;
}
