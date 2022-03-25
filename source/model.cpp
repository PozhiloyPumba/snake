#include "model.hpp"

#include <unistd.h>

#include "view.hpp"

namespace gameModel {

    // bool Snake::checkSelfDestruction ()
    // {
    //     auto head = body_.front ();

    //     for (auto curIt = ++body_.begin (), endIt = body_.end (); curIt != endIt; ++curIt)
    //         if (head == (*curIt))
    //             return true;

    //     return false;
    // }

    Game::Game ()
    {
        auto v = graphicInterface::View::get ();
        v->drawing = std::bind (&Game::drawAll, this);
        v->setCoordObjs = std::bind (&Game::controller, this);

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

    void Game::drawAll ()
    {
        auto v = graphicInterface::View::get ();
        v->drawFrame ();

        for (auto r : rabbits_)
            v->paint (r);

        for (auto s : snakes_)
            v->paint (s);
    }

    bool Game::checkSnakeCrash ()   //TODO: fix stop program if one of snakes crash
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        for (auto curIt = snakes_.begin (), endIt = snakes_.end (); curIt != endIt; ++curIt) {
    
            auto head = (*curIt).body_.front ();

            if (head.first <= 0 || head.first >= termSize.first - 1)
                return true;
            if (head.second <= 0 || head.second >= termSize.second - 1)
                return true;

            for (auto nextIt = snakes_.begin (); nextIt != endIt; ++nextIt) {
                if (nextIt == curIt) {
                    for (auto snakeCurIt = std::next ((*curIt).body_.begin ()), snakeEndIt = (*curIt).body_.end (); snakeCurIt != snakeEndIt; ++snakeCurIt)
                        if (head == (*snakeCurIt))
                            return true;
                }
                else {
                    for (auto segment: nextIt->body_)
                        if (head == segment)
                            return true;
                }
            }
        }

        return false;
    }

    // TODO:
    // bool Game::checkWin ()
    // {
    //     auto termSize = graphicInterface::View::get ()->getTermSize ();

    //     if (static_cast<int> (snake_.body_.size ()) == (termSize.first - 2) * (termSize.second - 2))
    //         return true;

    //     return false;
    // }

    void Game::addGamer (Control::Human &ctrl)
    {
        auto v = graphicInterface::View::get ();
        Control::Snake s ({v->getTermSize ().first / 5, v->getTermSize ().second / 2 + snakes_.size ()});
        
        snakes_.push_back (s);

        ctrl.setSnake (snakes_.back ());
    }

    void Game::snakeStep (Control::Snake &s)
    {
        auto head = s.body_.front ();
        bool neSiel = true;

        for (auto curIt = rabbits_.begin (), endIt = rabbits_.end (); curIt != endIt; ++curIt) {
            if ((*curIt) == head) {
                rabbits_.erase (curIt);
                neSiel = false;
                rabbits_.push_back (getNewRandomPair ());
                break;
            }
        }

        if (neSiel)
            s.body_.pop_back ();

        coord_t newHead;

        switch (s.direction_) {
            case Control::Snake::dir::UP: newHead = {head.first, head.second - 1}; break;
            case Control::Snake::dir::DOWN: newHead = {head.first, head.second + 1}; break;
            case Control::Snake::dir::LEFT: newHead = {head.first - 1, head.second}; break;
            case Control::Snake::dir::RIGHT: newHead = {head.first + 1, head.second}; break;
        }

        s.body_.push_front (newHead);
    }

    int Game::controller ()
    {
        // if (checkWin ())
        //     return 2;
        if (snakes_.empty ())
            return 0;
        
        for (auto &s : snakes_)
            snakeStep (s);

        if (checkSnakeCrash ())
            return 1;

        return 0;
    }
}  // namespace gameModel
