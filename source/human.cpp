#include "human.hpp"

#include "model.hpp"
#include "view.hpp"

namespace Control {
    Snake::~Snake () {}

    void Human::initHuman (const std::initializer_list<std::string> &buttons)  // (up, left, down, right)
    {
        name_ = "Player" + std::to_string (++numberOfPlayers_);

        if (buttons.size () != 4)
            throw std::logic_error ("you choose the wrong quantity of buttons");

        for (auto curIt = buttons.begin (), endIt = buttons.end (); curIt != endIt; ++curIt)
            buttons_.push_back (*curIt);
    }

    Human::Human (const std::initializer_list<std::string> &buttons) : Snake (controlType::HUMAN)
    {
        initHuman (buttons);
    }

    Human::Human (const std::string &defaultVariant) : Snake (controlType::HUMAN)
    {
        if (defaultVariant == "arrows") {
            initHuman ({"\e[A", "\e[D", "\e[B", "\e[C"});
            return;
        }
        if (defaultVariant == "wasd") {
            initHuman ({"w", "a", "s", "d"});
            return;
        }
        throw std::invalid_argument ("I don't know this default combination");
    }

    void Human::clearCache ()
    {
        auto v = graphicInterface::View::get ();

        for (auto but : buttons_)
            v->eraseButton (but);
    }

    void Human::setButtons ()
    {
        auto v = graphicInterface::View::get ();

        dir e = dir::UP;

        for (auto curIt = buttons_.begin (), endIt = buttons_.end (); curIt != endIt; ++curIt) {
            v->addButton (*curIt, std::bind (&Human::buttonHandler, this, e));
            e = static_cast<dir> (static_cast<std::underlying_type<dir>::type> (e) + 1);
        }
    }

    StupidBot::StupidBot () : Snake (controlType::BOT)
    {
        name_ = "StupidBot" + std::to_string (++numberOfStupidBots_);
    }

    void StupidBot::step ()  // TODO: make it some smarter)))
    {
        // static int a = 0;
    }
}  // namespace Control