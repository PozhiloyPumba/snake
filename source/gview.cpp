#include "gview.hpp"

namespace graphicInterface {
    #define X_SCREEN_SIZE 800.
    #define Y_SCREEN_SIZE 600.
    
    GView::~GView ()
    {
    }

    sf::Keyboard::Key GView::fromStringtoKey (const std::string &button)    // I hate this function
    {
        switch (std::toupper(button[0])) {
            case 'A': return sf::Keyboard::A;
            case 'B': return sf::Keyboard::B;
            case 'C': return sf::Keyboard::C;
            case 'D': return sf::Keyboard::D;
            case 'E': return sf::Keyboard::E;
            case 'F': return sf::Keyboard::F;
            case 'G': return sf::Keyboard::G;
            case 'H': return sf::Keyboard::H;
            case 'I': return sf::Keyboard::I;
            case 'J': return sf::Keyboard::J;
            case 'K': return sf::Keyboard::K;
            case 'L': return sf::Keyboard::L;
            case 'M': return sf::Keyboard::M;
            case 'N': return sf::Keyboard::N;
            case 'O': return sf::Keyboard::O;
            case 'P': return sf::Keyboard::P;
            case 'Q': return sf::Keyboard::Q;
            case 'R': return sf::Keyboard::R;
            case 'S': return sf::Keyboard::S;
            case 'T': return sf::Keyboard::T;
            case 'U': return sf::Keyboard::U;
            case 'V': return sf::Keyboard::V;
            case 'W': return sf::Keyboard::W;
            case 'X': return sf::Keyboard::X;
            case 'Y': return sf::Keyboard::Y;
            case 'Z': return sf::Keyboard::Z;
            case '\e': {
                if (button.length () == 1) return sf::Keyboard::Escape;
                if (button.length () == 3 && button[1] == '[') {
                    switch (button[2]) {
                        case 'A': return sf::Keyboard::Up;
                        case 'B': return sf::Keyboard::Down;
                        case 'C': return sf::Keyboard::Right;
                        case 'D': return sf::Keyboard::Left;
                    }
                }
                throw std::invalid_argument ("unknown button");
            }
            case '[': return sf::Keyboard::LBracket;
            case ']': return sf::Keyboard::RBracket;
            case ';': return sf::Keyboard::Semicolon;
            case ',': return sf::Keyboard::Comma;
            case '.': return sf::Keyboard::Period;
            case '\'': return sf::Keyboard::Quote;
            case '/': return sf::Keyboard::Slash;
            case '\\': return sf::Keyboard::Backslash;
            case '~': return sf::Keyboard::Tilde;
            case '=': return sf::Keyboard::Equal;
            case '-': return sf::Keyboard::Hyphen;
            case ' ': return sf::Keyboard::Space;
            case '\t': return sf::Keyboard::Tab;
            case '+': return sf::Keyboard::Add;
            default: throw std::invalid_argument ("unknown button");
        }
    }

    void GView::drawFrame ()
    {
        sf::Vector2u size = window_.getSize ();
        size = {size.x - size.x % ceilSize_, size.y - size.y % ceilSize_};
        sf::RectangleShape frame ({float(size.x), float(size.y)});
        frame.setFillColor (sf::Color::Cyan);

        sf::RectangleShape frameIn ({float(size.x  - 2 * ceilSize_), float(size.y - 2 * ceilSize_)});
        frameIn.setFillColor (sf::Color::Black);

        frameIn.move (ceilSize_, ceilSize_);
        std::cout << frame.getSize().x << " " << frame.getSize().y << std::endl;

        window_.draw (frame);
        window_.draw (frameIn);
    }

    void GView::run ()
    {
        window_.create(sf::VideoMode(X_SCREEN_SIZE, Y_SCREEN_SIZE), "Snake memes");

        const sf::Vector2u size = window_.getSize ();
        virtSize_ = {size.x / ceilSize_, size.y / ceilSize_};
        window_.setVerticalSyncEnabled(true);

        sf::Texture texture;
        if (!texture.loadFromFile("../sprites/jaba.png"))
            throw std::invalid_argument ("jaba doesn't open");    //TODO:
        
        texture.setSmooth(true);

        sf::Music music;
        if (!music.openFromFile("../sprites/spinner.wav"))
            throw std::invalid_argument ("music doesn't open");
        // Play the music
        music.setLoop (true);
        music.play ();

        while (window_.isOpen ()) {
            sf::Event event;
            while (window_.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window_.close();

                if (event.type == sf::Event::Resized) {
                    virtSize_ = {event.size.width / ceilSize_, event.size.height / ceilSize_};
                    float w = static_cast<float>(event.size.width);
                    float h = static_cast<float>(event.size.height);          
                    window_.setView(
                        sf::View(
                            sf::Vector2f(w / 2.0, h / 2.0), 
                            sf::Vector2f(w, h)
                        )
                    );
                }

                if (event.type == sf::Event::KeyPressed) {
                    auto res = buttonTable_.find (event.key.code);
                    if (res != buttonTable_.end ())
                        res->second ();
                }
            }

            window_.clear();
            drawFrame ();

            window_.display();

        }
    }
}  // namespace graphicInterface