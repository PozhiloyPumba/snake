#include "model.hpp"

#include <unistd.h>

#include "view.hpp"

namespace gameModel {

    Game::Game ()
    {
        auto v = graphicInterface::View::get ();
        v->drawing = std::bind (&Game::drawAll, this);
        v->setCoordObjs = std::bind (&Game::controller, this);
        v->botsHandler = std::bind (&Game::botsHandler, this);
        v->writeScoreTable = std::bind (&Game::drawScore, this);

        for (int i = 0; i < nRabbits_; ++i)
            rabbits_.push_back (getNewRandomPair ());
    }

    coord_t Game::getNewRandomPair ()  // TODO: fix spawn in snake
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        std::uniform_int_distribution<unsigned short> randFirst (1, termSize.first - 2);
        std::uniform_int_distribution<unsigned short> randSecond (1, termSize.second - 2);

        return {randFirst (generator_), randSecond (generator_)};
    }

    void Game::drawScore ()
    {
        auto v = graphicInterface::View::get ();

        for (auto line: tableScore_)
            v->write (line);
    }

    void Game::drawAll ()
    {
        auto v = graphicInterface::View::get ();
        v->drawFrame ();

        for (auto r : rabbits_)
            v->paint (r);

        for (auto s : snakes_)
            v->paint (*s);
    }

    bool Game::checkSnakeCrash ()
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        for (auto curIt = snakes_.begin (), endIt = snakes_.end (); curIt != snakes_.end ();) {
            auto prevIt = curIt;
            ++curIt;

            auto head = (*prevIt)->body_.front ();

            if (head.first <= 0 || head.first >= termSize.first - 1) {  // x-crash
                (*prevIt)->clearCache ();
                tableScore_.insert({(*prevIt)->name_, (*prevIt)->getLength ()});
                delete *prevIt;
                snakes_.erase (prevIt);
                continue;
            }
            if (head.second <= 0 || head.second >= termSize.second - 1) {  // y-crash
                (*prevIt)->clearCache ();
                tableScore_.insert({(*prevIt)->name_, (*prevIt)->getLength ()});
                delete *prevIt;
                snakes_.erase (prevIt);
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
                    (*prevIt)->clearCache ();
                    tableScore_.insert({(*prevIt)->name_, (*prevIt)->getLength ()});
                    delete *prevIt;
                    snakes_.erase (prevIt);
                    break;
                }
            }
        }

        return false;
    }

    void Game::addGamer (Control::Human &ctrl)
    {
        auto v = graphicInterface::View::get ()->getTermSize ();
        auto *hum = new Control::Human (ctrl);
        hum->setSnake ({v.first / 5, (v.second / 2 + snakes_.size ()) % v.second});

        snakes_.push_back (hum);
    }

    void Game::addGamer (Control::StupidBot &ctrl)
    {
        auto v = graphicInterface::View::get ()->getTermSize ();
        auto *bot = new Control::StupidBot (ctrl);
        bot->setSnake ({v.first / 5, (v.second / 2 + snakes_.size ()) % v.second});

        snakes_.push_back (bot);

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

        for (auto curIt = rabbits_.begin (), endIt = rabbits_.end (); curIt != endIt; ++curIt) {
            if ((*curIt) == newHead) {
                rabbits_.erase (curIt);
                neSiel = false;
                rabbits_.push_back (getNewRandomPair ());
                break;
            }
        }

        if (neSiel)
            s.body_.pop_back ();
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
