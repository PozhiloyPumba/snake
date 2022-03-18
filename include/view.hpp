#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>
#include <memory>
#include <functional>
#include <list>
#include "model.hpp"


namespace graphicInterface {
    class View {
    private:
        static inline const std::string textView = "text";
        static inline const std::string graphView = "graph";

    public:
        static inline std::shared_ptr<View> obj = nullptr;
        static std::shared_ptr<View> get (const std::string &type = textView);

        virtual ~View () = 0;

        virtual void run () = 0;
        
        using rabbit_t = std::pair<unsigned short, unsigned short>;

        virtual rabbit_t getTermSize () const = 0;
        virtual void paint (rabbit_t &rabbit) = 0;   // rabbit
        virtual void paint (gameModel::Snake &snake) = 0;   // rabbit

        std::function <void ()> drawing;
    };
}  // namespace graphicInterface

#endif