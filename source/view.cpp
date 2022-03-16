#include "view.hpp"

#include "gview.hpp"
#include "tview.hpp"

namespace graphicInterface {

    std::shared_ptr<View> View::get (const std::string &type)
    {
        if (!obj) {
            if (type == textView)
                obj = std::make_shared<TView> ();
            else if (type == graphView)
                obj = std::make_shared<GView> ();
            else
                throw std::logic_error ("You choose the wrong word for graphic try \"text\" or \"graph\"");
        }

        return obj;
    }

    View::~View (){};  // pure virtual dtor
}  // namespace graphicInterface