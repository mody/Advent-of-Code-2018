#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <set>
#include <variant>
#include <vector>

class Cell;

struct Position {
    int x = 0, y = 0;
    unsigned steps = 0;
    Cell* prev_cell = nullptr;

    Position left(Cell* prev = nullptr) const noexcept {
        return {x - 1, y, steps, prev};
    }

    Position right(Cell* prev = nullptr) const noexcept {
        return {x + 1, y, steps, prev};
    }

    Position up(Cell* prev = nullptr) const noexcept {
        return {x, y - 1, steps, prev};
    }

    Position down(Cell* prev = nullptr) const noexcept {
        return {x, y + 1, steps, prev};
    }
};


struct Player {
    Player(char team, Position pos) : _team{team}, _hp{200}, _pos{std::move(pos)} {}

    char team() const { return _team; }
    Position pos() const { return _pos; }
    void setPos(Position pos) { _pos = std::move(pos); }
    int hp() const { return _hp; }

    int hit() noexcept {
        _hp -= 3;
        return _hp;
    }

    bool operator< (Player const& oth) const noexcept {
        return (_pos.y*100+_pos.x) < (oth._pos.y*100+oth._pos.x);
    }

protected:
    char _team;
    int _hp;
    Position _pos;
};

using Team = std::vector<std::shared_ptr<Player>>;


struct Cell {
    Cell(Position p, bool w = false) : pos{std::move(p)}, wall{w}, player{nullptr}, steps{0}, prev_cell{nullptr} {}

    Cell(Cell&&) = default;
    Cell(Cell const&) = default;
    Cell& operator=(Cell&&) = default;
    Cell& operator=(Cell const&) = default;

    const Position pos;
    bool wall;
    Player* player;
    std::set<Player*> hitboxes;

    unsigned steps = 0;
    Cell* prev_cell;

    std::string to_string() const {
        if (wall) { return "#"; }
        if (player) { return std::string{} + player->team(); }
        // if (!hitboxes.empty()) { return "H"; }
        return " ";
    }
};


class Mapa
{
public:
    Mapa(std::istream& in)
    {
        Position pos;
        std::string line;
        while ( std::getline(std::cin, line) )
        {
            if ( line.empty() ) { break; }
            _x_size = line.size();
            for ( auto const& c : line ) {
                if ( c == '#' ) {
                    content.push_back(Cell(pos, true));
                } else if (c == '.') {
                    content.push_back(Cell(pos));
                } else if (c == 'E') {
                    content.push_back(Cell(pos));
                    elves.push_back(std::make_shared<Player>(c, pos));
                } else if (c == 'G') {
                    content.push_back(Cell(pos));
                    goblins.push_back(std::make_shared<Player>(c, pos));
                }
                pos = pos.right();
            }
            pos = pos.down();
            pos.x = 0;
        }
        _y_size = content.size() % _x_size;

        // put players to the map
        std::sort(elves.begin(), elves.end());
        for (auto p : elves) {
            put(p.get());
        }
        std::sort(goblins.begin(), goblins.end());
        for (auto p : goblins) {
            put(p.get());
        }
    }


    Player* can_attack(Player* me, const char enemy) {
        Player* attack = nullptr;
        switch ( get(me->pos()).hitboxes.size() )
        {
        case 0:
            break;
        case 1: {
            Player* h = *get(me->pos()).hitboxes.begin();
            assert(h);
            if ( h->team() == enemy && h->hp() > 0) {
                attack = h;
            }
            break;
        }
        default:
            // 2 or more
            int hp = std::numeric_limits<int>::max();
            for ( Player* h : get(me->pos()).hitboxes ) {
                assert(h);
                if ( h->team() == 'G' && h->hp() > 0 && h->hp() < hp ) {
                    attack = h;
                    hp     = h->hp();
                }
            }
        }
        if (attack) {
            std::cout << "By enemy " << enemy << " with HP " << attack->hp() << "\n";
        } else {
            std::cout << "No enemy near.\n";
        }
        return attack;
    }

