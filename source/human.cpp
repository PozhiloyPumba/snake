#include "human.hpp"

#include <queue>

#include "model.hpp"
#include "view.hpp"

namespace Control {
    Snake::~Snake () {}

    Human::Human (const std::vector<std::string> &buttons, const std::string &name) : Snake (controlType::HUMAN, name)  // (up, left, down, right)
    {
        if (buttons.size () != 4)
            throw std::logic_error ("you choose the wrong quantity of buttons");
        
        std::set <std::string> unique;
        std::copy (buttons.begin (), buttons.end (), std::insert_iterator (unique, unique.end ()));
        
        if (unique.size () != 4)
            throw std::logic_error ("you have two or more same buttons");

        std::copy (buttons.begin (), buttons.end (), std::back_insert_iterator (buttons_));
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

        std::for_each (buttons_.begin (), buttons_.end (), [this, v, e = 0] (auto but) mutable { v->addButton (but, std::bind (&Human::buttonHandler, this, static_cast<dir> (e++))); });
    }

    Bot::~Bot () {};

    int Bot::indexFromPair (const coord_t &pair) const
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        return (pair.second - 1) * (termSize.first - 2) + pair.first - 1;
    }

    coord_t Bot::pairFromIndex (int index) const
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        int x = index % (termSize.first - 2) + 1;
        int y = (index - x + 1) / (termSize.first - 2) + 1;

        return {x, y};
    }

    std::vector<int> Bot::getNeighbours (const int curCeilIndex)
    {
        auto coords = pairFromIndex (curCeilIndex);

        auto termSize = graphicInterface::View::get ()->getTermSize ();
        std::vector<int> neighbours;

        // clang-format off
        neighbours.push_back ((coords.second - 1 > 0) ? indexFromPair ({coords.first, coords.second - 1}) : -1);                    // up
        neighbours.push_back ((coords.first  - 1 > 0) ? indexFromPair ({coords.first - 1, coords.second}) : -1);                    // left
        neighbours.push_back ((coords.second + 1 < termSize.second - 1) ? indexFromPair ({coords.first, coords.second + 1}) : -1);  // down
        neighbours.push_back ((coords.first  + 1 < termSize.first  - 1) ? indexFromPair ({coords.first + 1, coords.second}) : -1);  // right
        // clang-format on

        return neighbours;
    }

    void SmartBot::step ()  // BFS
    {
        std::set<int> inQueue;
        std::queue<std::pair<int, dir>> queue;

        auto startNeighbours = getNeighbours (indexFromPair (body_.front ()));
        int i = -1;

        for (auto n : startNeighbours) {
            ++i;

            if (available_->find (n) != available_->end ()) {
                queue.push ({n, static_cast<dir> (i)});
                inQueue.insert (n);
            }
            else {
                if (food_->find (n) != food_->end ()) {
                    direction_ = static_cast<dir> (i);
                    return;
                }
            }
        }

        while (!queue.empty ()) {
            auto curCeilIndex = queue.front ();
            queue.pop ();

            std::vector<int> neighbours = getNeighbours (curCeilIndex.first);

            for (auto n : neighbours) {
                if (inQueue.find (n) != inQueue.end ())
                    continue;

                if (available_->find (n) != available_->end ()) {
                    queue.push ({n, curCeilIndex.second});
                    inQueue.insert (n);
                }
                else {
                    if (food_->find (n) != food_->end ()) {
                        direction_ = curCeilIndex.second;
                        return;
                    }
                }
            }
        }
    }

    void StupidBot::step ()
    {
        std::vector<dir> queue;

        auto startNeighbours = getNeighbours (indexFromPair (body_.front ()));
        int i = -1;

        for (auto n : startNeighbours) {
            ++i;

            if (available_->find (n) != available_->end ())
                queue.push_back (static_cast<dir> (i));
            else {
                if (food_->find (n) != food_->end ()) {
                    direction_ = static_cast<dir> (i);
                    return;
                }
            }
        }

        if (queue.size ()) {
            std::uniform_int_distribution<int> rand (0, queue.size () - 1);
            auto rnd = rand (generator_);
            direction_ = queue[rnd];
        }
    }

}  // namespace Control