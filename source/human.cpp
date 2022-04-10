#include "human.hpp"
#include "model.hpp"
#include "view.hpp"

namespace Control {

    void Human::initHuman (const std::initializer_list<std::string> &buttons) // (up, left, down, right)
    {
        if (buttons.size () != 4)   throw std::logic_error ("you choose the wrong quantity of buttons");
        
        auto v = graphicInterface::View::get ();

        dir e = dir::UP;

        for (auto curIt = buttons.begin (), endIt = buttons.end (); curIt != endIt; ++curIt) {
            v->addButton (*curIt, std::bind (&Human::buttonHandler, this, e));
            e = static_cast<dir>(static_cast<std::underlying_type<dir>::type>(e) + 1);
            buttons_.push_back (*curIt);
        }
    }

    Human::Human (const std::initializer_list<std::string> &buttons)
    {
        initHuman (buttons);
    }

    Human::Human (const std::string &defaultVariant)
    {
        if (defaultVariant == "arrows") { initHuman ({"\e[A", "\e[D", "\e[B", "\e[C"}); return;}
        if (defaultVariant == "wasd") { initHuman ({"w", "a", "s", "d"}); return;}
        throw std::invalid_argument ("I don't know this default combination");
    }

    void Human::clearCache ()
    {
        auto v = graphicInterface::View::get ();
        
        for (auto but: buttons_)
            v->eraseButton (but);
    }

    StupidBot::StupidBot ()
    {

    }
}  // namespace Control