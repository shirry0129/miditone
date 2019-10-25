#define _CRT_SECURE_NO_WARNINGS

#ifdef MIDITONE_WIIBALANCEBOARD

#include "BalanceBoard.hpp"

#include <chrono>
#include "wiiuse.h"

#ifdef _DEBUG
#pragma comment(lib, "wiiuse_debug.lib")
#else
#pragma comment(lib, "wiiuse.lib")
#endif

namespace wii {
    Button::Button() : state_(State::Released) {}
    Button::~Button() {}

    bool Button::pressed() const {
        return state_ == State::Pressed || state_ == State::Down;
    }

    bool Button::released() const {
        return !pressed();
    }

    bool Button::down() const {
        return state_ == State::Down;
    }

    bool Button::up() const {
        return state_ == State::Up;
    }

    void Button::press() {
        state_ = State::Down;
    }

    void Button::release() {
        state_ = State::Up;
    }

    void Button::update() {
        if (state_ == State::Down)
            state_ = State::Pressed;
        else if (state_ == State::Up)
            state_ = State::Released;
    }


    void BasicDevice::deleter_t::cleanup(wiimote_t** wm, int n) {
        wiiuse_cleanup(wm, n);
    }

    BasicDevice::BasicDevice() : device_(wiiuse_init(1), deleter) {}

    BasicDevice::~BasicDevice() {}

    bool BasicDevice::is_connected() {
        return wiiuse_connect(device_.get(), 1);
    }

    void BasicDevice::disconnect() {
        wiiuse_disconnect(*device_);
        serial_number_.clear();
    }

    const SerialNumber<char>& BasicDevice::serial_number() const noexcept {
        return serial_number_;
    }

    BalanceBoard::BalanceBoard()
        : calibration_tl_(0.0f), calibration_tr_(0.0f), calibration_bl_(0.0f), calibration_br_(0.0f), BasicDevice() {
    }

    BalanceBoard::~BalanceBoard() {}

    bool BalanceBoard::connect(int index) {
        wiiuse_set_output(wiiuse_loglevel::LOGLEVEL_DEBUG, NULL);

        wiiuse_find(device_.get(), 1, index, 0); // argument `timeout' is unused in library

        wiimote_t* wm = *device_;
        if (wm->exp.type != EXP_WII_BOARD) {
            wiiuse_disconnect(*device_);
            return false;
        }

        wchar_t wstr[128];
        wiiuse_serial_number(*device_, wstr, sizeof(wstr));
        char str[256];
        wcstombs(str, wstr, 256);
        serial_number_.parse(str);

        return wiiuse_connect(device_.get(), 1);
    }

    void BalanceBoard::on_led() {
        wiiuse_set_leds(*device_, WIIMOTE_LED_1);
    }

    void BalanceBoard::off_led() {
        wiiuse_set_leds(*device_, WIIMOTE_LED_NONE);
    }

    void BalanceBoard::set_led(bool is_on) {
        is_on ? on_led() : off_led();
    }

    float BalanceBoard::battery_level() {
        return (*device_)->battery_level;
    }

    float BalanceBoard::top_left() const noexcept {
        wii_board_t* wb = (wii_board_t*)(&(*device_)->exp.wb);
        return wb->tl + calibration_tl_;
    }

    float BalanceBoard::top_right() const noexcept {
        wii_board_t* wb = (wii_board_t*)(&(*device_)->exp.wb);
        return wb->tr + calibration_tr_;
    }

    float BalanceBoard::bottom_left() const noexcept {
        wii_board_t* wb = (wii_board_t*)(&(*device_)->exp.wb);
        return wb->bl + calibration_bl_;
    }

    float BalanceBoard::bottom_right() const noexcept {
        wii_board_t* wb = (wii_board_t*)(&(*device_)->exp.wb);
        return wb->br + calibration_br_;
    }

    void BalanceBoard::calibration() {
        wii_board_t* wb = (wii_board_t*)(&(*device_)->exp.wb);
        calibration_tl_ = -wb->tl;
        calibration_tr_ = -wb->tr;
        calibration_bl_ = -wb->bl;
        calibration_br_ = -wb->br;
    }

    bool BalanceBoard::update() {
        front_button_.update();

        if (!wiiuse_poll(device_.get(), 1))
            return false;

        wiimote_t* wm = *device_;
        if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A)) {
            front_button_.press();
        } else if (IS_RELEASED(wm, WIIMOTE_BUTTON_A)) {
            front_button_.release();
        }

        return true;
    }

}

#endif
