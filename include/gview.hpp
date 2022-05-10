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
        const int ceilSize_ = 25;
        bool end_ = false;
        int chapterOfCycle_ = 1;
        int alreadyWriten_ = 0;
        std::pair<float, float> scale_ = {1, 1};

        sf::RenderWindow window_;
        sf::Sprite spr_;
        sf::Texture freshMeat_;
        sf::Texture frame_;
        std::vector<sf::Texture> head_;
        std::vector<sf::Texture> tail_;
        sf::Font font_;

        std::unordered_map<sf::Keyboard::Key, std::function<void ()>> buttonTable_;
        sf::Keyboard::Key fromStringtoKey (const std::string &button);
        std::pair<int, int> virtSize_;

        void loadTexture (sf::Texture &dest, const std::string &fileName);
        void setTextureInSprite (const sf::Texture &texture);
        void startScreen ();
        void endScreen ();
        void closeAndResizeHelper (const sf::Event &event);
        void menu ();
        void menuAddPlayer ();
        void menuAddBot (int type);
        void createGamer ();
        void createCustomGamer (const std::string &name);
        void chooserKey (std::vector<std::string>::iterator &key);
        void inputName (sf::Text &title, std::string &name);
        void chooseController (sf::Text &title, std::string &name);
        void smallViewAfterAddingPlayer (const std::string &name, Control::Snake::controlType type);

    public:
        GView ();

        ~GView () override = default;

        void run () override;

        std::pair<int, int> getTermSize () const override { return virtSize_; }
        void paint (const std::pair<int, int> &rabbit) override;
        void paint (const Control::Snake &snake) override;
        void write (const std::pair<std::string, size_t> &line) override;
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