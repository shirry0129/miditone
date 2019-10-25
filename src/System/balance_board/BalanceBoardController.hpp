#pragma once

#include "BalanceBoard.hpp"

#include <chrono>
#include <mutex>
#include <functional>


struct sensor_weight {
    float top_left;
    float top_right;
    float bottom_left;
    float bottom_right;
};

class BalanceBoardController : protected wii::BalanceBoard {
public:
    BalanceBoardController();
    ~BalanceBoardController();

    // [summary]
    //   入力のしきい値を設定する
    // [params]
    //   kg 押されたと判定するしきい値 [kg]
    void set_threshold(float kg) { threshold_ = kg; }

    // 切断する
    void disconnect();

    // バッテリー残量を取得する
    using wii::BalanceBoard::battery_level;

    // 接続されているか返す
    using wii::BalanceBoard::is_connected;

    // [summary]
    //   接続する
    // [params]
    //   serial_number 接続する筐体のシリアル番号 (MACアドレス)
    //   find_limits   探索するデバイスの数
    // [return]
    //   接続成功でtrue
    bool connect(const wii::SerialNumber<char>& serial_number, size_t find_limits = 5);

    // [summary]
    //   入力状態を更新する
    // [return]
    //   通信成功でtrue
    bool update();

    // 左上センサー
    const wii::Button& top_left() const { return tl_; }
    // 右上センサー
    const wii::Button& top_right() const { return tr_; }
    // 左下センサー
    const wii::Button& bottom_left() const { return bl_; }
    // 右下センサー
    const wii::Button& bottom_right() const { return br_; }

    // 重心計算による仮想ボタン
    const wii::Button& top() const { return top_; }
    const wii::Button& right() const { return right_; }
    const wii::Button& bottom() const { return bottom_; }
    const wii::Button& left() const { return left_; }

    // [summary]
    //   最後に更新されてから現在までの時間を返す
    // [return]
    //   最終更新からの時間 [秒] 1ms刻み
    double time_from_last_update();

    // [summary]
    //   キャリブレーションを行う
    bool calibration();

    sensor_weight weight() const;

    // [summary]
    //    重心を取得する
    // [return]
    //    first 左右の重心 (-0.5 ~ 0.5) 左が正
    //    second 上下の重心 (-0.5 ~ 0.5) 上が正
    std::pair<double, double> gravity(double threshold = 0.2);

private:
    wii::Button tr_;
    wii::Button tl_;
    wii::Button br_;
    wii::Button bl_;
    wii::Button top_;
    wii::Button right_;
    wii::Button bottom_;
    wii::Button left_;

    // [kg]
    float threshold_;

    std::chrono::time_point<std::chrono::system_clock> last_update_time_;
};


// 非同期通信版
class AsyncBalanceBoardController {
public:
    AsyncBalanceBoardController();
    ~AsyncBalanceBoardController();

    // [summary]
    //   非同期で接続を開始する
    // [params]
    //   serial_number 接続する筐体のシリアル番号 (MACアドレス)
    //   find_limits   探索するデバイスの数
    // [return]
    //   正常に非同期で実行開始できたらtrue
    bool start_connect(const wii::SerialNumber<char>& serial_number, size_t find_limits = 5);

    // [summary]
    //   接続されるまで待つ
    bool wait_connect();

    // [summary]
    //   接続処理を終了
    bool stop_connect();

    // [summary]
    //   非同期で入力状態の更新を開始する
    // [return]
    //   正常に非同期で実行開始できたらtrue
    bool start_update();

    // [summary]
    //   入力状態の更新を終了する
    bool stop_update();

    // [summary]
    //   入力のしきい値を設定する
    // [params]
    //   kg 押されたと判定するしきい値 [kg]
    void set_threshold(float kg);

    // 切断する
    // 別スレッドの処理も停止します
    void disconnect();

    // バッテリー残量を取得する
    float battery_level();

    // 接続されているか返す
    bool is_connected();

    // 左上センサー
    wii::Button top_left();
    // 右上センサー
    wii::Button top_right();
    // 左下センサー
    wii::Button bottom_left();
    // 右下センサー
    wii::Button bottom_right();

    // [summary]
    //   最後に更新されてから現在までの時間を返す
    // [return]
    //   最終更新からの時間 [秒] 1ms刻み
    double time_from_last_update();

    sensor_weight weight();

    // [summary]
    //    重心を取得する
    // [return]
    //    first 左右の重心 (-0.5 ~ 0.5) 左が正
    //    second 上下の重心 (-0.5 ~ 0.5) 上が正
    std::pair<double, double> gravity(double threshold = 0.2);

    // [summary]
    //   キャリブレーションを行う
    bool calibration();

private:
    BalanceBoardController board_;

    enum async_operation_type {
        update, connect
    };

    std::pair<std::thread, async_operation_type> thread_;
    std::mutex board_mutex_;
    bool thread_continue;
};
