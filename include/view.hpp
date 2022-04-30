#ifndef VIEW_HPP
#define VIEW_HPP

#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <chrono>

#include "model.hpp"

namespace graphicInterface {
    using namespace std::chrono_literals;

    class View {
    private:
        static inline const std::string_view textView = "text";
        static inline const std::string_view graphView = "graph";
    
    protected:

        std::chrono::milliseconds tickTime_ = 200ms;

    public:
        static inline std::shared_ptr<View> obj = nullptr;
        static std::shared_ptr<View> get (const std::string &type = "");

        virtual ~View () = 0;

        virtual void run () = 0;

        using coord_t = std::pair<int, int>;

        virtual coord_t getTermSize () const = 0;
        virtual void paint (const coord_t &rabbit) = 0;                       // rabbit
        virtual void paint (const Control::Snake &snake) = 0;                 // snake
        virtual void drawFrame () = 0;                                        // frame
        virtual void write (const std::pair<std::string, size_t> &line) = 0;  // text
        virtual void endHandler () = 0;

        std::function<void ()> drawing;
        std::function<int ()> setCoordObjs;
        std::function<void ()> botsHandler;
        std::function<void ()> writeScoreTable;
        std::function<void ()> resizeHandler;

        virtual void addButton (const std::string &button, const std::function<void ()> &handler) = 0;
        virtual void eraseButton (const std::string &button_) = 0;
    };
}  // namespace graphicInterface

#endif