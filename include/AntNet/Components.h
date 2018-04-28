//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_COMPONENT_H
#define ANTNET_COMPONENT_H

#include <AntNet/ECS.h>

class PositionComponent : public Component {
private:
    int xpos;
    int ypos;

public:
    int x() { return xpos; }
    int y() { return ypos; }

    void init() override {
        xpos = 0;
        ypos = 0;
    }

    void update() override {
        xpos++;
        ypos++;
    }

    void setPos(int x, int y) {
        xpos = x;
        ypos = y;
    }
};

#endif //ANTNET_COMPONENT_H
