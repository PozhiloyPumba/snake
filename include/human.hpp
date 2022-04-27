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
        dir prevDir_ = dir::RIGHT;   // it is for fix rotate to itself
        dir direction_ = dir::RIGHT;
        controlType whoami;
        std::string name_;

        void setSnake (const coord_t &begin)
        {
            for (int i = 0; i < 4; ++i)
                body_.push_back ({begin.first - i, begin.second});
        }

        size_t getLength () const {
            return body_.size ();
        }

        virtual void clearCache () {}
        Snake (const controlType &type) : whoami (type) {}
        virtual ~Snake () = 0;
    };

    class Human final : public Snake {
        static inline int numberOfPlayers_ = 0;
        std::vector<std::string> buttons_;

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
        void initHuman (const std::initializer_list<std::string> &buttons);

    public:
        Human (const std::initializer_list<std::string> &buttons);
        Human (const std::string &defaultVariant);

        void setName (const std::string &name) {
            name_ = name;
        }

        void clearCache () override;

        ~Human () override {}
    };

    class StupidBot final : public Snake {
        static inline int numberOfStupidBots_ = 0;
        gameModel::Game *model_ = nullptr;

    public:
        StupidBot ();
        void setModel (gameModel::Game *model) { model_ = model; }
        void step ();
        ~StupidBot () override {}
    };
}  // namespace Control

#endif