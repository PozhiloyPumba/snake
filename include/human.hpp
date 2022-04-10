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
        enum class dir {
            UP,
            LEFT,
            DOWN,
            RIGHT
        };

        std::list<coord_t> body_;
        dir direction_ = dir::RIGHT;

        void setSnake (const coord_t &begin) {
            for (int i = 0; i < 5; ++i)
                body_.push_back ({begin.first - i, begin.second});
        }

        virtual void clearCache () {}
    };

    class Human final : public Snake {
        std::vector <std::string> buttons_;

        inline void buttonHandler (dir direction)
        {
            switch (direction){
                case Snake::dir::UP:   
                    if (direction_ != dir::DOWN)
                        direction_ = dir::UP;
                    printf ("up\n");
                    break;
                case Snake::dir::DOWN:
                    if (direction_ != dir::UP)
                        direction_ = dir::DOWN;
                    printf ("down\n");
                    break;
                case Snake::dir::LEFT:
                    if (direction_ != dir::RIGHT)
                        direction_ = dir::LEFT;
                    printf ("left\n");
                    break;
                case Snake::dir::RIGHT:
                    if (direction_ != dir::LEFT)
                        direction_ = dir::RIGHT;
                    printf ("right\n");
                    break;
            }
        }
        void initHuman (const std::initializer_list<std::string> &buttons);

    public:
        Human (const std::initializer_list<std::string> &buttons);
        Human (const std::string &defaultVariant);

        void clearCache () override;
    };

    class StupidBot final : public Snake {
        gameModel::Game *model_ = nullptr;
        
    public:
        StupidBot ();
        void setModel (gameModel::Game *model) { model_ = model; }
    };
}  // namespace Control

#endif