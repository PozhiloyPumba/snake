#include <iostream>

#include "model.hpp"
#include "view.hpp"

int main (int argc, char *argv[])
{
    auto screen = graphicInterface::View::get ((argc == 2) ? argv[1] : "text");
    gameModel::Game model;

    Control::Human h1 ("arrows");
    // Control::Human h2 ("wasd");

    h1.setName ("aboba");

    model.addGamer (h1);
    // model.addGamer (h2);

    for (int i = 0; i < 1; ++i) {
        Control::StupidBot bot;
        model.addGamer (bot);
    }

    screen->run ();

    return 0;
}