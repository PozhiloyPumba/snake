#include "gview.hpp"

namespace graphicInterface {
    GView::~GView ()
    {
    }

    void GView::drawFrame ()
    {
    }

    void GView::run ()
    {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Snake memes");
        sf::Texture texture;
        if (!texture.loadFromFile("../sprites/jaba.png"))
            throw std::invalid_argument ("ahahha");    //TODO:
        
        sf::Music music;
        if (!music.openFromFile("../sprites/spinner.wav"))
            throw std::invalid_argument ("ahahha");    //TODO:
        // Play the music
        music.play();
        
        sf::Sprite sprite(texture);
        sf::Vector2f targetSize(200.0f, 200.0f); //целевой размер
 
        sprite.setScale(targetSize.x / sprite.getLocalBounds().width, targetSize.y / sprite.getLocalBounds().height);

        sprite.setPosition (300, 300);

        window.setVerticalSyncEnabled(true);
        
        sprite.setOrigin (500, 500);

        while (window.isOpen ()) {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                
            }

            window.clear();
            sprite.rotate (5);
            window.draw(sprite);

            window.display();

        }
    }
}  // namespace graphicInterface