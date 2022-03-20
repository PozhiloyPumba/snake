#ifndef MODEL_HPP
#define MODEL_HPP

#include <poll.h>
#include <signal.h>

#include <chrono>
#include <list>
#include <random>
#include <utility>

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

        bool checkSelfDestruction ();
    };

    class Game final {
    private:
        const int nRabbits_ = 10;
        const int beginSnakeLen_ = 5;
        std::mt19937 generator_{static_cast<long unsigned int> (std::chrono::system_clock::now ().time_since_epoch ().count ())};
        std::list<coord_t> rabbits_;
        Snake snake_;
        coord_t getNewRandomPair ();
        void drawAll ();
        bool controller ();
        void buttonHandler ();
        bool checkSnakeCrash ();

    public:
        Game ();
    };
}  // namespace gameModel

#endif