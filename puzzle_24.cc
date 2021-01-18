#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

#define TEST 0

struct Group {
    Group(
        int _id,
        int64_t _units,
        int64_t _hp,
        std::set<char> _immune,
        std::set<char> _weak,
        int64_t _attack_power,
        char _attack_type,
        char _initiative)
        : id {_id}
        , units {_units}
        , hp {_hp}
        , immune {_immune}
        , weak {_weak}
        , attack_power {_attack_power}
        , attack_type {_attack_type}
        , initiative {_initiative}
        , effective_power {_units * _attack_power}
        , taken{false}
        , possible_damage{0}
        , attacked_by{0}
        , attacks{0}
    { }

    void compute_possible_attack(Group const& attacking) noexcept {
        if (immune.count(attacking.attack_type)) {
            possible_damage = 0;
        } else if (weak.count(attacking.attack_type)) {
            possible_damage = attacking.effective_power * 2;
        } else {
            possible_damage = attacking.effective_power;
        }
    }

    void accept_hit(Group const& attacking) noexcept {
        assert(units > 0);
        compute_possible_attack(attacking);
        units -= possible_damage / hp;
        update();
    }

    bool empty() const noexcept {
        return units <= 0;
    }

    void update() noexcept { effective_power = units * attack_power; }

    void reset() noexcept
    {
        update();
        taken = false;
        possible_damage = 0;
        attacked_by = 0;
        attacks = 0;
    }

    int id;
    int64_t units;
    int64_t hp;
    std::set<char> immune;
    std::set<char> weak;
    int64_t attack_power;
    char attack_type;
    char initiative;

    int64_t effective_power;
    bool taken;
    int64_t possible_damage;
    int attacked_by;
    int attacks;
};

using Army = std::vector<Group>;

int solve_attack(Group& attacking, Army& a2)
{
    for (Group& target : a2) {
        if (target.taken) continue;
        target.compute_possible_attack(attacking);
    }
    auto top = std::max_element(a2.begin(), a2.end(), [](Group const& a, Group const& b) {
            return (a.taken == b.taken)
            ? (a.possible_damage == b.possible_damage
                    ? (a.effective_power == b.effective_power
                        ? a.initiative < b.initiative
                        : a.effective_power < b.effective_power)
                    : a.possible_damage < b.possible_damage)
            : a.taken;
            });
    if (top == a2.end() || top->taken || top->possible_damage == 0) {
        // no target
        return 0;
    }
    top->taken = true;
    top->attacked_by = attacking.id;
    return top->id;
}

std::pair<int64_t, int64_t> solve(Army a1, Army a2, int64_t attack_boost) {
    auto get_group = [&a1, &a2](int id) -> Group& {
        Army::iterator from, to;
        if (id < 200) {
            from = a1.begin();
            to = a1.end();
        } else {
            from = a2.begin();
            to = a2.end();
        }
        auto it = std::find_if(from, to, [&id](Group const& g) { return g.id == id; });
        assert(it != to);
        return *it;
    };

    for (Group& g : a1) {
        g.attack_power += attack_boost;
        g.update();
    }

    for ( int round = 0; round < 2000; ++round )
    {
        assert(!a1.empty());
        assert(!a2.empty());

        // cleanup
        for (Army& a : {std::ref(a1), std::ref(a2)}) {
            for (Group& g : a) {
                g.reset();
            }
        }

        // order of choosing
        Army all{a1};
        all.insert(all.end(), a2.begin(), a2.end());
        std::sort(all.begin(), all.end(), [](Group const& a, Group const& b) {
            return (a.effective_power == b.effective_power)
                ? (a.initiative > b.initiative)
                : (a.effective_power > b.effective_power);
        });

        for (Group const& g : all) {
            Group& attacking = get_group(g.id);
            if (attacking.id < 200) {
                attacking.attacks = solve_attack(attacking, a2);
            } else {
                attacking.attacks = solve_attack(attacking, a1);
            }
        }


        // Attack order
        std::sort(all.begin(), all.end(), [](Group const& a, Group const& b) {
            return a.initiative > b.initiative;
        });

        for (Group const& g : all) {
            Group& attacking = get_group(g.id);
            if (attacking.empty()) {
                continue;
            }
            if (!attacking.attacks) {
                continue;
            }
            Group& attacked = get_group(attacking.attacks);
            attacked.accept_hit(attacking);
        }

        auto rm_empty = [](Army& a) {
            for (auto it = a.begin(); it != a.end();) {
                if (it->empty()) {
                    it = a.erase(it);
                } else {
                    ++it;
                }
            }
        };

        rm_empty(a1);
        rm_empty(a2);

        if (a1.empty() || a2.empty()) break;
    }

    return std::make_pair(
        std::accumulate(a1.begin(), a1.end(), 0U, [](unsigned const& prev, Group const& g) { return prev + g.units; }),
        std::accumulate(a2.begin(), a2.end(), 0U, [](unsigned const& prev, Group const& g) { return prev + g.units; }));
}


