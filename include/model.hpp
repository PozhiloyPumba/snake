#ifndef MODEL_HPP
#define MODEL_HPP

#include <algorithm>
#include <chrono>
#include <list>
#include <set>
#include <random>
#include <utility>
#include <iomanip>

#include "human.hpp"

namespace gameModel {
    using coord_t = std::pair<unsigned short, unsigned short>;

    class Game final {
    private:
        const int nRabbits_ = 10;
        std::mt19937 generator_{static_cast<long unsigned int> (std::chrono::system_clock::now ().time_since_epoch ().count ())};
        std::list<coord_t> rabbits_;
        std::list<Control::Snake *> snakes_;

        using ScoreLine = std::pair<std::string, size_t>;
        static inline std::function<bool (ScoreLine, ScoreLine)> tablePred_ = [] (const ScoreLine &lhs, const ScoreLine &rhs) {return lhs.second > rhs.second;};

        std::multiset<ScoreLine, decltype (tablePred_)> tableScore_ {tablePred_};

        coord_t getNewRandomPair ();
        void drawScore ();
        void drawAll ();
        int controller ();
        bool checkSnakeCrash ();
        bool checkWin ();
        void snakeStep (Control::Snake &s);
        void botsHandler ();

    public:
        Game ();
        void addGamer (Control::Human &ctrl);    // it is copy for fix dead objects in main
        void addGamer (Control::StupidBot &ctrl);
    };
}  // namespace gameModel

#endif