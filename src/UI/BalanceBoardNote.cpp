#include "BalanceBoardNote.hpp"
#include "../GameInfo.hpp"

namespace ui {
    BalanceBoardNote::BalanceBoardNote(size_t _laneNum, float _wakeUpTime, double _angle, float _speed, bool _isConcurrent) :
    Note(_laneNum, _wakeUpTime),
    angle(_angle),
    speed(_speed),
    isConcurrent(_isConcurrent){}

    void BalanceBoardNote::setIsConcurrent(bool _concurrent) {
        isConcurrent = _concurrent;
    }




    BalanceBoardHitNote::BalanceBoardHitNote(size_t _laneNum, float _judgeTime, float _wakeUpTime, double _angle, float _speed, bool _isConcurrent) :
    BalanceBoardNote(_laneNum, _wakeUpTime, _angle, _speed, _isConcurrent),
    judgeTime(_judgeTime){}

    void BalanceBoardHitNote::update(double currentTime) {
        double time = wakeUpTime - (judgeTime - currentTime);

        if (time < 0) {
            note.set(0, 0, 0, 0, 0, 0, 0, 0);
            return;
        }

        float nextX = time * speed + 100;

        Vec2 tl(gameinfo::originalScreenCenter + Vec2(nextX - (noteWidth / 2), -(noteHeight / 2)));
        Vec2 tr(gameinfo::originalScreenCenter + Vec2(nextX + (noteWidth / 2), -(noteHeight / 2))); 
        Vec2 bl(gameinfo::originalScreenCenter + Vec2(nextX - (noteWidth / 2), (noteHeight / 2)));
        Vec2 br(gameinfo::originalScreenCenter + Vec2(nextX + (noteWidth / 2), (noteHeight / 2)));

        note.set(tl, tr, br, bl);
    }

    void BalanceBoardHitNote::draw() const {
        Texture noteTexture = TextureAsset(U"hitNote");
        
        if (isConcurrent) {
            noteTexture = TextureAsset(U"concurrentHitNote");
        }
        
        Transformer2D laneRotate(Mat3x2::Rotate(angle, gameinfo::originalScreenCenter));
        {
            Transformer2D noteRotate(Mat3x2::Rotate(90_deg, Float2(note.p0 + Float2(noteWidth / 2, noteHeight / 2))));

            if (!isJudged) {
                if (note.p0.x + noteWidth / 2 < gameinfo::originalScreenCenter.x + lineRadius + 50) {
                    note(noteTexture).draw();
                }
            }
        }
    }





    BalanceBoardHoldNote::BalanceBoardHoldNote(size_t _laneNum, float _startTime, float _endTime, float _wakeUpTime, double _angle, float _speed, bool _isConcurrent) :
    BalanceBoardNote(_laneNum, _wakeUpTime, _angle, _speed, _isConcurrent),
    startTime(_startTime),
    endTime(_endTime){}

    void BalanceBoardHoldNote::update(double currentTime) {
        double begTime = wakeUpTime - (startTime - currentTime);
        double finTime = wakeUpTime - (endTime - currentTime);

        if (begTime < 0) {
            note.set(0, 0, 0, 0, 0, 0, 0, 0);
            return;
        }

        float firstX = begTime * speed + 100;
        float lastX = (finTime < 0) ? 100 : finTime * speed + 100;

        if (isJudging) {
            firstX = lineRadius;
        }

        Vec2 tl(gameinfo::originalScreenCenter + Vec2(lastX, -(noteWidth / 2)));
        Vec2 tr(gameinfo::originalScreenCenter + Vec2(firstX, -(noteWidth / 2)));
        Vec2 bl(gameinfo::originalScreenCenter + Vec2(lastX, noteWidth / 2));
        Vec2 br(gameinfo::originalScreenCenter + Vec2(firstX, noteWidth / 2));

        note.set(tl, tr, br, bl);

        tl = Vec2(gameinfo::originalScreenCenter + Vec2(lastX - (noteWidth / 2), -(noteHeight / 2)));
        tr = Vec2(gameinfo::originalScreenCenter + Vec2(lastX + (noteWidth / 2), -(noteHeight / 2)));
        bl = Vec2(gameinfo::originalScreenCenter + Vec2(lastX - (noteWidth / 2), (noteHeight / 2)));
        br = Vec2(gameinfo::originalScreenCenter + Vec2(lastX + (noteWidth / 2), (noteHeight / 2)));

        end.set(tl, tr, br, bl);

        tl = Vec2(gameinfo::originalScreenCenter + Vec2(firstX - (noteWidth / 2), -(noteHeight / 2)));
        tr = Vec2(gameinfo::originalScreenCenter + Vec2(firstX + (noteWidth / 2), -(noteHeight / 2)));
        bl = Vec2(gameinfo::originalScreenCenter + Vec2(firstX - (noteWidth / 2), (noteHeight / 2)));
        br = Vec2(gameinfo::originalScreenCenter + Vec2(firstX + (noteWidth / 2), (noteHeight / 2)));

        start.set(tl, tr, br, bl);
    }

    void BalanceBoardHoldNote::draw() const {
        Vec2 startCenter(start.p0 + Vec2(noteWidth / 2, noteHeight / 2));
        Vec2 endCenter(end.p0 + Vec2(noteWidth / 2, noteHeight / 2));
        
        Texture noteTexture = TextureAsset(U"hitNote");
        Color noteColor = Palette::Aquamarine;
        
        if (isConcurrent) {
            noteTexture = TextureAsset(U"concurrentHitNote");
            noteColor = Palette::Lemonchiffon;
        }

        Transformer2D laneRotate(Mat3x2::Rotate(angle, gameinfo::originalScreenCenter));
        if (!isJudged) {
            if (startCenter.x > gameinfo::originalScreenCenter.x && endCenter.x < gameinfo::originalScreenCenter.x + lineRadius + 50) {
                note.draw(noteColor);
                {
                    Transformer2D noteRotate(Mat3x2::Rotate(90_deg, startCenter));
                    start(noteTexture).draw();
                }
            }
            if (endCenter.x > gameinfo::originalScreenCenter.x && endCenter.x < gameinfo::originalScreenCenter.x + lineRadius + 50) {
                Transformer2D noteRotate(Mat3x2::Rotate(90_deg, endCenter));
                end(noteTexture).draw();
            }
        }
    }

}
