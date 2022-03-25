#ifndef MODEL_HPP
#define MODEL_HPP

#include <poll.h>
#include <signal.h>

#include <chrono>
#include <list>
#include <random>
#include <utility>

#include "human.hpp"

namespace gameModel {
    using coord_t = std::pair<unsigned short, unsigned short>;

    class Game final {
    private:
        const int nRabbits_ = 10;
        const int beginSnakeLen_ = 5;
        std::mt19937 generator_{static_cast<long unsigned int> (std::chrono::system_clock::now ().time_since_epoch ().count ())};
        std::list<coord_t> rabbits_;
        std::list<Control::Snake> snakes_;
        coord_t getNewRandomPair ();
        void drawAll ();
        int controller ();
        bool checkSnakeCrash ();
        bool checkWin ();
        void snakeStep (Control::Snake &s);

    public:
        Game ();
        void addGamer (Control::Human &ctrl);
    };
}  // namespace gameModel

#endif