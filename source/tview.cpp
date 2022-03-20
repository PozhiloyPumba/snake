#include "tview.hpp"

namespace graphicInterface {

    void sigintHandler (int sigN)
    {
        if (sigN != SIGINT)
            throw std::invalid_argument ("strange signal " + sigN);

        TView::interruptHandler ();
    }

    void sigChangeTermSizeHandler (int sigN)
    {
        if (sigN != SIGWINCH)
            throw std::invalid_argument ("strange signal " + sigN);

        TView::changeTermSizeHandler ();
    }

    TView::TView (int fps) : delay_ (1000000 / fps)
    {
        tcgetattr (0, &old_);
        struct termios raw;
        cfmakeraw (&raw);
        raw.c_lflag |= ISIG;
        raw.c_cc[VINTR] = 3;  // ctrl + C
        tcsetattr (0, TCSANOW, &raw);

        ioctl (STDOUT_FILENO, TIOCGWINSZ, &termSize_);
        virtSize_ = {termSize_.ws_col / 2, termSize_.ws_row};

        interruptHandler = std::bind (&TView::endHandler, this);
        signal (SIGINT, &graphicInterface::sigintHandler);
        changeTermSizeHandler = std::bind (&TView::drawFrame, this);
        signal (SIGWINCH, &graphicInterface::sigChangeTermSizeHandler);
    }

    TView::~TView ()
    {
    }

    void TView::endHandler ()
    {
        end_ = true;
        tcsetattr (0, TCSANOW, &old_);
    }

    void TView::drawHLine (unsigned short xBeg, unsigned short yBeg, unsigned short length) const
    {
        xBeg = xBeg * 2 + 1;
        ++yBeg;

        printf ("\e[%d;%dH", yBeg, xBeg);
        for (short i = 0; i < length; ++i)
            printf ("%c%c", sym_.first, sym_.second);
    }

    void TView::drawVLine (unsigned short xBeg, unsigned short yBeg, unsigned short length) const
    {
        xBeg = xBeg * 2 + 1;
        ++yBeg;

        for (short i = 0; i < length; ++i) {
            printf ("\e[%d;%dH", yBeg++, xBeg);
            printf ("%c%c", sym_.first, sym_.second);
        }
    }

    void TView::drawFrame ()
    {
        ioctl (STDOUT_FILENO, TIOCGWINSZ, &termSize_);

        printf ("\e[1;1H\e[J");

        virtSize_ = {termSize_.ws_col / 2, termSize_.ws_row};

        setColor (purple_, purple_);
        sym_ = {' ', ' '};
        drawHLine (0, 0, virtSize_.first);
        drawHLine (0, virtSize_.second - 1, virtSize_.first);
        drawVLine (0, 0, virtSize_.second);
        drawVLine (virtSize_.first - 1, 0, virtSize_.second);
        resetColor ();
        fflush (stdout);
    }

    void TView::paint (std::pair<unsigned short, unsigned short> &rabbit)
    {
        setColor (black_, lightGrey_);
        sym_ = {' ', ' '};
        drawVLine (rabbit.first, rabbit.second, 1);
        resetColor ();
        fflush (stdout);
    }

    void TView::paint (gameModel::Snake &snake)
    {
        setColor (black_, red_);
        switch (snake.direction_) {
            case gameModel::Snake::dir::UP: sym_ = {'/', '\\'}; break;
            case gameModel::Snake::dir::DOWN: sym_ = {'\\', '/'}; break;
            case gameModel::Snake::dir::LEFT: sym_ = {'<', ' '}; break;
            case gameModel::Snake::dir::RIGHT: sym_ = {' ', '>'}; break;
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

    void TView::run ()
    {
        drawFrame ();

        while (!end_) {
            if (setCoordObjs ()) {
                endHandler ();
                break;
            }

            drawing ();
        }

        printf ("\e[1;1H\e[J"); // clearing window
        printf ("Game Over\n");
    }
}  // namespace graphicInterface
