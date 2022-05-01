#include "figures.hpp"

namespace graphicInterface {
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

        std::generate_n (
            std::back_insert_iterator (quarterCircle),
            pointsNumberInRoundPart,
            [this, precision, n = 0.] () mutable -> sf::Vector2f { n += precision; return {cosf(n) * radius_, sinf(n) * radius_}; });

        std::for_each (quarterCircle.rbegin (), quarterCircle.rend (), 
                       [this] (auto point) { vertices_.push_back ({rect_.width - radius_ + point.x, radius_ - point.y}); });
        std::for_each (quarterCircle.begin (), quarterCircle.end (), 
                       [this] (auto point) { vertices_.push_back ({rect_.width - radius_ + point.x, rect_.height - radius_ + point.y}); });
        std::for_each (quarterCircle.rbegin (), quarterCircle.rend (), 
                       [this] (auto point) { vertices_.push_back ({radius_ - point.x, rect_.height - radius_ + point.y}); });
        std::for_each (quarterCircle.begin (), quarterCircle.end (), 
                       [this] (auto point) { vertices_.push_back ({radius_ - point.x, radius_ - point.y}); });
    }
}