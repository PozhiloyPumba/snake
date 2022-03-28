#ifndef VIEW_HPP
#define VIEW_HPP

#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <unordered_map>

#include "model.hpp"

namespace graphicInterface {
    class View {
    private:
        static inline const std::string textView = "text";
        static inline const std::string graphView = "graph";

    public:
        static inline std::shared_ptr<View> obj = nullptr;
        static std::shared_ptr<View> get (const std::string &type = "");

        virtual ~View () = 0;

        virtual void run () = 0;

        using coord_t = std::pair<unsigned short, unsigned short>;

        virtual coord_t getTermSize () const = 0;
        virtual void paint (coord_t &rabbit) = 0;        // rabbit
        virtual void paint (Control::Snake &snake) = 0;  // snake
        virtual void drawFrame () = 0;                   // frame
        virtual void endHandler () = 0;

        std::function<void ()> drawing;
        std::function<int ()> setCoordObjs;

        std::unordered_map<std::string, std::function<void ()>> buttonHandler_;
    };
}  // namespace graphicInterface

#endif