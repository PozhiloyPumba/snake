#ifndef TVIEW_HPP
#define TVIEW_HPP

#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <poll.h>

#include <functional>
#include <utility>

#include "view.hpp"

namespace graphicInterface {
    class TView final : public View {
    private:
        int delay_;
        bool end_ = false;

        struct termios old_;
        const short colorFrameBack_ = 41;
        const short colorFrameFore_ = 34;
        std::pair<char, char> sym_{' ', ' '};

        struct winsize termSize_ = {0, 0, 0, 0};
        std::pair<unsigned short, unsigned short> virtSize_ = {0, 0};

        inline void setColor (char colorBack, char colorFront) const { printf ("\e[%dm\e[%dm", colorFront, colorBack); }
        inline void resetColor () const { printf ("\e[m"); }
        void drawHLine (unsigned short xBeg, unsigned short yBeg, unsigned short length) const;  // numerate from 0
        void drawVLine (unsigned short xBeg, unsigned short yBeg, unsigned short length) const;  // numerate from 0

    public:
        TView (int fps = 120);

        static inline std::function<void ()> interruptHandler;
        static inline std::function<void ()> changeTermSizeHandler;

        void endHandler ();

        ~TView () override;

        void drawFrame ();
        void run () override;
    };
}  // namespace graphicInterface

#endif