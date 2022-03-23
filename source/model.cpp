#include <unistd.h>

#include "model.hpp"

#include "view.hpp"

namespace gameModel {

    bool Snake::checkSelfDestruction ()
    {
        auto head = body_.front ();

        for (auto curIt = ++body_.begin (), endIt = body_.end (); curIt != endIt; ++curIt)
            if (head == (*curIt))
                return true;

        return false;
    }

    Game::Game ()
    {
        auto v = graphicInterface::View::get ();
        v->drawing = std::bind (&Game::drawAll, this);
        v->setCoordObjs = std::bind (&Game::controller, this);

        for (int i = 0; i < nRabbits_; ++i)
            rabbits_.push_back (getNewRandomPair ());

        for (int i = 0; i < beginSnakeLen_ && v->getTermSize ().first / 5 - i > 0; ++i)
            snake_.body_.push_back ({v->getTermSize ().first / 5 - i, v->getTermSize ().second / 2});
    }

    coord_t Game::getNewRandomPair ()   //TODO: fix spawn in snake
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

        v->paint (snake_);
    }

    void Game::buttonHandler ()
    {
        struct pollfd in = {0, POLL_IN, 0};

        if (poll (&in, 1, 500) == 1) {
            unsigned char c;
            read (0, &c, 1);

            if (c == '\033') {
                read (0, &c, 1);
                if (c == 'q')
                    graphicInterface::View::get ()->endHandler();
                
                read (0, &c, 1);
                
                switch (c) {
                    case 'A':
                        if (snake_.direction_ != Snake::dir::DOWN)
                            snake_.direction_ = Snake::dir::UP;
                        break;
                    case 'B':
                        if (snake_.direction_ != Snake::dir::UP)
                            snake_.direction_ = Snake::dir::DOWN;
                        break;
                    case 'C':
                        if (snake_.direction_ != Snake::dir::LEFT)
                            snake_.direction_ = Snake::dir::RIGHT;
                        break;
                    case 'D':
                        if (snake_.direction_ != Snake::dir::RIGHT)
                            snake_.direction_ = Snake::dir::LEFT;
                        break;
                    case 'q':
                        graphicInterface::View::get ()->endHandler();
                        break;
                }
            }
            if (c == 'q')
                graphicInterface::View::get ()->endHandler();
        }
    }

    bool Game::checkSnakeCrash ()
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        auto head = snake_.body_.front ();

        if (head.first <= 0 || head.first >= termSize.first - 1)
            return true;
        if (head.second <= 0 || head.second >= termSize.second - 1)
            return true;
        if (snake_.checkSelfDestruction ())
            return true;

        return false;
    }
    
    bool Game::checkWin ()
    {
        auto termSize = graphicInterface::View::get ()->getTermSize ();

        if (snake_.body_.size () == (termSize.first - 2) * (termSize.second - 2))
            return true;

        return false;
    }


    int Game::controller ()  // TODO: refactor this function
    {
        if (checkWin ())
            return 2;
        
        if (checkSnakeCrash ())
            return 1;

        buttonHandler ();

        auto head = snake_.body_.front ();
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
            snake_.body_.pop_back ();

        coord_t newHead;

        switch (snake_.direction_) {
            case Snake::dir::UP: newHead = {head.first, head.second - 1}; break;
            case Snake::dir::DOWN: newHead = {head.first, head.second + 1}; break;
            case Snake::dir::LEFT: newHead = {head.first - 1, head.second}; break;
            case Snake::dir::RIGHT: newHead = {head.first + 1, head.second}; break;
        }

        snake_.body_.push_front (newHead);

        return 0;
    }
}  // namespace gameModel
