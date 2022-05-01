#ifndef FIGURES_HPP
#define FIGURES_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <vector>
#include <iterator>

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

        inline size_t       getPointCount(        ) const override { return vertices_.size();}
        inline sf::Vector2f getPoint     (size_t i) const override { return vertices_[i];    }
    };

    RoundedRectangle::RoundedRectangle (const sf::FloatRect &rect, const float radius) : rect_ (rect), radius_ (radius)
    {
        init ();
        setPosition (rect.left, rect.top);
        sf::Shape::update ();
    }

    void RoundedRectangle::init ()
    {
        const int pointsNumberInRoundPart = 20;
        const float precision = M_PI_2 / pointsNumberInRoundPart;

        std::vector<sf::Vector2f> quarterCircle;
        
        float n = 0;
        std::generate_n (
            std::back_insert_iterator (quarterCircle),
            pointsNumberInRoundPart,
            [this, precision, &n] () -> sf::Vector2f { n += precision; return {cosf(n) * radius_, sinf(n) * radius_}; });

        std::for_each (quarterCircle.rbegin (), quarterCircle.rend (), 
                       [this] (auto point) { vertices_.push_back ({rect_.width - radius_ + point.x, radius_ - point.y}); });
        std::for_each (quarterCircle.begin (), quarterCircle.end (), 
                       [this] (auto point) { vertices_.push_back ({rect_.width - radius_ + point.x, rect_.height - radius_ + point.y}); });
        std::for_each (quarterCircle.rbegin (), quarterCircle.rend (), 
                       [this] (auto point) { vertices_.push_back ({radius_ - point.x, rect_.height - radius_ + point.y}); });
        std::for_each (quarterCircle.begin (), quarterCircle.end (), 
                       [this] (auto point) { vertices_.push_back ({radius_ - point.x, radius_ - point.y}); });
    }

}  // namespace graphicInterface

#endif