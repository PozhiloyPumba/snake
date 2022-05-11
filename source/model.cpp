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
        v->resizeHandler = std::bind (&Game::setAvailablefields, this);
        v->addPlayerHandler = std::bind (&Game::addGamer, this, std::placeholders::_1, std::placeholders::_2);
        v->addBotHandler = std::bind (&Game::addBot, this, std::placeholders::_1);
    }

    Game::~Game ()
    {
        for (auto s : snakes_)
            delete s;
    }

    void Game::setAvailablefields ()
    {
        available_.clear ();
        rabbits_.clear ();

        auto size = graphicInterface::View::get ()->getTermSize ();
        auto fieldSize = (size.first - 2) * (size.second - 2);

        std::generate_n (
            std::inserter (available_, available_.end ()),
            fieldSize - 1,
            [n = 0] () mutable { return ++n; });

        for (auto s : snakes_)
            std::for_each (s->body_.begin (), s->body_.end (), [this] (auto forErase) { available_.erase (indexFromPair (forErase)); });

        for (int i = 0; i < nRabbits_; ++i) {
            if (!available_.size ())
                break;
            auto last = indexFromPair (getNewRandomPair ());
            rabbits_.insert (last);
            available_.erase (last);
        }
    }

    coord_t Game::getNewRandomPair ()
    {
        std::uniform_int_distribution<int> rand (0, available_.size () - 1);
        auto rnd = rand (generator_);
        auto reqIt = std::next (available_.begin (), rnd);
        return pairFromIndex (*reqIt);
    }

    int Game::indexFromPair (const coord_t &pair) const
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        return (pair.second - 1) * (termSize.first - 2) + pair.first - 1;
    }

    coord_t Game::pairFromIndex (int index) const
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        int x = index % (termSize.first - 2) + 1;
        int y = (index - x + 1) / (termSize.first - 2) + 1;

        return {x, y};
    }

    void Game::drawScore () const
    {
        auto v = graphicInterface::View::get ();

        std::for_each (tableScore_.begin (), tableScore_.end (), [v] (auto line) { v->write (line); });
    }

    void Game::drawAll () const
    {
        auto v = graphicInterface::View::get ();
        v->drawFrame ();

        std::for_each (rabbits_.begin (), rabbits_.end (), [this, v] (auto r) { v->paint (pairFromIndex (r)); });
        std::for_each (snakes_.begin (), snakes_.end (), [v] (auto s) { v->paint (*s); });
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

    void Game::addGamer (const std::vector<std::string> &buttons, const std::string &name)
    {
        static int countDefault = 0;
        auto v = graphicInterface::View::get ()->getTermSize ();
        Control::Human *hum;
        try {
            if (name == "")
                hum = new Control::Human (buttons,
                                        "Player" + std::to_string (++countDefault));
            else
                hum = new Control::Human (buttons, name);

            hum->setSnake ({v.first / 5, (v.second / 2 + snakes_.size () * 2) % (v.second - 2) + 1});

            std::for_each (hum->body_.begin (), hum->body_.end (), [this] (auto forErase) { available_.erase (indexFromPair (forErase)); });
            
            hum->setButtons ();
            snakes_.push_back (hum);
        }
        catch (std::logic_error &e) {       // it is for fix the same buttons
            std::cout << e.what () << std::endl;
            graphicInterface::View::get ()->alert ();
            hum->clearCache ();
        }
    }

    void Game::addBot (Control::Bot::TypeOfBot typeOfBot)
    {
        auto v = graphicInterface::View::get ()->getTermSize ();
        
        auto fillBot = [&] (auto bot) {
            bot->setSnake ({v.first / 5, (v.second / 2 + snakes_.size () * 2) % (v.second - 2) + 1});
            snakes_.push_back (bot);

            std::for_each (bot->body_.begin (), bot->body_.end (), [this] (auto forErase) { available_.erase (indexFromPair (forErase)); });

            bot->setAvailable (available_);
            bot->setFood (rabbits_);
        };

        if (typeOfBot == Control::Bot::TypeOfBot::SMART) {
            auto *bot = new Control::SmartBot ();
            fillBot (bot);
        }
        else {
            auto *bot = new Control::StupidBot ();
            fillBot (bot);
        }

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
            if (pairFromIndex (*curIt) == newHead) {
                rabbits_.erase (curIt);

                neSiel = false;
                if (available_.size ()) {
                    auto last = indexFromPair (getNewRandomPair ());
                    rabbits_.insert (last);
                    available_.erase (last);
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
                auto *bot = static_cast<Control::Bot *> (*(curIt++));
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
