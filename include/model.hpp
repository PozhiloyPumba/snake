#ifndef MODEL_HPP
#define MODEL_HPP

#include <list>
#include <random>
#include <utility>
#include <random>
#include <chrono>

#include "view.hpp"

namespace gameModel {
    
    class game final {
    private:
        const int nRabbits_ = 2000;
        std::mt19937 generator {static_cast<long unsigned int>(std::chrono::system_clock::now ().time_since_epoch ().count ())};
        std::list<std::pair<unsigned short, unsigned short>> rabbits_;

    public:
        game ()
        {
            graphicInterface::View::get()->drawing = std::bind (&game::drawAll, this);
            for (int i = 0; i < nRabbits_; ++i)
                addNewRabbit();
        }

        void addNewRabbit ();

        void drawAll ();
    };
}

#endif