#include "gview.hpp"

namespace graphicInterface {
#define X_SCREEN_SIZE 800.
#define Y_SCREEN_SIZE 600.

    GView::GView ()
    {
        if (!font_.loadFromFile ("../sprites/ComicSansMS.ttf"))
            throw std::invalid_argument ("../sprites/ComicSansMS.ttf doesn't open");
        
        loadTexture (freshMeat_, "../sprites/freshMeat.png");
        virtSize_ = {X_SCREEN_SIZE / ceilSize_, Y_SCREEN_SIZE / ceilSize_};

        for (int i = 0; i < 2; ++i) {
            sf::Texture pacman;
            loadTexture (pacman, "../sprites/pacman" + std::to_string (i) + ".png");
            head_.push_back (pacman);
        }

        for (int i = 0; i < 4; ++i) {
            sf::Texture tail;
            loadTexture (tail, "../sprites/tail" + std::to_string (i) + ".png");
            tail_.push_back (tail);
        }
    }

    void GView::loadTexture (sf::Texture &dest, const std::string &fileName)
    {
        if (!dest.loadFromFile (fileName))
            throw std::invalid_argument (fileName + std::string ("doesn't open"));

        dest.setSmooth (true);
    }

    void GView::setTextureInSprite (const sf::Texture &texture)
    {
        spr_.setTexture (texture);
        sf::Vector2f targetSize(ceilSize_, ceilSize_);

        spr_.setScale(
            targetSize.x / spr_.getLocalBounds().width,
            targetSize.y / spr_.getLocalBounds().height);
         
        spr_.setOrigin(
            spr_.getLocalBounds().width/2.0f, 
            spr_.getLocalBounds().height/2.0f);
        
        spr_.setRotation (0);
    }

    sf::Keyboard::Key GView::fromStringtoKey (const std::string &button)  // I hate this function
    {
        switch (std::toupper (button[0])) {
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
                if (button.length () == 1)
                    return sf::Keyboard::Escape;
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

    void GView::startScreen ()
    {
        using namespace std::chrono_literals;
        auto globalStart = std::chrono::steady_clock::now ();
        
        while (window_.isOpen () && std::chrono::steady_clock::now () < globalStart + 3000ms) {
            sf::Event event;
            while (window_.pollEvent (event))
                closeAndResizeHelper (event);

            auto timer = [&] (auto time) {
                sf::Text text (time, font_, 150);
                text.setOrigin (
                    text.getLocalBounds().width/2.0f, 
                    text.getLocalBounds().height/2.0f
                );

                text.setFillColor (sf::Color::Green);
                text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y);
                window_.clear ();
                drawFrame ();
                window_.draw (text);
                window_.display ();
            };

            if (std::chrono::steady_clock::now () < globalStart + 1000ms) {
                timer ("3");
                continue;
            }
            if (std::chrono::steady_clock::now () < globalStart + 2000ms) {
                timer ("2");
                continue;
            }
            timer ("1");
        }
    }

    void GView::endScreen ()
    {
        const sf::Vector2u size = window_.getSize ();
        sf::RectangleShape frameIn ({float (size.x - 2 * ceilSize_), float (size.y - 2 * ceilSize_)});
        frameIn.setFillColor (sf::Color (0, 0, 0, 192));

        frameIn.move (ceilSize_, ceilSize_);

        window_.draw (frameIn);

        sf::Music music;
        if (!music.openFromFile("../sprites/VIKA.wav"))
            throw std::invalid_argument ("music doesn't open");
        
        music.setLoop (true);
        music.play ();

        sf::Text text ("POINTS:", font_, 40);
        text.setOrigin (
            text.getLocalBounds().width/2.0f, 
            text.getLocalBounds().height/2.0f
        );

        text.setFillColor (sf::Color::Green);
        text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y - 210);
        window_.draw (text);
        
        writeScoreTable ();

        window_.display ();

        while (window_.isOpen ()) {
            sf::Event event;
            while (window_.pollEvent (event))
                closeAndResizeHelper (event);
        }
    }

