#include "model.hpp"

#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <iterator>

#include "view.hpp"

namespace gameModel {

    Game::Game ()
    {
        auto v = graphicInterface::View::get ();
        v->drawing = std::bind (&Game::drawAll, this);
        v->setCoordObjs = std::bind (&Game::controller, this);
        v->botsHandler = std::bind (&Game::botsHandler, this);
        v->writeScoreTable = std::bind (&Game::drawScore, this);

        auto size = v->getTermSize ();
        auto fieldSize = (size.first - 2) * (size.second - 2);

        std::generate_n (
            std::inserter (available_, available_.end ()),
            fieldSize - 1,
            [n = 0] () mutable { return ++n; });

        for (int i = 0; i < nRabbits_; ++i) {
            if (!available_.size ())
                break;
            rabbits_.push_back (getNewRandomPair ());
            auto last = rabbits_.back ();
            available_.erase (indexFromPair (last));
        }
    }

    coord_t Game::getNewRandomPair ()
    {
        std::uniform_int_distribution<int> rand (0, available_.size () - 1);
        auto rnd = rand (generator_);
        auto reqIt = std::next (available_.begin (), rnd);
        return pairFromIndex (*reqIt);
    }

    int Game::indexFromPair (const coord_t &pair)
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        return (pair.second - 1) * (termSize.first - 2) + pair.first - 1;
    }

    coord_t Game::pairFromIndex (int index)
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        int x = index % (termSize.first - 2) + 1;
        int y = (index - x + 1) / (termSize.first - 2) + 1;

        return {x, y};
    }

    void Game::drawScore () const
    {
        auto v = graphicInterface::View::get ();

        std::for_each (tableScore_.begin (), tableScore_.end (), [v] (auto line) { v->write (line);});
    }

    void Game::drawAll () const
    {
        auto v = graphicInterface::View::get ();
        v->drawFrame ();

        std::for_each (rabbits_.begin (), rabbits_.end (), [v] (auto r) { v->paint (r);});
        std::for_each (snakes_.begin (), snakes_.end (), [v] (auto s) { v->paint (*s);});
    }

    bool Game::checkSnakeCrash ()
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        for (auto curIt = snakes_.begin (), endIt = snakes_.end (); curIt != snakes_.end ();) {
            auto prevIt = curIt;
            ++curIt;

            auto head = (*prevIt)->body_.front ();

            auto deletingSnake = [this, &prevIt] (auto correctionFactor) {
                (*prevIt)->clearCache ();
                tableScore_.insert ({(*prevIt)->name_, (*prevIt)->getLength () + correctionFactor});
                for (auto elem : (*prevIt)->body_)
                    available_.insert (indexFromPair (elem));
                delete *prevIt;
                snakes_.erase (prevIt);
            };

            if (head.first <= 0 || head.first >= termSize.first - 1) {  // x-crash
                (*prevIt)->body_.pop_front ();
                deletingSnake (1);
                continue;
            }
            if (head.second <= 0 || head.second >= termSize.second - 1) {  // y-crash
                (*prevIt)->body_.pop_front ();
                deletingSnake (1);
                continue;
            }

            bool crash = false;
            for (auto nextIt = snakes_.begin (); nextIt != endIt; ++nextIt) {
                if (nextIt == prevIt) {  // self crash
                    for (auto snakeCurIt = std::next ((*prevIt)->body_.begin ()), snakeEndIt = (*prevIt)->body_.end (); snakeCurIt != snakeEndIt; ++snakeCurIt)
                        if (head == (*snakeCurIt)) {
                            crash = true;
                            break;
                        }
                }
                else {  // neighbour crash
                    for (auto segment : (*nextIt)->body_)
                        if (head == segment) {
                            crash = true;
                            break;
                        }
                }
                if (crash) {
                    deletingSnake (0);
                    break;
                }
            }
        }

        return false;
    }

    void Game::addGamer (const Control::Human &ctrl)
    {
        auto v = graphicInterface::View::get ()->getTermSize ();
        auto *hum = new Control::Human (ctrl);
        hum->setSnake ({v.first / 5, (v.second / 2 + snakes_.size () * 2) % (v.second - 2) + 1});

        std::for_each (hum->body_.begin (), hum->body_.end (), [this] (auto forErase) {available_.erase (indexFromPair (forErase));});

        hum->setButtons ();
        snakes_.push_back (hum);
    }

    void Game::addGamer (const Control::StupidBot &ctrl)
    {
        auto v = graphicInterface::View::get ()->getTermSize ();
        auto *bot = new Control::StupidBot (ctrl);

        bot->setSnake ({v.first / 5, (v.second / 2 + snakes_.size () * 2) % (v.second - 2) + 1});
        snakes_.push_back (bot);

        std::for_each (bot->body_.begin (), bot->body_.end (), [this] (auto forErase) {available_.erase (indexFromPair (forErase));});

        bot->setModel (this);
    }

    void Game::snakeStep (Control::Snake &s)
    {
        s.prevDir_ = s.direction_;
        auto head = s.body_.front ();
        bool neSiel = true;

        coord_t newHead;

        switch (s.direction_) {
            case Control::Snake::dir::UP: newHead = {head.first, head.second - 1}; break;
            case Control::Snake::dir::DOWN: newHead = {head.first, head.second + 1}; break;
            case Control::Snake::dir::LEFT: newHead = {head.first - 1, head.second}; break;
            case Control::Snake::dir::RIGHT: newHead = {head.first + 1, head.second}; break;
        }

        s.body_.push_front (newHead);
        available_.erase (indexFromPair (newHead));

        for (auto curIt = rabbits_.begin (), endIt = rabbits_.end (); curIt != endIt; ++curIt) {
            if ((*curIt) == newHead) {
                rabbits_.erase (curIt);

                neSiel = false;
                if (available_.size ()) {
                    rabbits_.push_back (getNewRandomPair ());
                    auto last = rabbits_.back ();
                    available_.erase (indexFromPair (last));
                }
                break;
            }
        }

        if (neSiel) {
            available_.insert (indexFromPair (s.body_.back ()));
            s.body_.pop_back ();
        }
    }

    void Game::botsHandler ()
    {
        auto curIt = snakes_.begin (), endIt = snakes_.end ();
        while (curIt != endIt) {
            curIt = std::find_if (curIt, endIt, [] (auto &sn) { return sn->whoami == Control::Snake::controlType::BOT; });
            if (curIt != endIt) {
                auto *bot = static_cast<Control::StupidBot *> (*(curIt++));
                bot->step ();
            }
        }
    }

    int Game::controller ()
    {
        for (auto &s : snakes_)
            snakeStep (*s);

        checkSnakeCrash ();

        if (snakes_.empty ())
            return 1;

        return 0;
    }
}  // namespace gameModel
