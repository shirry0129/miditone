
#ifndef BalanceBoardNote_hpp
#define BalanceBoardNote_hpp

#include <Siv3D.hpp>
#include "Note.hpp"

namespace ui {

    constexpr double noteWidth = 200;
    constexpr double noteHeight = 50;
    constexpr int lineRadius = 450;

    class BalanceBoardNote : public Note {
    protected:
        double angle;
        float speed;
        bool isConcurrent;

    public:
        BalanceBoardNote(size_t _laneNum, float _wakeUpTime, double _angle, float _speed, bool _isConcurrent);
        ~BalanceBoardNote() = default;
        void setIsConcurrent(bool _concurrent);

        virtual void update(double currentTime) = 0;
        virtual void draw() const = 0;
    };

    class BalanceBoardHitNote final : public BalanceBoardNote {
    private:

    public:
        const float judgeTime;
        BalanceBoardHitNote(size_t _laneNum, float _judgeTime, float _wakeUpTime, double _angle, float _speed, bool _isConcurrent);

        virtual void update(double currentTime) override;
        virtual void draw() const override;
    };

    class BalanceBoardHoldNote final : public BalanceBoardNote {
    private:
        Quad start;
        Quad end;

    public:
        const float startTime;
        const float endTime;

        BalanceBoardHoldNote(size_t _laneNum, float _startTime, float _endTime, float _wakeUpTime, double _angle, float _speed, bool _isConcurrent);
        virtual void update(double currentTime) override;
        virtual void draw() const override;
    };

}
#endif // BalanceBoardNote_hpp
