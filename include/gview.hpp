#ifndef GVIEW_HPP
#define GVIEW_HPP

#include "view.hpp"

namespace graphicInterface {
    class GView final : public View {
    private:
        int delay_;

    public:
        GView (int fps = 120) : delay_ (1000000 / fps) {}

        ~GView () override;

        void drawScene ();
        void run () override;

        std::pair<unsigned short, unsigned short> getTermSize () const override { return {0, 0}; }  // TODO:
        void paint (std::pair<unsigned short, unsigned short> &rabbit) override {}                  // TODO:
        void paint (gameModel::Snake &snake) override {}                                            // TODO:
        void drawFrame () override {}                                                               // TODO:
        void endHandler () override {}
    };
}  // namespace graphicInterface

#endif