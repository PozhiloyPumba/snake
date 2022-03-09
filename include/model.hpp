#ifndef MODEL_HPP
#define MODEL_HPP

#include <list>
#include <random>
#include <utility>

class game final {
private:
    std::list<std::pair<unsigned short, unsigned short>> rabbits_;

public:
    game ();
};

#endif