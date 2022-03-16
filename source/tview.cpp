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

        TView::changeTermSizeHandler();
    }

    TView::TView (int fps) : delay_ (1000000 / fps)
    {
        tcgetattr (0, &old_);
        struct termios raw;
        cfmakeraw (&raw);
        raw.c_lflag |= ISIG;
        raw.c_cc[VINTR] = 3;    // ctrl + C
        tcsetattr (0, TCSANOW, &raw);

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
        tcsetattr (0, TCSANOW, &old_);
        end_ = true;
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

        setColor (colorFrameBack_, colorFrameFore_);
        drawHLine (0, 0, virtSize_.first);
        drawHLine (0, virtSize_.second - 1, virtSize_.first);
        drawVLine (0, 0, virtSize_.second);
        drawVLine (virtSize_.first - 1, 0, virtSize_.second);
        fflush (stdout);
        resetColor ();
    }

    void TView::run ()
    {
        struct pollfd in = {0, POLL_IN, 0};
        drawFrame ();

        while (!end_) {

            if (poll (&in, 1, delay_) == 1) {
                char c;
                read (0, &c, 1);

                if (c == 'q')
                    endHandler ();
            }
        }
    }
}  // namespace graphicInterface
