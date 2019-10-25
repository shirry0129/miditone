#ifdef MIDITONE_WIIBALANCEBOARD

#include "BalanceBoardController.hpp"

#include <cmath>


BalanceBoardController::BalanceBoardController()
    : threshold_(1.0), last_update_time_(std::chrono::system_clock::now()) {}

BalanceBoardController::~BalanceBoardController() {
    disconnect();
}

void BalanceBoardController::disconnect() {
    off_led();
    BalanceBoard::disconnect();
}

bool BalanceBoardController::connect(const wii::SerialNumber<char>& serial_number, size_t find_limits) {
    for (size_t i = 0; i < find_limits; i++) {
        BalanceBoard::connect(i);
        if (BalanceBoard::serial_number_ == serial_number) {
            BalanceBoard::on_led();
            BalanceBoardController::calibration();
            return true;
        } else {
            BalanceBoard::disconnect();
        }
    }

    return false;
}

bool BalanceBoardController::update() {
    tr_.update();
    tl_.update();
    br_.update();
    bl_.update();

    if (!BalanceBoard::update())
        return false;

    if (BalanceBoard::front_button().down())
        BalanceBoard::calibration();

    if (BalanceBoard::top_right() >= threshold_)
        tr_.press();
    else if (BalanceBoard::top_right() <= 0.1)
        tr_.release();

    if (BalanceBoard::top_left() >= threshold_)
        tl_.press();
    else if (BalanceBoard::top_left() <= 0.1)
        tl_.release();

    if (BalanceBoard::bottom_right() >= threshold_)
        br_.press();
    else if (BalanceBoard::bottom_right() <= 0.1)
        br_.release();

    if (BalanceBoard::bottom_left() >= threshold_)
        bl_.press();
    else if (BalanceBoard::bottom_left() <= 0.1)
        bl_.release();

    const auto [x, y] = gravity();

    if (std::abs(x) < 0.2 && y > 0.3)
        top_.press();
    else
        top_.release();

    if (std::abs(x) < 0.2 && y < -0.3)
        bottom_.press();
    else
        bottom_.release();

    if (std::abs(y) < 0.2 && x > 0.3)
        right_.press();
    else
        right_.release();

    if (std::abs(y) < 0.2 && x > 0.3)
        left_.press();
    else
        left_.release();

    last_update_time_ = std::chrono::system_clock::now();

    return true;
}

double BalanceBoardController::time_from_last_update() {
    auto now = std::chrono::system_clock::now();       // 計測終了時刻を保存
    auto dur = now - last_update_time_;        // 要した時間を計算
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    return msec / 1000.0;
}


bool BalanceBoardController::calibration() {
    if (!update())
        return false;

    BalanceBoard::calibration();

    return true;
}

sensor_weight BalanceBoardController::weight() const {
    sensor_weight weight;
    weight.top_left = BalanceBoard::top_left();
    weight.top_right = BalanceBoard::top_right();
    weight.bottom_left = BalanceBoard::bottom_left();
    weight.bottom_right = BalanceBoard::bottom_right();

    return weight;
}

std::pair<double, double> BalanceBoardController::gravity(double threshold) {
    double right_side_weight = (double)BalanceBoard::top_right() + (double)BalanceBoard::bottom_right();
    double left_side_weight = (double)BalanceBoard::top_left() + (double)BalanceBoard::bottom_left();
    double upper_side_weight = (double)BalanceBoard::top_right() + (double)BalanceBoard::top_left();
    double lower_side_weight = (double)BalanceBoard::bottom_left() + (double)BalanceBoard::bottom_right();

    float gravity_x;
    if ((right_side_weight + left_side_weight) < threshold)
        gravity_x = 0.0;
    else
        gravity_x = -0.5 + right_side_weight / (right_side_weight + left_side_weight);

    float gravity_y;
    if ((upper_side_weight + lower_side_weight) < threshold)
        gravity_y = 0.0;
    else
        gravity_y = -0.5 + upper_side_weight / (upper_side_weight + lower_side_weight);

    gravity_x = std::fmax(-0.5, gravity_x);
    gravity_x = std::fmin(0.5, gravity_x);
    gravity_y = std::fmax(-0.5, gravity_y);
    gravity_y = std::fmin(0.5, gravity_y);

    return std::pair(gravity_x, gravity_y);
}



AsyncBalanceBoardController::AsyncBalanceBoardController() : thread_continue(false) {}

AsyncBalanceBoardController::~AsyncBalanceBoardController() {
    thread_continue = false;

    if (thread_.first.joinable())
        thread_.first.join();

    board_.disconnect();
}

bool AsyncBalanceBoardController::start_connect(const wii::SerialNumber<char>& serial_number, size_t find_limits) {
    if (thread_.first.joinable())
        return false; // thread already exists

    auto connect_async_func = [this, &serial_number, find_limits] () {
        while (!board_.is_connected() && thread_continue) {
            board_.connect(serial_number, find_limits);
        }
    };

    thread_continue = true;
    thread_.first = std::thread(connect_async_func);
    thread_.second = async_operation_type::connect;

    return true;
}

bool AsyncBalanceBoardController::wait_connect() {
    if (thread_.second != async_operation_type::connect)
        return false;

    if (thread_.first.joinable())
        thread_.first.join();

    thread_continue = false;

    return true;
}

bool AsyncBalanceBoardController::stop_connect() {
    if (thread_.second != async_operation_type::connect)
        return false;

    thread_continue = false;

    if (thread_.first.joinable())
        thread_.first.join();

    return true;
}

bool AsyncBalanceBoardController::start_update() {
    if (thread_.first.joinable())
        return false; // thread already exists

    auto update_async_func = [this] () {
        while (thread_continue)
            board_.update();
    };

    thread_continue = true;
    thread_.first = std::thread(update_async_func);
    thread_.second = async_operation_type::update;

    return true;
}

bool AsyncBalanceBoardController::stop_update() {
    if (thread_.second != async_operation_type::update)
        return false;

    thread_continue = false;

    if (thread_.first.joinable())
        thread_.first.join();

    return true;
}

void AsyncBalanceBoardController::set_threshold(float kg) {
    std::lock_guard<std::mutex> lock(board_mutex_);

    board_.set_threshold(kg);
}

void AsyncBalanceBoardController::disconnect() {
    thread_continue = false;

    if (thread_.first.joinable())
        thread_.first.join();

    board_.disconnect();
}

float AsyncBalanceBoardController::battery_level() {
    std::lock_guard<std::mutex> lock(board_mutex_);

    return board_.battery_level();
}

bool AsyncBalanceBoardController::is_connected() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.is_connected();
}

wii::Button AsyncBalanceBoardController::top_left() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.top_left();
}

wii::Button AsyncBalanceBoardController::top_right() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.top_right();
}

wii::Button AsyncBalanceBoardController::bottom_left() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.bottom_left();
}

wii::Button AsyncBalanceBoardController::bottom_right() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.bottom_right();
}

double AsyncBalanceBoardController::time_from_last_update() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.time_from_last_update();
}

sensor_weight AsyncBalanceBoardController::weight() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.weight();
}

std::pair<double, double>  AsyncBalanceBoardController::gravity(double threshold) {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.gravity(threshold);
}

bool AsyncBalanceBoardController::calibration() {
    std::lock_guard<std::mutex> lock(board_mutex_);
    return board_.calibration();
}

#endif
