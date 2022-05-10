#include "gview.hpp"

#include <functional>

#include "button.hpp"
#include "figures.hpp"

namespace graphicInterface {
#define X_SCREEN_SIZE 800.
#define Y_SCREEN_SIZE 600.

    GView::GView ()
    {
        if (!font_.loadFromFile ("../sprites/ComicSansMS.ttf"))
            throw std::invalid_argument ("../sprites/ComicSansMS.ttf doesn't open");

        loadTexture (freshMeat_, "../sprites/freshMeat.png");

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

        window_.create (sf::VideoMode (X_SCREEN_SIZE, Y_SCREEN_SIZE), "Snake memes");
        const sf::Vector2u size = window_.getSize ();
        virtSize_ = {size.x / ceilSize_, size.y / ceilSize_};
        window_.setVerticalSyncEnabled (true);
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

        sf::Vector2f targetSize (ceilSize_, ceilSize_);
        spr_.setScale (
            targetSize.x / spr_.getLocalBounds ().width,
            targetSize.y / spr_.getLocalBounds ().height);

        spr_.setOrigin (
            spr_.getLocalBounds ().width / 2.0f,
            spr_.getLocalBounds ().height / 2.0f);

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
            // case 'Q': return sf::Keyboard::Q;    q for quit
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
            default: throw std::invalid_argument ("unknown button");
        }
    }

    void GView::startScreen ()
    {
        using namespace std::chrono_literals;
        auto globalStart = std::chrono::steady_clock::now ();

        auto timer = [&] (auto time) {
            sf::Text text (time, font_, 100);
            text.setOrigin (
                text.getLocalBounds ().width / 2.0f,
                text.getLocalBounds ().height / 2.0f);

            text.setFillColor (sf::Color::Green);
            text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y);
            window_.clear ();
            drawFrame ();
            window_.draw (text);
            window_.display ();
        };

        while (window_.isOpen () && std::chrono::steady_clock::now () < globalStart + 3000ms) {
            sf::Event event;
            while (window_.pollEvent (event))
                closeAndResizeHelper (event);

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
        sf::Music music;
        if (!music.openFromFile ("../sprites/VIKA.wav"))
            throw std::invalid_argument ("music doesn't open");

        music.setLoop (true);
        music.play ();

        bool resized = true;

        while (window_.isOpen ()) {
            sf::Event event;
            while (window_.pollEvent (event)) {
                closeAndResizeHelper (event);
                if (event.type == sf::Event::Resized)
                    resized = true;
            }

            if (resized) {
                window_.clear ();
                drawFrame ();
                sf::Vector2u size = window_.getSize ();
                size = {size.x - size.x % ceilSize_, size.y - size.y % ceilSize_};
                sf::RectangleShape frameIn ({float (size.x - 2 * ceilSize_), float (size.y - 2 * ceilSize_)});
                frameIn.setFillColor (sf::Color (0, 0, 0, 192));

                frameIn.move (ceilSize_, ceilSize_);

                window_.draw (frameIn);

                sf::Text text ("POINTS:", font_, 40);
                text.setOrigin (
                    text.getLocalBounds ().width / 2.0f,
                    text.getLocalBounds ().height / 2.0f);

                text.setFillColor (sf::Color::Green);
                text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y - window_.getView ().getCenter ().y / 2);
                window_.draw (text);

                writeScoreTable ();

                window_.display ();
                alreadyWriten_ = 0;
                resized = false;
            }
        }
    }

    void GView::closeAndResizeHelper (const sf::Event &event)
    {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q))
            window_.close ();
        if (event.type == sf::Event::Resized) {
            virtSize_ = {event.size.width / ceilSize_, event.size.height / ceilSize_};
            float w = static_cast<float> (event.size.width);
            float h = static_cast<float> (event.size.height);
            scale_ = {w / X_SCREEN_SIZE, h / Y_SCREEN_SIZE};

            window_.setView (
                sf::View (
                    sf::Vector2f (w / 2.0, h / 2.0),
                    sf::Vector2f (w, h)));
            resizeHandler ();
        }
    }

    void GView::drawFrame ()
    {
        sf::Vector2u size = window_.getSize ();
        size = {size.x - size.x % ceilSize_, size.y - size.y % ceilSize_};
        sf::FloatRect frame (5, 5, size.x - 10, size.y - 10);

        RoundedRectangle roundRect{frame, static_cast<float> (ceilSize_ / 2)};

        roundRect.setFillColor (sf::Color (0, 0, 0, 255));
        roundRect.setOutlineColor (sf::Color::Blue);
        roundRect.setOutlineThickness (2);

        window_.draw (roundRect);
        sf::FloatRect frameIn (ceilSize_ - 5, ceilSize_ - 5, size.x - 2 * (ceilSize_ - 5), size.y - 2 * (ceilSize_ - 5));

        RoundedRectangle roundRectIn{frameIn, static_cast<float> (5)};

        roundRectIn.setFillColor (sf::Color (0, 0, 0, 255));
        roundRectIn.setOutlineColor (sf::Color::Blue);
        roundRectIn.setOutlineThickness (2);

        window_.draw (roundRectIn);
    }

    void GView::paint (const std::pair<int, int> &rabbit)
    {
        setTextureInSprite (freshMeat_);
        spr_.setPosition ((rabbit.first + 0.5) * ceilSize_, (rabbit.second + 0.5) * ceilSize_ + 1);

        window_.draw (spr_);
    }

    void GView::paint (const Control::Snake &snake)
    {
        setTextureInSprite (head_[chapterOfCycle_]);

        switch (snake.direction_) {
            case Control::Snake::dir::UP: spr_.setRotation (90); break;
            case Control::Snake::dir::DOWN: spr_.setRotation (-90); break;
            case Control::Snake::dir::LEFT: spr_.setRotation (0); break;
            case Control::Snake::dir::RIGHT: spr_.setRotation (180); break;
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

    void GView::write (const std::pair<std::string, size_t> &line)
    {
        sf::Text text ("", font_);
        text.setFillColor (sf::Color::Magenta);

        text.setString (std::to_string (++alreadyWriten_) + ". " + line.first + "\t" + std::to_string (line.second));
        text.setOrigin (
            text.getLocalBounds ().width / 2.0f,
            text.getLocalBounds ().height / 2.0f);

        text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 2 + alreadyWriten_ * 40);
        window_.draw (text);
    }

    void GView::chooserKey (std::vector<std::string>::iterator &key)
    {
        bool endChooser = false;

        while (!endChooser && window_.isOpen ()) {
            sf::Event event;
            if (window_.pollEvent (event)) {
                closeAndResizeHelper (event);

                if (event.type == sf::Event::KeyPressed) {
                    const sf::Keyboard::Key keycode = event.key.code;
                    if (keycode >= sf::Keyboard::A && keycode <= sf::Keyboard::Z) {
                        *key = (keycode - sf::Keyboard::A + 'a');
                        endChooser = true;
                    }

                    if (keycode >= sf::Keyboard::Right && keycode >= sf::Keyboard::Down) {
                        switch (keycode) {
                            case sf::Keyboard::Up: *key = "\e[A"; break;
                            case sf::Keyboard::Down: *key = "\e[B"; break;
                            case sf::Keyboard::Right: *key = "\e[C"; break;
                            case sf::Keyboard::Left: *key = "\e[D"; break;
                            default: throw std::invalid_argument ("unknown button");
                        }
                        endChooser = true;
                    }
                }
            }
        }
    }

    void GView::createCustomGamer (const std::string &name)
    {
        sf::Text title ("Select keys (press Enter after selection):", font_);
        title.setFillColor (sf::Color::Magenta);
        title.setOrigin (
            title.getLocalBounds ().width / 2.0f,
            title.getLocalBounds ().height / 2.0f);

        title.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 10);

        std::vector<std::string> buttons{4, ""};

        Button up = {{350, 200}, {450, 300}, sf::Color::Blue, "", std::bind (&GView::chooserKey, this, buttons.begin ())};
        Button left = {{225, 325}, {325, 425}, sf::Color::Blue, "", std::bind (&GView::chooserKey, this, buttons.begin () + 1)};
        Button down = {{350, 325}, {450, 425}, sf::Color::Blue, "", std::bind (&GView::chooserKey, this, buttons.begin () + 2)};
        Button right = {{475, 325}, {575, 425}, sf::Color::Blue, "", std::bind (&GView::chooserKey, this, buttons.begin () + 3)};

        bool endCustom = false;

        while (!endCustom && window_.isOpen ()) {
            sf::Event event;
            if (window_.pollEvent (event)) {
                closeAndResizeHelper (event);

                up.pressed (event, window_, scale_);
                left.pressed (event, window_, scale_);
                down.pressed (event, window_, scale_);
                right.pressed (event, window_, scale_);

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                    endCustom = true;
            }

            up.setString (buttons[0]);
            left.setString (buttons[1]);
            down.setString (buttons[2]);
            right.setString (buttons[3]);

            window_.clear ();
            title.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 10);
            window_.draw (title);

            up.draw (window_, scale_);
            left.draw (window_, scale_);
            down.draw (window_, scale_);
            right.draw (window_, scale_);

            window_.display ();
        }
        addPlayerHandler (buttons, name);
    }

    void GView::inputName (sf::Text &title, std::string &name)
    {
        sf::Text text ("", font_);
        text.setFillColor (sf::Color::Red);

        bool endName = false;

        while (!endName && window_.isOpen ()) {
            sf::Event event;
            if (window_.pollEvent (event)) {
                if (event.type == sf::Event::Closed)
                    window_.close ();
                if (event.type == sf::Event::Resized) {
                    virtSize_ = {event.size.width / ceilSize_, event.size.height / ceilSize_};
                    float w = static_cast<float> (event.size.width);
                    float h = static_cast<float> (event.size.height);
                    scale_ = {w / X_SCREEN_SIZE, h / Y_SCREEN_SIZE};

                    window_.setView (
                        sf::View (
                            sf::Vector2f (w / 2.0, h / 2.0),
                            sf::Vector2f (w, h)));
                }

                if (event.type == sf::Event::KeyPressed) {
                    const sf::Keyboard::Key keycode = event.key.code;
                    if (keycode >= sf::Keyboard::A && keycode <= sf::Keyboard::Z && name.length () < 50) {
                        char c = static_cast<char> (keycode - sf::Keyboard::A + 'a');
                        name.push_back (c);
                    }
                    if (keycode == sf::Keyboard::Enter)
                        endName = true;

                    if (keycode == sf::Keyboard::BackSpace && name.length ()) {
                        name.pop_back ();
                    }
                }
            }
            text.setString (name);
            text.setOrigin (
                text.getLocalBounds ().width / 2.0f,
                text.getLocalBounds ().height / 2.0f);

            text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 2);

            window_.clear ();

            title.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 2 - 40);
            window_.draw (title);
            window_.draw (text);

            window_.display ();
        }
    }

    void GView::chooseController (sf::Text &title, std::string &name)
    {
        bool endControls = false;

        title.setFillColor (sf::Color::Magenta);
        title.setString ("Choose control type:");
        title.setOrigin (
            title.getLocalBounds ().width / 2.0f,
            title.getLocalBounds ().height / 2.0f);

        title.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 10);

        std::vector<std::string> buttonArrows = {"\e[A", "\e[D", "\e[B", "\e[C"};
        std::vector<std::string> buttonWASD = {"w", "a", "s", "d"};

        Button arrows ({100, 100}, {700, 200}, sf::Color::Blue, "arrows", std::bind (View::addPlayerHandler, buttonArrows, name));
        Button wasd ({100, 250}, {700, 350}, sf::Color::Blue, "WASD", std::bind (View::addPlayerHandler, buttonWASD, name));
        Button custom ({100, 400}, {700, 500}, sf::Color::Blue, "custom", std::bind (&GView::createCustomGamer, this, name));

        while (!endControls && window_.isOpen ()) {
            sf::Event event;
            if (window_.pollEvent (event)) {
                if (event.type == sf::Event::Closed)
                    window_.close ();
                if (event.type == sf::Event::Resized) {
                    virtSize_ = {event.size.width / ceilSize_, event.size.height / ceilSize_};
                    float w = static_cast<float> (event.size.width);
                    float h = static_cast<float> (event.size.height);
                    scale_ = {w / X_SCREEN_SIZE, h / Y_SCREEN_SIZE};

                    window_.setView (
                        sf::View (
                            sf::Vector2f (w / 2.0, h / 2.0),
                            sf::Vector2f (w, h)));
                }
                if (arrows.pressed (event, window_, scale_) ||
                    wasd.pressed (event, window_, scale_)   ||
                    custom.pressed (event, window_, scale_)) {
                        
                    smallViewAfterAddingPlayer (name, Control::Snake::controlType::HUMAN);
                    endControls = true;
                }
            }
            window_.clear ();
            title.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 10);
            window_.draw (title);

            arrows.draw (window_, scale_);
            wasd.draw (window_, scale_);
            custom.draw (window_, scale_);

            window_.display ();
        }
    }

    void GView::createGamer ()
    {
        sf::Text title ("Enter name:", font_);
        title.setFillColor (sf::Color::Magenta);
        title.setOrigin (
            title.getLocalBounds ().width / 2.0f,
            title.getLocalBounds ().height / 2.0f);

        title.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 2 - 40);

        std::string name;

        inputName (title, name);

        chooseController (title, name);
    }

    void GView::menu ()
    {
        sf::Text menuLegend ("MENU (press Enter to start):", font_, 50);
        menuLegend.setOrigin (
            menuLegend.getLocalBounds ().width / 2.0f,
            menuLegend.getLocalBounds ().height / 2.0f);

        menuLegend.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 10);

        Button addGamer ({100, 100}, {700, 200}, sf::Color::Blue, "add new player", std::bind (&GView::createGamer, this));
        Button addSmartBot ({100, 250}, {700, 350}, sf::Color::Blue, "add smart bot", std::bind (addBotHandler, 1));
        Button addStupidBot ({100, 400}, {700, 500}, sf::Color::Blue, "add stupid bot", std::bind (addBotHandler, 0));

        bool endMenu = false;
        while (!endMenu && window_.isOpen ()) {
            sf::Event event;
            if (window_.pollEvent (event)) {
                closeAndResizeHelper (event);

                if (addGamer.pressed (event, window_, scale_))
                    continue;
                if (addSmartBot.pressed (event, window_, scale_)) {
                    smallViewAfterAddingPlayer ("smart", Control::Snake::controlType::BOT);
                    continue;
                }
                if (addStupidBot.pressed (event, window_, scale_)) {
                    smallViewAfterAddingPlayer ("stupid", Control::Snake::controlType::BOT);
                    continue;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                    endMenu = true;
            }

            window_.clear ();
            menuLegend.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y / 10);
            window_.draw (menuLegend);
            addGamer.draw (window_, scale_);
            addSmartBot.draw (window_, scale_);
            addStupidBot.draw (window_, scale_);

            window_.display ();
        }
    }

    void GView::smallViewAfterAddingPlayer (const std::string &name, Control::Snake::controlType type)
    {
        using namespace std::chrono_literals;
        auto globalStart = std::chrono::steady_clock::now ();

        auto printData = [&] (auto data) {
            sf::Text text (data, font_, 40);
            text.setOrigin (
                text.getLocalBounds ().width / 2.0f,
                text.getLocalBounds ().height / 2.0f);

            text.setFillColor (sf::Color::Green);
            text.setPosition (window_.getView ().getCenter ().x, window_.getView ().getCenter ().y);
            window_.clear ();
            drawFrame ();
            window_.draw (text);
            window_.display ();
        };

        while (window_.isOpen () && std::chrono::steady_clock::now () < globalStart + 1500ms) {
            sf::Event event;
            while (window_.pollEvent (event))
                closeAndResizeHelper (event);
            
            if (type == Control::Snake::controlType::BOT) {
                printData (name + " bot was registered");
            }
            else {
                printData ("Player: \"" + name + "\" was registered");
            }
        }
    }

    void GView::run ()
    {
        menu ();
        resizeHandler ();

        startScreen ();

        while (window_.isOpen ()) {
            sf::Event event;

            auto start = std::chrono::steady_clock::now ();

            while (std::chrono::steady_clock::now () < start + tickTime_) {
                while (window_.pollEvent (event)) {
                    closeAndResizeHelper (event);

                    if (event.type == sf::Event::KeyPressed) {
                        auto res = buttonTable_.find (event.key.code);
                        if (res != buttonTable_.end ())
                            res->second ();
                    }
                }
                if (chapterOfCycle_ && std::chrono::steady_clock::now () > start + tickTime_ / 2) {
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