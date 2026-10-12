#include "robot_model.hpp"
#include <string>

RobotModel::RobotModel(int id):
id_(id),
name_(std::to_string(id)),
state_(RobotState::Idle),
x_(0),
targetX_(0),
speed_(0),
movementConfigured_(false)
{}

void RobotModel::configureMovement(double targetX, double speed) {
    targetX_ = targetX;
    speed_ = speed;
    movementConfigured_ = true;
}

void RobotModel::move(double elapsedTime) {
    if (!movementConfigured_) return;

    if (x_ == targetX_) { 
        movementConfigured_ = false; 
        state_ = RobotState::Idle;
        return; 
    }

    if (state_ == RobotState::Idle) state_ = RobotState::Moving;

    double distance = speed_ * elapsedTime;

    if (targetX_ > x_) {
        if (x_ + distance > targetX_) x_ = targetX_;
        else x_ += distance;
    } else if (targetX_ < x_) {
        if (x_ - distance < targetX_) x_ = targetX_;
        else x_ -= distance;
    } else {
        movementConfigured_ = false; 
        state_ = RobotState::Idle;
        return; 
    }

    if (x_ == targetX_) { 
        movementConfigured_ = false; 
        state_ = RobotState::Idle;
        return; 
    }
}

void RobotModel::stop() {
    movementConfigured_ = false;
    state_ = RobotState::Idle;
    speed_ = 0;
}

RobotState RobotModel::getState() {
    return state_;
}

double RobotModel::getPosition() {
    return x_;
}

int RobotModel::getId() {
    return id_;
}

std::string RobotModel::getName() {
    return name_;
}