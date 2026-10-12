#pragma once
#include <string>


enum class RobotState {
    Idle,
    Moving
};

class RobotModel {
    public:
        RobotModel(int id);
        void configureMovement(double targetX_, double speed_);
        void move(double elapsedTime);
        void stop();
        RobotState getState();
        double getPosition();
        int getId();
        std::string getName();


    private:
        int id_;
        std::string name_;
        RobotState state_;
        double x_;
        double targetX_;
        double speed_;
        bool movementConfigured_;
};