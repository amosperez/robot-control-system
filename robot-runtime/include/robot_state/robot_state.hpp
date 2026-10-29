#pragma once
#include "robot_model.hpp"

class RobotState {
    public:
        virtual void move(double elapsedTime);
        virtual void stop();
    private:
        RobotModel robot_;
};