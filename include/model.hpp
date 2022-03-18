#ifndef MODEL_HPP
#define MODEL_HPP

#include <list>
#include <random>
#include <utility>
#include <random>
#include <chrono>


namespace gameModel {
    using coord_t = std::pair<unsigned short, unsigned short>;
    
    struct Snake {
        enum class dir {
            UP,
            DOWN,
            RIGHT,
            LEFT
        };

        std::list<coord_t> body_;
        dir direction_ = dir::RIGHT;
    };

    class Game final {
    private:
        const int nRabbits_ = 10;
        std::mt19937 generator {static_cast<long unsigned int>(std::chrono::system_clock::now ().time_since_epoch ().count ())};
        std::list<coord_t> rabbits_;
        Snake snake_;

    public:
        Game ();

        coord_t getNewRandomPair ();

        void drawAll ();
    };
}

#endif