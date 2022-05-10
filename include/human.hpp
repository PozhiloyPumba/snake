#ifndef HUMAN_HPP
#define HUMAN_HPP

#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace Control {
    using coord_t = std::pair<int, int>;

    struct Snake {
        enum class controlType {
            HUMAN,
            BOT
        };

        enum class dir {
            UP,
            LEFT,
            DOWN,
            RIGHT
        };

        std::list<coord_t> body_;
        dir prevDir_ = dir::RIGHT;  // it is for fix rotate to itself
        dir direction_ = dir::RIGHT;
        controlType whoami;
        std::string name_;

        void setSnake (const coord_t &begin)
        {
            for (int i = 0; i < 4; ++i)
                if (begin.first - i > 0)
                    body_.push_back ({begin.first - i, begin.second});
        }

        size_t getLength () const { return body_.size (); }

        virtual void clearCache () {}
        Snake (const controlType &type, const std::string &name) : whoami (type), name_ (name) {}
        virtual ~Snake () = 0;
    };

    class Human final : public Snake {
        static inline int numberOfPlayers_ = 0;
        std::set<std::string> buttons_;

        inline void buttonHandler (dir direction)
        {
            switch (direction) {
                case Snake::dir::UP:
                    if (prevDir_ != dir::DOWN)
                        direction_ = dir::UP;
                    break;
                case Snake::dir::DOWN:
                    if (prevDir_ != dir::UP)
                        direction_ = dir::DOWN;
                    break;
                case Snake::dir::LEFT:
                    if (prevDir_ != dir::RIGHT)
                        direction_ = dir::LEFT;
                    break;
                case Snake::dir::RIGHT:
                    if (prevDir_ != dir::LEFT)
                        direction_ = dir::RIGHT;
                    break;
            }
        }

    public:
        Human (const std::vector<std::string> &buttons, const std::string &name);
        void setButtons ();

        void clearCache () override;

        ~Human () override = default;
    };

    class StupidBot final : public Snake {
        static inline int numberOfStupidBots_ = 0;
        std::set<int> *available_ = nullptr;
        std::set<int> *food_ = nullptr;

        int indexFromPair (const coord_t &pair) const;
        coord_t pairFromIndex (int index) const;
        std::vector<int> getNeighbours (const int curCeilIndex);

    public:
        StupidBot ();
        void setAvailable (std::set<int> &available) { available_ = &available; }
        void setFood (std::set<int> &food) { food_ = &food; }
        void step ();
        ~StupidBot () override = default;
    };
}  // namespace Control

#endif