int main() {
#if TEST == 1
    Army a1 {
        Group(100, 17, 5390, {}, {'R', 'B'}, 4507, 'F', 2),
        Group(101, 989, 1274, {'F'}, {'B', 'S'}, 25, 'S', 3)};
    Army a2 {
        Group(200, 801, 4706, {}, {'R'}, 116, 'B', 1),
        Group(201, 4485, 2961, {'R'}, {'F', 'C'}, 12, 'S', 4)};
#else
    Army a1 {
        Group(100, 337, 6482, {'C', 'B'}, {'R', 'F'}, 189, 'S', 15),
        Group(101, 571, 3178, {}, {'F'}, 47, 'S', 12),
        Group(102, 116, 7940, {}, {}, 638, 'F', 18),
        Group(103, 6017, 9349, {}, {'C'}, 14, 'C', 6),
        Group(104, 2246, 4002, {}, {'R', 'S'}, 16, 'C', 3),
        Group(105, 3950, 4493, {'R', 'F'}, {'B'}, 10, 'R', 8),
        Group(106, 7494, 1141, {'B'}, {}, 1, 'C', 17),
        Group(107, 2501, 9007, {}, {}, 35, 'C', 7),
        Group(108, 844, 3222, {'B', 'S'}, {}, 37, 'R', 9),
        Group(109, 1371, 3695, {'C'}, {}, 25, 'C', 10)};

    Army a2 {
        Group(200, 2295, 16577, {'R'}, {}, 12, 'F', 13),
        Group(201, 837, 6736, {}, {'F'}, 14, 'R', 19),
        Group(202, 2841, 9360, {'B'}, {'R', 'C'}, 6, 'F', 14),
        Group(203, 7374, 51597, {'B', 'F'}, {'C'}, 12, 'R', 1),
        Group(204, 1544, 29226, {}, {'F', 'B'}, 35, 'B', 5),
        Group(205, 293, 13961, {'S'}, {'R'}, 79, 'R', 2),
        Group(206, 1219, 38142, {'C', 'F'}, {}, 53, 'B', 4),
        Group(207, 5233, 30595, {'F'}, {'B', 'C'}, 11, 'R', 11),
        Group(208, 397, 43710, {'C', 'B'}, {'S', 'R'}, 171, 'S', 16),
        Group(209, 1316, 36203, {}, {'S', 'B'}, 50, 'C', 20)};
#endif

    auto r1 = solve(a1, a2, 0).second;
    std::cout << "1 Result=" << r1 << "\n";

    int64_t min_attack = 0, max_attack = 20000, koef;
    std::pair<int64_t,int64_t> r2;
    do {
        koef = min_attack + (max_attack - min_attack) / 2;
        r2 = solve(a1, a2, koef);
        if (r2.second == 0) {
            max_attack = koef;
        } else {
            min_attack = koef;
        }
    } while(max_attack - min_attack > 1);

    std::cout << "2 Result=" << r2.first << "\n";

    return 0;
}
