#include "robot_runtime.hpp"
#include <cassert>


int main () {

    {
        RobotRuntime robot_runtime;

        robot_runtime.run();
    }

    return 0;
}