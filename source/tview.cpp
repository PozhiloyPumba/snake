#include "tview.hpp"

namespace graphicInterface {

    void sigintHandler (int sigN)
    {
        TView::funcHandler (sigN);
    }

    TView::TView (int fps) : delay_ (1000000 / fps)
    {
        tcgetattr (0, &old_);
        struct termios raw;
        cfmakeraw (&raw);
        raw.c_lflag |= ISIG;
        raw.c_cc[VINTR] = 'q';
        tcsetattr (0, TCSANOW, &raw);

        funcHandler = std::bind (&TView::handler, this, std::placeholders::_1);
        signal (SIGINT, &graphicInterface::sigintHandler);
    }

    TView::~TView ()
    {
    }
    
    void TView::handler (int sigN)
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
        struct winsize w;

        ioctl (STDOUT_FILENO, TIOCGWINSZ, &w);

        if (w.ws_col == termSize_.ws_col && w.ws_row == termSize_.ws_row)
            return;
        termSize_ = w;

        printf ("\e[1;1H\e[J");

        virtSize_ = {w.ws_col / 2, w.ws_row - w.ws_row % 2};

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
        printf ("\e[1;1H\e[J");
        fflush (stdout);
        while (!end_) {
            drawFrame ();
            usleep (delay_);
        }
    }
}  // namespace graphicInterface
