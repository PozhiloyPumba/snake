#include "tview.hpp"

namespace graphicInterface {

    void sigintHandler (int sigN)
    {
        if (sigN != SIGINT)
            return;

        TView::interruptHandler ();
    }

    void sigChangeTermSizeHandler (int sigN)
    {
        if (sigN != SIGWINCH)
            return;

        TView::changeTermSizeHandler ();
    }

    TView::TView ()
    {
        tcgetattr (0, &old_);
        struct termios raw = {};
        cfmakeraw (&raw);
        raw.c_lflag |= ISIG;
        raw.c_cc[VINTR] = 3;  // ctrl + C
        tcsetattr (0, TCSANOW, &raw);

        ioctl (STDOUT_FILENO, TIOCGWINSZ, &termSize_);
        virtSize_ = {termSize_.ws_col / 2, termSize_.ws_row};

        interruptHandler = std::bind (&TView::endHandler, this);
        signal (SIGINT, &graphicInterface::sigintHandler);
        changeTermSizeHandler = std::bind (&TView::resizer, this);
        signal (SIGWINCH, &graphicInterface::sigChangeTermSizeHandler);
    }

    void TView::endHandler ()
    {
        end_ = true;
    }

    void TView::resizer ()
    {
        ioctl (STDOUT_FILENO, TIOCGWINSZ, &termSize_);
        virtSize_ = {termSize_.ws_col / 2, termSize_.ws_row};

        if (!end_)
            resizeHandler ();
    }

    void TView::drawHLine (int xBeg, int yBeg, int length) const
    {
        xBeg = xBeg * 2 + 1;
        ++yBeg;

        printf ("\e[%d;%dH", yBeg, xBeg);
        for (int i = 0; i < length; ++i)
            printf ("%c%c", sym_.first, sym_.second);
    }

    void TView::drawVLine (int xBeg, int yBeg, int length) const
    {
        xBeg = xBeg * 2 + 1;
        ++yBeg;

        for (int i = 0; i < length; ++i) {
            printf ("\e[%d;%dH", yBeg++, xBeg);
            printf ("%c%c", sym_.first, sym_.second);
        }
    }

