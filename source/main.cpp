#include <iostream>

#include "model.hpp"
#include "view.hpp"

int main (int argc, char *argv[])
{
    std::shared_ptr<graphicInterface::View> screen = graphicInterface::View::get ((argc == 2) ? argv[1] : "text");
    gameModel::game model;

    screen->run ();

    return 0;
}