#ifndef HUMAN_HPP
#define HUMAN_HPP

#include <list>
#include <string>
#include <utility>

namespace Control {
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

        Snake (const coord_t &begin) {
            for (int i = 0; i < 5; ++i)
                body_.push_back ({begin.first - i, begin.second});
        }
    };

    class Human final {
        Snake *snake_ = nullptr;

        inline void upHandler ()
        {
            if (snake_->direction_ != Control::Snake::dir::DOWN)
                snake_->direction_ = Control::Snake::dir::UP;
        }
        inline void downHandler ()
        {
            if (snake_->direction_ != Control::Snake::dir::UP)
                snake_->direction_ = Control::Snake::dir::DOWN;
        }
        inline void rightHandler ()
        {
            if (snake_->direction_ != Control::Snake::dir::LEFT)
                snake_->direction_ = Control::Snake::dir::RIGHT;
        }
        inline void leftHandler ()
        {
            if (snake_->direction_ != Control::Snake::dir::RIGHT)
                snake_->direction_ = Control::Snake::dir::LEFT;
        }

    public:
        void setSnake (Snake &s) { snake_ = &s; }
        Human (const std::string &up, const std::string &left, const std::string &down, const std::string &right);
    };
}  // namespace Control

#endif