    Cell* find_move(Player* me, const char enemy) {
        clear_steps();
        std::deque<Position> q;

        Cell* destination = nullptr;

        {
            Cell* prev_cell = &get(me->pos());
            q.push_back(me->pos().up(prev_cell));
            q.push_back(me->pos().right(prev_cell));
            q.push_back(me->pos().down(prev_cell));
            q.push_back(me->pos().left(prev_cell));
        }

        for (;!q.empty();) {
            Position pos{q.front()};
            q.pop_front();

            std::cout << "Consider x=" << pos.x << ", y=" << pos.y << ", steps=" << pos.steps
                << ", prev x=" << pos.prev_cell->pos.x << ", y=" << pos.prev_cell->pos.y << ", steps=" << pos.prev_cell->pos.steps
                << "\n";
            // dump_steps();

            Cell& cell = get(pos);
            if (cell.wall || cell.player) {
                continue;
            }

            if (cell.steps > pos.steps) {
                cell.steps = pos.steps;
                cell.prev_cell = pos.prev_cell;
            }

            // check hitboxes
            Player* opponent = nullptr;
            int last_hp = 201;
            for (auto* h : cell.hitboxes) {
                std::cout << "H: x="<<h->pos().x << ", y=" << h->pos().y << ", "<<h->team()<<", hp="<<h->hp();
                if (h->team() == enemy && h->hp() < last_hp) {
                    std::cout << " ENEMY";
                    opponent = h;
                    last_hp = h->hp();
                }
                std::cout << "\n";
            }

            if (opponent) {
                destination = &cell;
                break;
            }

            auto queue_next = [&](Position _p)
            {
                auto const& _c = get(_p);
                if (!(_c.wall || _c.player || _c.prev_cell == &cell || _c.steps < pos.steps)) {
                    q.push_back(_p);
                }
            };

            ++pos.steps;
            queue_next(pos.up(&cell));
            queue_next(pos.right(&cell));
            queue_next(pos.down(&cell));
            queue_next(pos.left(&cell));
        }

        dump_steps();

        if (destination) {
            // back-track
            std::cout << "Destination x=" << destination->pos.x << ", y=" << destination->pos.y << "\n";
            for ( Cell* start_cell = &get(me->pos()); destination->prev_cell != start_cell;
                  destination = destination->prev_cell )
            {
                std::cout << "Move to x=" << destination->pos.x << ", y=" << destination->pos.y << "\n";
            }
            std::cout << "Move to x=" << destination->pos.x << ", y=" << destination->pos.y << "\n";
            return destination;
        }
        return nullptr;
    }


    bool turn() {
        bool done = true;
        for (auto it = elves.begin(); it != elves.end(); ) {
            auto& me = *it;
            dump();
            // Elve attack
            Player* attack = can_attack(me.get(), 'G');
            if (attack) {
                done = false;
                if(attack->hit() <= 0) {
                    it = kill_player(attack, elves);
                }
            } else {
                // Elve move
                Cell* dest = find_move(me.get(), 'G');
                if (dest) {
                    done = false;
                    move_player(me.get(), dest->pos);
                }
                ++it;
            }
        }

        for (auto it = goblins.begin(); it != goblins.end(); ) {
            auto& me = *it;
            dump();
            // Goblin attack
            Player* attack = can_attack(me.get(), 'E');
            if (attack) {
                done = false;
                if(attack->hit() <= 0) {
                    it = kill_player(attack, goblins);
                }
            } else {
                // Goblin move
                Cell* dest = find_move(me.get(), 'E');
                if (dest) {
                    done = false;
                    move_player(me.get(), dest->pos);
                }
                ++it;
            }
        }
        dump();

        return done;
    }


    Team::iterator kill_player(Player* p, Team& team) {
        assert(p);
        std::cout << "Player at x=" << p->pos().x << ", y=" << p->pos().y << " was killed\n";
        clear(p);
        get(p->pos()).player = nullptr;
        return team.erase(std::find_if(team.begin(), team.end(), [&p](auto const& member) { return member.get() == p; }));
    }

    void move_player(Player* me, Position dest) {
        clear(me);
        me->setPos(dest);
        put(me);
    }

    void put(Player* p) {
        assert(p);
        get(p->pos()).player = p;
        auto put_hitbox      = [this, &p](Position pos) {
            auto& c = get(pos);
            if ( !c.wall ) { c.hitboxes.insert(p); }
        };
        put_hitbox(p->pos().up());
        put_hitbox(p->pos().down());
        put_hitbox(p->pos().left());
        put_hitbox(p->pos().right());
    }

    void clear(Player* p) {
        assert(p);
        auto clear_hitbox = [this, &p](Position pos) {
            get(pos).hitboxes.erase(p);
        };
        clear_hitbox(p->pos().up());
        clear_hitbox(p->pos().down());
        clear_hitbox(p->pos().left());
        clear_hitbox(p->pos().right());
        get(p->pos()).player = nullptr;
    }

    void clear_steps() noexcept {
        for (auto & cell : content) {
            cell.steps = 99;
            cell.prev_cell = nullptr;
        }
    }

    Cell const& get(Position const& p) const { return content.at(p.y * _x_size + p.x); }
    Cell& get(Position const& p) { return content.at(p.y * _x_size + p.x); }

    int x_size() const noexcept { return _x_size; }
    int y_size() const noexcept { return _y_size; }

    void dump() const {
        for ( size_t i = 0; i < content.size(); ++i ) {
            if (i % _x_size == 0) {
                std::cout << "\n";
            }
            std::cout << content.at(i).to_string();
        }
        std::cout << "\n";
    }

    void dump_steps() const {
        for ( size_t i = 0; i < content.size(); ++i ) {
            if (i % _x_size == 0) {
                std::cout << "\n";
            }
            std::cout << std::setw(3) << content.at(i).steps;
        }
        std::cout << "\n";
    }

protected:
    Team elves, goblins;
    std::vector<Cell> content;
    int _x_size, _y_size;
};

#if 0
#######
#E..G.#
#...#.#
#.G.#G#
#######

#########
#G..G..G#
#.......#
#.......#
#G..E..G#
#.......#
#.......#
#G..G..G#
#########
#endif

int main() {

    Mapa mapa(std::cin);

    unsigned turns = 0;
    while(!mapa.turn()) { ++turns; }

    std::cout << "Turns=" << turns << "\n";

    return 0;
}
