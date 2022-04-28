#ifndef MODEL_HPP
#define MODEL_HPP

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <list>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "human.hpp"

namespace gameModel {
    using coord_t = std::pair<int, int>;

    class Game final {
    private:
        const int nRabbits_ = 10;
        std::mt19937 generator_{static_cast<long long unsigned> (std::chrono::system_clock::now ().time_since_epoch ().count ())};
        std::list<coord_t> rabbits_;
        std::list<Control::Snake *> snakes_;
        std::vector<int> available_;

        using ScoreLine = std::pair<std::string, size_t>;
        static inline std::function<bool (ScoreLine, ScoreLine)> tablePred_ = [] (const ScoreLine &lhs, const ScoreLine &rhs) { return lhs.second > rhs.second; };

        std::multiset<ScoreLine, decltype (tablePred_)> tableScore_{tablePred_};

        coord_t getNewRandomPair ();
        void drawScore () const;
        void drawAll () const;
        int controller ();
        bool checkSnakeCrash ();
        void snakeStep (Control::Snake &s);
        void botsHandler ();
        int indexFromPair (const coord_t &pair);
        coord_t pairFromIndex (int index);

    public:
        Game ();
        void addGamer (const Control::Human &ctrl);
        void addGamer (const Control::StupidBot &ctrl);
    };
}  // namespace gameModel

#endif