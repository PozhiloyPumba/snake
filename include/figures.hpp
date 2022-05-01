#ifndef FIGURES_HPP
#define FIGURES_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iterator>
#include <vector>

namespace graphicInterface {
    class RoundedRectangle final : public sf::ConvexShape {
    private:
        std::vector<sf::Vector2f> vertices_;
        sf::Vector2f position{0.f, 0.f};
        sf::FloatRect rect_;
        float radius_;

        void init ();

    public:
        RoundedRectangle (const sf::FloatRect &rect, const float radius);

        ~RoundedRectangle () override = default;

        inline size_t getPointCount () const override { return vertices_.size (); }
        inline sf::Vector2f getPoint (size_t i) const override { return vertices_[i]; }
    };
}  // namespace graphicInterface

#endif