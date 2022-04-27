#ifndef HUMAN_HPP
#define HUMAN_HPP

#include <list>
#include <string>
#include <utility>
#include <vector>

namespace gameModel {
    class Game;
}

namespace Control {
    using coord_t = std::pair<unsigned short, unsigned short>;

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
        dir direction_ = dir::RIGHT;
        controlType whoami;

        void setSnake (const coord_t &begin)
        {
            for (int i = 0; i < 4; ++i)
                body_.push_back ({begin.first - i, begin.second});
        }

        virtual void clearCache () {}
        Snake (const controlType &type) : whoami (type) {}
        virtual ~Snake () = 0;
    };

    class Human final : public Snake {
        std::vector<std::string> buttons_;

        inline void buttonHandler (dir direction)
        {
            switch (direction) {
                case Snake::dir::UP:
                    if (direction_ != dir::DOWN)
                        direction_ = dir::UP;
                    break;
                case Snake::dir::DOWN:
                    if (direction_ != dir::UP)
                        direction_ = dir::DOWN;
                    break;
                case Snake::dir::LEFT:
                    if (direction_ != dir::RIGHT)
                        direction_ = dir::LEFT;
                    break;
                case Snake::dir::RIGHT:
                    if (direction_ != dir::LEFT)
                        direction_ = dir::RIGHT;
                    break;
            }
        }
        void initHuman (const std::initializer_list<std::string> &buttons);

    public:
        Human (const std::initializer_list<std::string> &buttons);
        Human (const std::string &defaultVariant);

        void clearCache () override;

        ~Human () override {}
    };

    class StupidBot final : public Snake {
        gameModel::Game *model_ = nullptr;

    public:
        StupidBot ();
        void setModel (gameModel::Game *model) { model_ = model; }
        void step ();
        ~StupidBot () override {}
    };
}  // namespace Control

#endif