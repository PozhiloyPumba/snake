#ifndef GVIEW_HPP
#define GVIEW_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "view.hpp"

namespace graphicInterface {
    using SpriteAndTexture = std::pair<sf::Sprite, sf::Texture>;

    class GView final : public View {
    private:
        const unsigned ceilSize_ = 20;
        bool end_ = false;
        int chapterOfCycle_ = 1;
        int alreadyWriten_ = 0;

        sf::RenderWindow window_;
        sf::Sprite spr_;
        sf::Texture freshMeat_;
        std::vector<sf::Texture> head_;
        std::vector<sf::Texture> tail_;
        sf::Font font_;

        std::unordered_map<sf::Keyboard::Key, std::function<void ()>> buttonTable_;
        sf::Keyboard::Key fromStringtoKey (const std::string &button);
        std::pair<unsigned short, unsigned short> virtSize_ = {0, 0};

        void loadTexture (sf::Texture &dest, const std::string &fileName);
        void setTextureInSprite (const sf::Texture &texture);
    public:
        GView ();

        ~GView () override = default;

        void run () override;

        std::pair<unsigned short, unsigned short> getTermSize () const override { return virtSize_; }
        void paint (const std::pair<unsigned short, unsigned short> &rabbit) override;
        void paint (const Control::Snake &snake) override;
        void write (const std::pair <std::string, size_t> &line) override;
        void drawFrame () override;
        void endHandler () override {}

        void addButton (const std::string &button, const std::function<void ()> &handler) override { buttonTable_.insert ({fromStringtoKey (button), handler}); }
        void eraseButton (const std::string &button) override
        {
            auto res = buttonTable_.find (fromStringtoKey (button));
            if (res != buttonTable_.end ())
                buttonTable_.erase (res);
        }
    };
}  // namespace graphicInterface

#endif