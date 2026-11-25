#pragma once
#include <string>

enum class RobotState {
    Idle,
    Running,
    Paused
};

class RobotModel {
    public:
        RobotModel(int id);

        int getId();
        std::string getName();
        void setName();

        double getCurrentXPosition();
        void setCurrentXPosition(double current_x_position);

        double getTargetXPosition();
        void setTargetXPosition(double target_x_position);

        void handleCommand(const char* command_buffer);
        void update();

    private:
        int id_;
        std::string name_;
        RobotState state_;

        double current_x_position_;
        double target_x_position_;
};