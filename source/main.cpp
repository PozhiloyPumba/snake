#include <iostream>

#include "model.hpp"
#include "view.hpp"

int main (int argc, char *argv[])
{
    auto screen = graphicInterface::View::get ((argc == 2) ? argv[1] : "text");
    gameModel::Game model;

    Control::Human h1 ("\e[A", "\e[D", "\e[B", "\e[C");
    Control::Human h2 ("w", "a", "s", "d");

    model.addGamer (h1);
    model.addGamer (h2);

    screen->run ();

    return 0;
}