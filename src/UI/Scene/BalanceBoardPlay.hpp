//
//  BaranceBoardPlay.hpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/20.
//

#ifndef BaranceBoardPlay_hpp
#define BaranceBoardPlay_hpp

#include "Common.hpp"
#include "../BalanceBoardScore.hpp"
#include "../../System/SystemScore.hpp"
#include "../../System/EventTimer.hpp"
#include "../../System/TimingJudge.hpp"

namespace ui {

    class BalanceBoardPlay : public MyApp::Scene {
    private:
        score::SystemScore m_file;
        ui::BalanceBoardScore m_score;
        musicgame::TimingJudge judger;
        double measureLength;
        double delay;
        double beatLength;
        const Audio m_song;
        const Audio beatSound;
        const Audio hitSound;
        EventTimer time;
        DecisionInfo decision;
        double pointEachNote;
        double point;
        musicgame::judge_results_t results;
        Effect decisionEffect;
        Texture shine;
        Texture albumArt;
        size_t combo;
        size_t maxWidth;
        bool isDrawable;
        
        Circle decisionLine;
        Array<double> laneAngle;
        Array<double> effectAngle;
        
        void drawSongInfo(const Vec2& tlPos) const;
        void drawScore(const Vec2& trPos) const;
        void compressedDisplay(const Vec2 &pos,const Font &assetInfo, const String &string) const;
        
    public:
        BalanceBoardPlay(const InitData& init);
        void update() override;
        void draw() const override;
    };

}

#endif /* BaranceBoardPlay_hpp */
