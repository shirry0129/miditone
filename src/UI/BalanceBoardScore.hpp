//
//  BalanceBoardScore.hpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/22.
//

#ifndef BalanceBoardScore_hpp
#define BalanceBoardScore_hpp

#include <Siv3D.hpp>
#include "BalanceBoardNote.hpp"
#include "../System/SystemScore.hpp"

namespace ui {
    class BalanceBoardScore {
    private:
        Array<Array<BalanceBoardHitNote>> hit;
        Array<Array<BalanceBoardHoldNote>> hold;
        Array<Array<BalanceBoardNote*>> score;
        float wakeUpTime;
        
    public:
        BalanceBoardScore();
        BalanceBoardScore(const score::SystemScore& _fromFile, float _speed, const Array<double>& laneAngle);
        ~BalanceBoardScore() = default;
        void setFromFile(const score::SystemScore& _fromFile, float _speed, const Array<double>& laneAngle);
        float getWakeUpTime();
        void deleteJudgedNote(size_t _lane, int spot);
        void adjustJudgingNote(size_t _lane, int spot);
        
        void update(double currentTime);
        void draw() const;
    };
};

#endif /* BalanceBoardScore_hpp */
