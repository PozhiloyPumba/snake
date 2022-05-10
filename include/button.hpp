#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <utility>

namespace graphicInterface {

    bool checkPointRectCollision (const sf::Vector2i &p, const sf::RectangleShape &rect)
    {
        sf::Vector2f left_top_point = rect.getPosition () - rect.getOrigin ();
        float left_x = left_top_point.x;
        float right_x = left_top_point.x + rect.getSize ().x;
        float left_y = left_top_point.y;
        float right_y = left_top_point.y + rect.getSize ().y;
        if (p.x > left_x && p.x < right_x && p.y > left_y && p.y < right_y)
            return true;
        return false;
    }

    template <typename functor>
    class Button {
    private:
        sf::Color color_;
        sf::RectangleShape form_;
        functor pred_;
        sf::Font font_;
        sf::Text text_;

    public:
        Button (const sf::Vector2f &ldp, const sf::Vector2f &rup, const sf::Color &color, const std::string &text, const functor &pred) : 
            color_ (color),
            pred_ (pred)
        {
            if (!font_.loadFromFile ("../sprites/ComicSansMS.ttf"))
                throw std::invalid_argument ("../sprites/ComicSansMS.ttf doesn't open");

            text_.setFont (font_);
            text_.setFillColor (sf::Color::Green);
            text_.setString (text);
            text_.setOrigin (
                text_.getLocalBounds ().width / 2.0f,
                text_.getLocalBounds ().height / 2.0f);
            text_.setPosition ({(rup.x + ldp.x) / 2, (rup.y + ldp.y) / 2});

            sf::RectangleShape rectangle ({rup.x - ldp.x, rup.y - ldp.y});
            rectangle.setFillColor (color);
            rectangle.setOrigin (
                rectangle.getLocalBounds ().width / 2.0f,
                rectangle.getLocalBounds ().height / 2.0f);
            
            rectangle.setPosition ({(rup.x + ldp.x) / 2, (rup.y + ldp.y) / 2});
            form_ = rectangle;
        }

        void setString (const std::string &str)
        {
            text_.setString (str);
        }

        void draw (sf::RenderWindow &window, const std::pair <float, float> &coef)
        {
            auto tmpForm = form_;
            tmpForm.setPosition (form_.getPosition ().x * coef.first, form_.getPosition ().y * coef.second);
            auto tmpText = text_;
            tmpText.setPosition (text_.getPosition ().x * coef.first, text_.getPosition ().y * coef.second);
            
            if (checkPointRectCollision(sf::Mouse::getPosition(window), tmpForm)) {
                form_.setFillColor(sf::Color::Red);
            }
            else {
                form_.setFillColor(color_);
            }
            window.draw (tmpForm);
            window.draw (tmpText);
        }

        bool pressed (sf::Event &event, sf::RenderWindow &window, const std::pair <float, float> &coef)
        {
            auto tmpForm = form_;
            tmpForm.setPosition (form_.getPosition ().x * coef.first, form_.getPosition ().y * coef.second);

            if (event.type == sf::Event::MouseButtonPressed) {
                int flag = true; 
                while (checkPointRectCollision(sf::Mouse::getPosition(window), tmpForm) && flag && window.isOpen ()) {
                    sf::Event event;
                    if (window.pollEvent (event)) {
                        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q))
                            window.close ();
                        if (event.type == sf::Event::MouseButtonReleased) {
                            flag = false;
                            if (checkPointRectCollision(sf::Mouse::getPosition(window), tmpForm)) {
                                pred_ ();
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }
    };

}  // namespace graphicInterface

#endif