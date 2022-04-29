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
#include <type_traits>

#include "view.hpp"

namespace graphicInterface {
    class TView final : public View {
    private:
        const int lightGrey_ = 249;
        const int black_ = 0;
        const int purple_ = 62;
        const int red_ = 196;
        const int green_ = 46;

        bool end_ = false;

        struct termios old_;
        std::pair<char, char> sym_{' ', ' '};

        struct winsize termSize_ = {0, 0, 0, 0};
        std::pair<int, int> virtSize_ = {0, 0};

        std::unordered_map<std::string, std::function<void ()>> buttonTable_;

        inline void setColor (unsigned char colorFront, unsigned char colorBack) const { printf ("\e[48;5;%dm\e[38;5;%dm", colorBack, colorFront); }
        inline void resetColor () const { printf ("\e[m\e[1;1H"); }
        void drawHLine (int xBeg, int yBeg, int length) const;  // numerate from 0
        void drawVLine (int xBeg, int yBeg, int length) const;  // numerate from 0
        void buttonHandler ();
        void startScreen ();
        void drawBigDigit (std::integral_constant<int, 1>) {setColor (black_, purple_); drawVLine (10, 10, 1); resetColor ();}  //TODO:
        void drawBigDigit (std::integral_constant<int, 2>) {setColor (black_, purple_); drawVLine (20, 20, 1); resetColor ();}
        void drawBigDigit (std::integral_constant<int, 3>) {setColor (black_, purple_); drawVLine (30, 30, 1); resetColor ();}

    public:
        TView ();
        ~TView () override = default;

        static inline std::function<void ()> interruptHandler;
        static inline std::function<void ()> changeTermSizeHandler;

        void endHandler () override;

        std::pair<int, int> getTermSize () const override { return virtSize_; }
        void paint (const std::pair<int, int> &rabbit) override;
        void paint (const Control::Snake &snake) override;
        void write (const std::pair<std::string, size_t> &line) override {}  // TODO:
        void drawFrame () override;

        void addButton (const std::string &button, const std::function<void ()> &handler) override { buttonTable_.insert ({button, handler}); }
        void eraseButton (const std::string &button) override
        {
            auto res = buttonTable_.find (button);
            if (res != buttonTable_.end ())
                buttonTable_.erase (res);
        }

        void run () override;
    };
}  // namespace graphicInterface

#endif