#include "robot_model.hpp"
#include <string>

RobotModel::RobotModel(int id):
id_(id),
name_(std::to_string(id)),
state_(RobotState::Idle)
{}

int RobotModel::getId() {
    return id_;
}

std::string RobotModel::getName() {
    return name_;
}

double RobotModel::getCurrentXPosition() {
    return current_x_position_;
};

void RobotModel::setCurrentXPosition(const double current_x_position) {
    current_x_position_ = current_x_position;
};

double RobotModel::getTargetXPosition() {
    return target_x_position_;
};

void RobotModel::setTargetXPosition(const double target_x_position) {
    target_x_position_ = target_x_position;
};

void RobotModel::handleCommand(const char* command_buffer) {
    if (strcmp(command_buffer, "") == 0) {
        return;
    }
    else if (strcmp(command_buffer, "stop") == 0) {
        if (RobotModel::state_ == RobotState::Paused) {
            RobotModel::state_ = RobotState::Idle;
        }
        else if (RobotModel::state_ == RobotState::Running) {
            RobotModel::state_ = RobotState::Idle;
        }
    }
    else if (strcmp(command_buffer, "move") == 0) {
        if (RobotModel::state_ == RobotState::Idle) {
            RobotModel::state_ = RobotState::Running;
        }
        else if (RobotModel::state_ == RobotState::Paused) {
            RobotModel::state_ = RobotState::Running;
        }
    }
    else if (strcmp(command_buffer, "pause") == 0) {
        if (RobotModel::state_ == RobotState::Running) {
            RobotModel::state_ = RobotState::Paused;
        }
    }
    else if (strcmp(command_buffer, "resume") == 0) {
        if (RobotModel::state_ == RobotState::Paused) {
            RobotModel::state_ = RobotState::Running;
        }
    }
}

void RobotModel::update() {
    if (RobotModel::state_ == RobotState::Idle) {
        return;
    }
    else if (RobotModel::state_ == RobotState::Running) {
        if (RobotModel::current_x_position_ != RobotModel::target_x_position_) {
            current_x_position_++;
        }

        if (RobotModel::current_x_position_ == RobotModel::target_x_position_) {
            RobotModel::state_ = RobotState::Idle;
        }
    }
    else if (RobotModel::state_ == RobotState::Paused) {
        return;
    }
}