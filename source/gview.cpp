#include "gview.hpp"

namespace graphicInterface {
    GView::~GView ()
    {
        std::cout << "graph destroy" << std::endl;
    }

    void GView::drawScene ()
    {
        std::cout << "todo graph draw" << std::endl;
    }

    void GView::run ()
    {
        bool end = false;
        while (!end) {
            end = true;
        }
    }
}  // namespace graphicInterface