    void TView::drawBigDigit (std::integral_constant<int, 1>)
    {
        setColor (black_, green_);
        sym_ = {' ', ' '};
        drawVLine (virtSize_.first / 2, virtSize_.second / 2 - 2, 5);
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2 + 2, 3);
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2 - 1, 1);
        resetColor ();
        fflush (stdout);
    }

    void TView::drawBigDigit (std::integral_constant<int, 2>)
    {
        setColor (black_, green_);
        sym_ = {' ', ' '};
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2 - 2, 3);
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2 + 2, 3);
        drawHLine (virtSize_.first / 2, virtSize_.second / 2, 1);
        drawHLine (virtSize_.first / 2 + 1, virtSize_.second / 2 - 1, 1);
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2 + 1, 1);
        resetColor ();
        fflush (stdout);
    }

    void TView::drawBigDigit (std::integral_constant<int, 3>)
    {
        setColor (black_, green_);
        sym_ = {' ', ' '};
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2 - 2, 3);
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2 + 2, 3);
        drawHLine (virtSize_.first / 2 - 1, virtSize_.second / 2, 3);
        drawVLine (virtSize_.first / 2 + 1, virtSize_.second / 2 - 2, 5);
        resetColor ();
        fflush (stdout);
    }

    void TView::menuAddPlayer ()
    {
        drawFrame ();
        printf ("\e[%d;%dHEnter a name (or press enter to set default name)", termSize_.ws_row / 6, termSize_.ws_col / 2 - 25);
        printf ("\e[%d;%dH", termSize_.ws_row / 6 + 1, termSize_.ws_col / 2);
        fflush (stdout);

        std::string name;
        unsigned char c;
        sym_ = {' ', ' '};

        while ((c = getchar ()) != '\n' && c != '\r') {
            if (c == 127 && name.length ())  // 127 = delete
                name.pop_back ();
            else if (name.length () < 50)  // character length limit
                name += c;

            drawHLine (1, termSize_.ws_row / 6, virtSize_.first - 2);
            printf ("\e[%d;%luH%s", termSize_.ws_row / 6 + 1, termSize_.ws_col / 2 - name.length () / 2, name.data ());
        }

        drawFrame ();
        printf ("\e[%d;%dHEnter controls:", termSize_.ws_row / 6, termSize_.ws_col / 2 - 8);
        printf ("\e[%d;%dH1: arrows (press \"1\")", termSize_.ws_row / 6 + 1, termSize_.ws_col / 2 - 10);
        printf ("\e[%d;%dH2: wasd (press \"2\")", termSize_.ws_row / 6 + 2, termSize_.ws_col / 2 - 9);
        printf ("\e[%d;%dH3: custom (press \"3\")", termSize_.ws_row / 6 + 3, termSize_.ws_col / 2 - 10);
        fflush (stdout);

        read (0, &c, 1);

        switch (c) {
            case '1': addPlayerHandler ({"\e[A", "\e[D", "\e[B", "\e[C"}, name); break;
            case '2': addPlayerHandler ({"w", "a", "s", "d"}, name); break;
            case '3': {
                drawFrame ();
                printf ("\e[%d;%dHEnter buttons (up, left, down, right):", termSize_.ws_row / 6, termSize_.ws_col / 2 - 20);

                std::vector<std::string> buttons;

                for (int i = 1; i < 5; ++i) {
                    printf ("\e[%d;%dH", termSize_.ws_row / 6 + i, termSize_.ws_col / 2);
                    std::string button;
                    while ((c = getchar ()) != '\n' && c != '\r') {
                        if (c == 127 && button.length ())  // 127 = delete
                            button.pop_back ();
                        else
                            button += c;

                        drawHLine (1, termSize_.ws_row / 6 + i, virtSize_.first - 2);
                        printf ("\e[%d;%luH%s", termSize_.ws_row / 6 + i, termSize_.ws_col / 2 - button.length () / 2, button.data ());
                    }
                    buttons.push_back (button);
                }
                addPlayerHandler (buttons, name);
            }
        }
    }

    void TView::menuAddBot (int type)
    {
        drawFrame ();

        printf ("\e[%d;%dHEnter a number of stupid bots", termSize_.ws_row / 6, termSize_.ws_col / 2 - 14);
        printf ("\e[%d;%dH", termSize_.ws_row / 6 + 1, termSize_.ws_col / 2 - 1);
        fflush (stdout);

        std::string strnumber;
        unsigned char c;
        sym_ = {' ', ' '};

        while ((c = getchar ()) != '\n' && c != '\r') {
            if (c == 127 && strnumber.length ())  // 127 = delete
                strnumber.pop_back ();
            else
                strnumber += c;

            drawHLine (1, termSize_.ws_row / 6, virtSize_.first - 2);
            printf ("\e[%d;%luH%s", termSize_.ws_row / 6 + 1, termSize_.ws_col / 2 - strnumber.length () / 2, strnumber.data ());
        }

        int number = std::atoi (strnumber.data ());

        for (int i = 0; i < number; ++i)
            addBotHandler (type);
    }

    void TView::menu ()
    {
        struct pollfd in = {0, POLL_IN, 0};

        bool start = false;

        while (!end_ && !start) {
            drawFrame ();

            printf ("\e[%d;%dHMENU:", termSize_.ws_row / 6, termSize_.ws_col / 2 - 2);
            printf ("\e[%d;%dH1: ADD PLAYER (print \"1\")", termSize_.ws_row / 6 + 1, termSize_.ws_col / 2 - 12);
            printf ("\e[%d;%dH2: ADD STUPID BOT (print \"2\")", termSize_.ws_row / 6 + 2, termSize_.ws_col / 2 - 14);
            printf ("\e[%d;%dH3: ADD SMART BOT (print \"3\")", termSize_.ws_row / 6 + 3, termSize_.ws_col / 2 - 14);
            printf ("\e[%d;%dHpress \"q\" for quit", termSize_.ws_row / 6 + 4, termSize_.ws_col / 2 - 8);
            printf ("\e[%d;%dHpress \"s\" for start", termSize_.ws_row / 6 + 5, termSize_.ws_col / 2 - 8);
            printf ("\e[%d;%dH", termSize_.ws_row / 6 + 6, termSize_.ws_col / 2);
            fflush (stdout);

            if (poll (&in, 1, 200) == 1) {
                unsigned char c;
                read (0, &c, 1);

                switch (c) {
                    case 's': start = true; break;
                    case 'q': endHandler (); break;
                    case '1': menuAddPlayer (); break;
                    case '2': menuAddBot (0); break;
                    case '3': menuAddBot (1); break;
                }
            }
        }
    }

    void TView::startScreen ()
    {
        using namespace std::chrono_literals;
        auto globalStart = std::chrono::steady_clock::now ();
        struct pollfd in = {0, POLL_IN, 0};

        while (!end_ && std::chrono::steady_clock::now () < globalStart + 3000ms) {
            drawFrame ();
            if (std::chrono::steady_clock::now () < globalStart + 1000ms) {
                drawBigDigit (std::integral_constant<int, 3>{});
            }
            else if (std::chrono::steady_clock::now () < globalStart + 2000ms) {
                drawBigDigit (std::integral_constant<int, 2>{});
            }
            else {
                drawBigDigit (std::integral_constant<int, 1>{});
            }

            if (poll (&in, 1, 200) == 1) {
                unsigned char c;
                read (0, &c, 1);
                if (c == 'q') {
                    endHandler ();
                    break;
                }
            }
        }
    }

    void TView::endScreen ()
    {
        setColor (white_, black_);
        struct pollfd in = {0, POLL_IN, 0};
        while (!end_) {
            if (poll (&in, 1, 200) == 1) {
                unsigned char c;
                read (0, &c, 1);
                break;
            }
            drawFrame ();

            printf ("\e[%d;%dHPOINTS:", termSize_.ws_row / 6, termSize_.ws_col / 2 - 3);

            writeScoreTable ();
            printf ("\e[%d;%dH(press any key to exit)", termSize_.ws_row / 6 + alreadyWriten_ + 1, termSize_.ws_col / 2 - 12);
            printf ("\e[0;0H");
            fflush (stdout);
            alreadyWriten_ = 0;
        }
        resetColor ();
        printf ("\e[1;1H\e[J");
    }

    void TView::drawFrame ()
    {
        printf ("\e[1;1H\e[J");

        setColor (purple_, purple_);
        sym_ = {' ', ' '};
        drawHLine (0, 0, virtSize_.first);
        drawHLine (0, virtSize_.second - 1, virtSize_.first);
        drawVLine (0, 0, virtSize_.second);
        drawVLine (virtSize_.first - 1, 0, virtSize_.second);
        resetColor ();
        fflush (stdout);
    }

    void TView::paint (const std::pair<int, int> &rabbit)
    {
        setColor (black_, lightGrey_);
        sym_ = {' ', ' '};
        drawVLine (rabbit.first, rabbit.second, 1);
        resetColor ();
        fflush (stdout);
    }

    void TView::paint (const Control::Snake &snake)
    {
        setColor (black_, red_);
        switch (snake.direction_) {
            case Control::Snake::dir::UP: sym_ = {'/', '\\'}; break;
            case Control::Snake::dir::DOWN: sym_ = {'\\', '/'}; break;
            case Control::Snake::dir::LEFT: sym_ = {'<', ' '}; break;
            case Control::Snake::dir::RIGHT: sym_ = {' ', '>'}; break;
        }

        drawVLine (snake.body_.front ().first, snake.body_.front ().second, 1);

        setColor (black_, green_);

        for (auto curIt = ++snake.body_.begin (), endIt = snake.body_.end (); curIt != endIt; ++curIt) {
            auto prev = std::prev (curIt);

            if (prev->second != curIt->second) {
                if (prev->second < curIt->second)
                    sym_ = {'^', '^'};
                else
                    sym_ = {'v', 'v'};
            }
            else if (prev->first != curIt->first) {
                if (prev->first < curIt->first)
                    sym_ = {'<', '<'};
                else
                    sym_ = {'>', '>'};
            }
            else
                throw std::invalid_argument ("wrong snake");

            drawVLine (curIt->first, curIt->second, 1);
        }

        resetColor ();
        fflush (stdout);
    }

    void TView::write (const std::pair<std::string, size_t> &line)
    {
        std::string text = std::to_string (++alreadyWriten_) + ". " + line.first + "    " + std::to_string (line.second);

        printf ("\e[%d;%dH%s", termSize_.ws_row / 6 + alreadyWriten_, termSize_.ws_col / 2 - int (text.length ()) / 2, text.data ());
    }

    void TView::buttonHandler ()
    {
        using namespace std::chrono_literals;
        struct pollfd in = {0, POLL_IN, 0};
        std::string request = "";
        auto start = std::chrono::steady_clock::now ();

        while (std::chrono::steady_clock::now () < start + tickTime_) {
            if (poll (&in, 1, 10) == 1) {  // 10 because I can do it)))
                unsigned char c;
                read (0, &c, 1);

                request += c;

                if (auto res = buttonTable_.find (request); res != buttonTable_.end ()) {
                    res->second ();
                    request = "";
                }

                if (c == 'q') {
                    endHandler ();
                    break;
                }
            }
        }
    }

    void TView::run ()
    {
        menu ();
        resizeHandler ();
        startScreen ();
        int result;
        while (!end_) {
            buttonHandler ();
            botsHandler ();

            result = setCoordObjs ();
            if (result) {
                endHandler ();
                break;
            }

            drawing ();
        }

        if (result == 1) {
            end_ = false;
            endScreen ();
        }

        printf ("\e[1;1H\e[J");  // control clearing window
        tcsetattr (0, TCSANOW, &old_);
    }
}  // namespace graphicInterface
