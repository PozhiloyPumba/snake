#include "human.hpp"
#include "model.hpp"
#include "view.hpp"

namespace Control {
    Human::Human (const std::initializer_list<std::string> &buttons)// (const std::string &up, const std::string &left, const std::string &down, const std::string &right)
    {
        if (buttons.size () != 4)   throw std::logic_error ("you choose the wrong quantity of buttons");
        
        auto v = graphicInterface::View::get ();

        dir e = dir::UP;

        for (auto curIt = buttons.begin (), endIt = buttons.end (); curIt != endIt; ++curIt) {
            v->buttonHandler_.insert ({*curIt, std::bind (&Human::buttonHandler, this, e)});
            e = static_cast<dir>(static_cast<std::underlying_type<dir>::type>(e) + 1);
            buttons_.push_back (*curIt);
        }
    }

    void Human::clearCache ()
    {
        auto v = graphicInterface::View::get ();

        for (auto but: buttons_) {
            auto res = v->buttonHandler_.find (but);
            if (res != v->buttonHandler_.end ())
                v->buttonHandler_.erase (res);
        }
    }

    StupidBot::StupidBot ()
    {

    }
}  // namespace Control