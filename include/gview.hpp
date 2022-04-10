#ifndef GVIEW_HPP
#define GVIEW_HPP

#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "view.hpp"

namespace graphicInterface {
    class GView final : public View {
    private:
        int delay_;
        const size_t ceilSize_ = 20;
        sf::RenderWindow window_;

        std::unordered_map<sf::Keyboard::Key, std::function<void ()>> buttonTable_;
        sf::Keyboard::Key fromStringtoKey (const std::string &button); //TODO: make this beautiful function
        std::pair<unsigned short, unsigned short> virtSize_ = {0, 0};

    public:
        GView (int fps = 120) : delay_ (1000000 / fps) {}

        ~GView () override;


        void run () override;

        std::pair<unsigned short, unsigned short> getTermSize () const override { return virtSize_; }
        void paint (std::pair<unsigned short, unsigned short> &rabbit) override {}                  // TODO:
        void paint (Control::Snake &snake) override {}                                              // TODO:    сколопендра
        void drawFrame () override;                                                                 // TODO:
        void endHandler () override {}

        void addButton (const std::string &button, const std::function<void ()> &handler) override { buttonTable_.insert ({fromStringtoKey (button), handler}); }
        void eraseButton (const std::string &button) override {
            auto res = buttonTable_.find (fromStringtoKey (button));
            if (res != buttonTable_.end ())
                buttonTable_.erase (res);
        }

    };
}  // namespace graphicInterface

#endif