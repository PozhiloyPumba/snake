#ifndef GVIEW_HPP
#define GVIEW_HPP

#include "view.hpp"

namespace graphicInterface {
    class GView final : public View {
    private:
        int delay_;

    public:
        GView (int fps = 24) : delay_ (1000000 / fps) {}

        ~GView () override;

        void drawScene ();
        void run () override;
    };
}  // namespace graphicInterface

#endif