    void GView::closeAndResizeHelper (const sf::Event &event)
    {
        if (event.type == sf::Event::Closed)
            window_.close ();
        if (event.type == sf::Event::Resized) {
            virtSize_ = {event.size.width / ceilSize_, event.size.height / ceilSize_};
            float w = static_cast<float> (event.size.width);
            float h = static_cast<float> (event.size.height);
            window_.setView (
                sf::View (
                    sf::Vector2f (w / 2.0, h / 2.0),
                    sf::Vector2f (w, h)));
        }
    }

    void GView::drawFrame ()  // TODO: pacman edges
    {
        sf::Vector2u size = window_.getSize ();
        size = {size.x - size.x % ceilSize_, size.y - size.y % ceilSize_};
        sf::RectangleShape frame ({float (size.x), float (size.y)});
        frame.setFillColor (sf::Color::Cyan);

        sf::RectangleShape frameIn ({float (size.x - 2 * ceilSize_), float (size.y - 2 * ceilSize_)});
        frameIn.setFillColor (sf::Color::Black);

        frameIn.move (ceilSize_, ceilSize_);

        window_.draw (frame);
        window_.draw (frameIn);
    }

    void GView::paint (const std::pair<unsigned short, unsigned short> &rabbit) //TODO: it needs in some moving
    {
        setTextureInSprite (freshMeat_);
        spr_.setPosition ((rabbit.first + 0.5) * ceilSize_, (rabbit.second + 0.5) * ceilSize_  + 1);

        window_.draw (spr_);
    }

    void GView::paint (const Control::Snake &snake)
    {
        setTextureInSprite (head_[chapterOfCycle_]);

        switch (snake.direction_) {
            case Control::Snake::dir::UP:   spr_.setRotation (90);  break;
            case Control::Snake::dir::DOWN: spr_.setRotation (-90); break;
            case Control::Snake::dir::LEFT: spr_.setRotation (0);   break;
            case Control::Snake::dir::RIGHT:spr_.setRotation (180); break;
        }
        auto headPosition = *(snake.body_.begin ());
        spr_.setPosition ((headPosition.first + 0.5) * ceilSize_, (headPosition.second + 0.5) * ceilSize_);

        window_.draw (spr_);
        
        int i = 0;
        for (auto curIt = ++snake.body_.begin (), endIt = snake.body_.end (); curIt != endIt; ++curIt) {
            
            setTextureInSprite (tail_[++i % 4]);
            spr_.setPosition ((curIt->first + 0.5) * ceilSize_, (curIt->second + 0.5) * ceilSize_);

            window_.draw (spr_);
        }
    }

    void GView::write (const std::pair <std::string, size_t> &line)
    {
        sf::Text text ("", font_);
        text.setFillColor (sf::Color::Magenta);

        text.setString (std::to_string (++alreadyWriten_) + ". " + line.first + "\t" + std::to_string (line.second));
        text.setOrigin (
            text.getLocalBounds().width/2.0f, 
            text.getLocalBounds().height/2.0f
        );

        text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y - 200 + alreadyWriten_ * 30);
        window_.draw (text);
    }

    void GView::run ()  //TODO: refactor
    {
        window_.create (sf::VideoMode (X_SCREEN_SIZE, Y_SCREEN_SIZE), "Snake memes");

        const sf::Vector2u size = window_.getSize ();
        virtSize_ = {size.x / ceilSize_, size.y / ceilSize_};
        window_.setVerticalSyncEnabled (true);
        
        using namespace std::chrono_literals;

        startScreen ();

        while (window_.isOpen ()) {
            sf::Event event;

            auto start = std::chrono::steady_clock::now ();

            while (std::chrono::steady_clock::now () < start + 200ms) {
                while (window_.pollEvent (event)) {
                    closeAndResizeHelper (event);

                    if (event.type == sf::Event::KeyPressed) {
                        auto res = buttonTable_.find (event.key.code);
                        if (res != buttonTable_.end ())
                            res->second ();
                    }
                }
                if (chapterOfCycle_ && std::chrono::steady_clock::now () > start + 100ms) {
                    chapterOfCycle_ = 0;
                    window_.clear ();
                    drawing ();

                    window_.display ();
                }
            }

            chapterOfCycle_ = 1;
            botsHandler ();
            auto result = setCoordObjs ();

            if (result) {
                end_ = true;
                break;
            }

            window_.clear ();
            drawing ();
            window_.display ();
        }

        if (end_)
            endScreen ();
    }
}  // namespace graphicInterface