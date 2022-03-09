#ifndef VIEW_HPP
#define VIEW_HPP

#include <iostream>
#include <memory>

namespace graphicInterface {
    class View {
    private:
        static inline const std::string textView = "text";
        static inline const std::string graphView = "graph";

    public:
        static inline std::shared_ptr<View> obj = nullptr;
        static std::shared_ptr<View> get (const std::string type);

        virtual ~View () = 0;
        virtual void run () = 0;
    };
}  // namespace graphicInterface

#endif