#include "human.hpp"

#include "view.hpp"

namespace Control {
    Human::Human (const std::string &up, const std::string &left, const std::string &down, const std::string &right)
    {
        auto v = graphicInterface::View::get ();

        v->buttonHandler_.insert ({up, std::bind (&Human::upHandler, this)});
        v->buttonHandler_.insert ({down, std::bind (&Human::downHandler, this)});
        v->buttonHandler_.insert ({left, std::bind (&Human::leftHandler, this)});
        v->buttonHandler_.insert ({right, std::bind (&Human::rightHandler, this)});
    }
}  // namespace Control