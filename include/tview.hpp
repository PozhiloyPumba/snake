#ifndef TVIEW_HPP
#define TVIEW_HPP

#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>

#include <utility>

#include "view.hpp"

namespace graphicInterface {
    class TView final : public View {
    private:
        int delay_;
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
        TView (int fps = 120) : delay_ (1000000 / fps) {}

        ~TView () override;

        void drawFrame ();
        void run () override;
    };
}  // namespace graphicInterface

#endif