#include "model.hpp"
#include "view.hpp"

namespace gameModel {
    
    Game::Game ()
    {
        auto v = graphicInterface::View::get();
        v->drawing = std::bind (&Game::drawAll, this);

        for (int i = 0; i < nRabbits_; ++i)
            rabbits_.push_back (getNewRandomPair());

        for (int i = 0; i < 4; ++i)
            snake_.body_.push_back ({v->getTermSize ().first / 5 - i, v->getTermSize ().second / 2});
    }

    coord_t Game::getNewRandomPair ()
    {
        std::pair <unsigned short, unsigned short> termSize = graphicInterface::View::get()->getTermSize();

        std::uniform_int_distribution<unsigned short> randFirst (1, termSize.first - 2);
        std::uniform_int_distribution<unsigned short> randSecond (1, termSize.second - 2);

        return {randFirst(generator), randSecond(generator)};
    }

    void Game::drawAll ()
    {
        std::shared_ptr<graphicInterface::View> v = graphicInterface::View::get();

        for (auto r: rabbits_)
            v->paint(r);

        v->paint (snake_);
    }
}