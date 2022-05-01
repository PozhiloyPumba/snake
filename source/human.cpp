#include <queue>

#include "human.hpp"
#include "model.hpp"
#include "view.hpp"

namespace Control {
    Snake::~Snake () {}

    void Human::initHuman (const std::initializer_list<std::string> &buttons)  // (up, left, down, right)
    {
        name_ = "Player" + std::to_string (++numberOfPlayers_);

        if (buttons.size () != 4)
            throw std::logic_error ("you choose the wrong quantity of buttons");

        std::for_each (buttons.begin (), buttons.end (), [this] (auto but) {buttons_.push_back (but);});
    }

    Human::Human (const std::initializer_list<std::string> &buttons) : Snake (controlType::HUMAN)
    {
        initHuman (buttons);
    }

    Human::Human (const std::string &defaultVariant) : Snake (controlType::HUMAN)
    {
        if (defaultVariant == "arrows") {
            initHuman ({"\e[A", "\e[D", "\e[B", "\e[C"});
            return;
        }
        if (defaultVariant == "wasd") {
            initHuman ({"w", "a", "s", "d"});
            return;
        }
        throw std::invalid_argument ("I don't know this default combination");
    }

    void Human::clearCache ()
    {
        auto v = graphicInterface::View::get ();

        for (auto but : buttons_)
            v->eraseButton (but);
    }

    void Human::setButtons ()
    {
        auto v = graphicInterface::View::get ();

        std::for_each (buttons_.begin (), buttons_.end (), 
                       [this, v, e = 0] (auto but) mutable {v->addButton (but, std::bind (&Human::buttonHandler, this, static_cast <dir> (e++)));});
    }

    StupidBot::StupidBot () : Snake (controlType::BOT)
    {
        name_ = "StupidBot" + std::to_string (++numberOfStupidBots_);
    }

    int StupidBot::indexFromPair (const coord_t &pair) const
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        return (pair.second - 1) * (termSize.first - 2) + pair.first - 1;
    }

    coord_t StupidBot::pairFromIndex (int index) const
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        int x = index % (termSize.first - 2) + 1;
        int y = (index - x + 1) / (termSize.first - 2) + 1;

        return {x, y};
    }

    std::vector<int> StupidBot::getNeighbours (const int curCeilIndex)
    {
        auto coords = pairFromIndex (curCeilIndex);

        auto termSize = graphicInterface::View::get ()->getTermSize ();
        std::vector<int> neighbours;

        neighbours.push_back ((coords.second - 1 > 0) ? indexFromPair ({coords.first, coords.second - 1}) : -1);   // up
        neighbours.push_back ((coords.first - 1 > 0)  ? indexFromPair ({coords.first - 1, coords.second}) : -1);   // left
        neighbours.push_back ((coords.second + 1 < termSize.second - 1) ? indexFromPair ({coords.first, coords.second + 1}) : -1);   // down
        neighbours.push_back ((coords.first + 1 < termSize.first - 1)  ? indexFromPair ({coords.first + 1, coords.second}) : -1);   // right

        return neighbours;
    }

    void StupidBot::step () // simple BFS
    {
        std::set<int> inQueue;
        std::queue<std::pair <int, dir>> queue;

        auto startNeighbours = getNeighbours (indexFromPair (body_.front ()));
        int i = -1;

        for (auto n: startNeighbours) {
            ++i;

            if (auto resAv = available_->find (n); resAv != available_->end ()) {
                queue.push ({n, static_cast<dir> (i)});
                inQueue.insert (n);
            }
            else {
                if (auto resInFood = food_->find (n); resInFood != food_->end ()) {
                    direction_ = static_cast<dir> (i);
                    return;
                }
            }
        }

        while (!queue.empty ()) {
            auto curCeilIndex = queue.front ();
            queue.pop ();

            std::vector<int> neighbours = getNeighbours (curCeilIndex.first);

            for (auto n: neighbours) {
                if (auto resInQ = inQueue.find (n); resInQ != inQueue.end ())
                    continue;

                if (auto resAv = available_->find (n); resAv != available_->end ()) {
                    queue.push ({n, curCeilIndex.second});
                    inQueue.insert (n);
                }
                else {
                    auto resInFood = food_->find (n);
                    if (resInFood != food_->end ()) {
                        direction_ = curCeilIndex.second;
                        return;
                    }
                }
            }
        }
    }
}  // namespace Control