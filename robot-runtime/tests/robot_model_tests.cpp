#include <string>
#include <iostream>

#include "robot_model.hpp"
#include <cassert>


int main () {

    // Given: The robot is configured to move in a positive direction.
    //  When: The robot is instructed to move, but not enough to reach its target destination.
    //  Then: The robot’s position equals the configured speed multiplied by the elapsed time, and its state is Moving.
    {
        RobotModel robot(123);

        double speed = 1;
        double targetDestination = 10;
        double elapsedTime = 2;
        double expectedPosition = 2;
        RobotState expectedState = RobotState::Moving;

        robot.configureMovement(targetDestination, speed);
        robot.move(elapsedTime);

        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }

    // Given: The robot is configured to move in a positive direction.
    //  When: The robot is instructed to move enough to reach its target destination.
    //  Then: The robot's position is equal to the target destination and its state is Idle.
    {
        RobotModel robot(123);

        double speed = 1;
        double targetDestination = 10;
        double elapsedTime = 10;
        double expectedPosition = 10;
        RobotState expectedState = RobotState::Idle;

        robot.configureMovement(targetDestination, speed);
        robot.move(elapsedTime);

        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }


    // Given: The robot is configured to move in a positive direction.
    //  When: The robot is instructed to move a distance that would overshoot the target destination.
    //  Then: The robot's position is equal to the target destination and its state is Idle.
    {
        RobotModel robot(123);

        double speed = 2;
        double targetDestination = 10;
        double elapsedTime = 7;
        double expectedPosition = targetDestination;
        RobotState expectedState = RobotState::Idle;

        robot.configureMovement(targetDestination, speed);
        robot.move(elapsedTime);

        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }

    // Given: The robot is configured to move in a negative direction.
    //  When: The robot is instructed to move, but not enough to reach its target destination.
    //  Then: The robot’s position equals the configured speed multiplied by the elapsed time multiplied by negative 1.
    {
        RobotModel robot(123);

        double speed = 1;
        double targetDestination = -10;
        double elapsedTime = 2;
        double expectedPosition = -2;
        RobotState expectedState = RobotState::Moving;

        robot.configureMovement(targetDestination, speed);
        robot.move(elapsedTime);

        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }

    // Given: The robot is configured to move in a negative direction.
    //  When: The robot is instructed to move enough to reach its target destination.
    //  Then: The robot's position is equal to the target destination and its state is Idle.
    {
        RobotModel robot(123);

        double speed = 1;
        double targetDestination = -10;
        double elapsedTime = 10;
        double expectedPosition = -10;
        RobotState expectedState = RobotState::Idle;

        robot.configureMovement(targetDestination, speed);
        robot.move(elapsedTime);

        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }

    // Given: The robot is configured to move in a negative direction.
    //  When: The robot is instructed to move a distance that would overshoot the target destination.
    //  Then: The robot's position is equal to the target destination and its state is Idle.
    {
        RobotModel robot(123);

        double speed = 2;
        double targetDestination = -10;
        double elapsedTime = 7;
        double expectedPosition = targetDestination;
        RobotState expectedState = RobotState::Idle;

        robot.configureMovement(targetDestination, speed);
        robot.move(elapsedTime);

        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }

    // Given: The robot is initialized, but not configured to move.
    //  When: The robot is instructed to move.
    //  Then: The robot's position remains unchanged and its state is Idle.
    {
        RobotModel robot(123);

        double elapsedTime = 2;
        double expectedPosition = 0;
        RobotState expectedState = RobotState::Idle;

        robot.move(elapsedTime);

        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }

    // Given: The robot is configured to move.
    //  When: The robot is instructed to move, then to stop, then to move again.
    //  Then: The robot's position remains unchanged after stopping, and its state is Idle.
    {
        RobotModel robot(123);

        double speed = 1;
        double targetDestination = 10;
        double elapsedTime = 2;
        double expectedPosition = 2;
        RobotState expectedState = RobotState::Idle;

        robot.configureMovement(targetDestination, speed);
        robot.move(elapsedTime);

        assert(robot.getPosition() == 2);
        assert(robot.getState() == RobotState::Moving);

        robot.stop();
        robot.move(elapsedTime);
        
        assert(robot.getPosition() == expectedPosition);
        assert(robot.getState() == expectedState);
    }

    return 0;
}