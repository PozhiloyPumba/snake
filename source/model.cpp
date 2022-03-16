#include "model.hpp"

namespace gameModel {
    
    void game::addNewRabbit ()
    {
        std::pair <unsigned short, unsigned short> termSize = graphicInterface::View::get()->getTermSize();

        std::uniform_int_distribution<unsigned short> randFirst (1, termSize.first - 2);
        std::uniform_int_distribution<unsigned short> randSecond (1, termSize.second - 2);

        rabbits_.push_back ({randFirst(generator), randSecond(generator)});
    }

    void game::drawAll ()
    {
        std::shared_ptr<graphicInterface::View> v = graphicInterface::View::get();

        for (auto r: rabbits_) {
            v->paint(r);
        }
    }
}