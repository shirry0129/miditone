#pragma once

#include <vector>
#include <memory>
#include <string>
#include <array>


struct wiimote_t;

namespace wii {
    class Button {
    public:
        enum State {
            Down,       // 押された瞬間
            Up,         // 離された瞬間
            Pressed,    // 押されている状態
            Released,   // 離されている状態
        };

        Button();
        ~Button();

        bool pressed() const;
        bool released() const;
        bool down() const;
        bool up() const;

        void press();
        void release();
        void update();

    private:
        State state_;
    };


    template <typename char_type>
    class SerialNumber {
    public:
        using string_type = std::basic_string<char_type>;

        SerialNumber() {}
        SerialNumber(const char_type* serial_number_str) :
            SerialNumber(string_type(serial_number_str)) {}
        SerialNumber(string_type serial_number_str) {
            parse(serial_number_str);
        }

        ~SerialNumber() {};

        void parse(const string_type& serial_number_str) {
            if (serial_number_str.size() != 6)
                return ;

            for (size_t i = 0; i < 6; i++) {
                auto str = serial_number_str.substr(i * 2, 2);
                for (auto& ch : str) ch = std::tolower(ch);

                sn_[5 - i] = str;
            }
        }

        void clear() {
            for (auto& s : sn_) {
                s.clear();
            }
        }

        std::array<string_type, 6> sn_;
    };

    template <typename char_type>
    bool operator ==(const SerialNumber<char_type>& a, const SerialNumber<char_type>& b) {
        for (size_t i = 0; i < 6; i++) {
            if (a.sn_.at(i) != b.sn_.at(i))
                return false;
        }

        return true;
    }

    template <typename char_type>
    bool operator !=(const SerialNumber<char_type>& a, const SerialNumber<char_type>& b) {
        return !(a == b);
    }


    class BasicDevice {
    public:
        BasicDevice();
        ~BasicDevice();

        bool is_connected();
        void disconnect();
        virtual bool connect(int index = 0) = 0;

        const SerialNumber<char>& serial_number() const noexcept;

    protected:
        struct deleter_t {
            void cleanup(wiimote_t**, int);

            void operator() (wiimote_t** wm) {
                cleanup(wm, 1);
            }
        } deleter;

        std::unique_ptr<wiimote_t*, deleter_t> device_;
        SerialNumber<char> serial_number_;
    };


    class BalanceBoard : public BasicDevice {
    public:
        BalanceBoard();
        ~BalanceBoard();

        bool connect(int index = 0) override;

        void on_led();
        void off_led();
        void set_led(bool is_on);

        // between 0.0 and 1.0f
        float battery_level();

        const Button& front_button() const noexcept { return front_button_; }

        // get sensors value [kg]
        float top_left() const noexcept;
        float top_right() const noexcept;
        float bottom_left() const noexcept;
        float bottom_right() const noexcept;

        // calibrated by software
        // fix all sensors to zero value.
        void calibration();

        bool update();

        const BalanceBoard& self() { return *this; };

    private:
        Button front_button_;

        float calibration_tl_; // top left
        float calibration_tr_; // top right
        float calibration_bl_; // bottom left
        float calibration_br_; // bottom right
    };
}
