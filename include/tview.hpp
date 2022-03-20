#ifndef TVIEW_HPP
#define TVIEW_HPP

#include <poll.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <functional>
#include <iterator>
#include <utility>

#include "view.hpp"

namespace graphicInterface {
    class TView final : public View {
    private:
        int delay_;
        bool end_ = false;

        struct termios old_;
        std::pair<char, char> sym_{' ', ' '};

        struct winsize termSize_ = {0, 0, 0, 0};
        std::pair<unsigned short, unsigned short> virtSize_ = {0, 0};

        inline void setColor (unsigned char colorFront, unsigned char colorBack) const { printf ("\e[48;5;%dm\e[38;5;%dm", colorBack, colorFront); }
        inline void resetColor () const { printf ("\e[m\e[1;1H"); }
        void drawHLine (unsigned short xBeg, unsigned short yBeg, unsigned short length) const;  // numerate from 0
        void drawVLine (unsigned short xBeg, unsigned short yBeg, unsigned short length) const;  // numerate from 0

    public:
        TView (int fps = 120);
        ~TView () override;

        static inline std::function<void ()> interruptHandler;
        static inline std::function<void ()> changeTermSizeHandler;

        void endHandler ();

        std::pair<unsigned short, unsigned short> getTermSize () const override { return virtSize_; }
        void paint (std::pair<unsigned short, unsigned short> &rabbit) override;
        void paint (gameModel::Snake &snake) override;
        void drawFrame () override;

        void run () override;
    };
}  // namespace graphicInterface

#endif