//
//  Play.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef Play_hpp
#define Play_hpp

#include "Common.hpp"
#include "../Lane.hpp"
#include "../Score.hpp"
#include "../Note.hpp"
#include "../../System/Score.hpp"
#include "../../System/EventTimer.hpp"
#include "../../System/TimingJudge.hpp"

namespace ui{

    class Play : public MyApp::Scene {
    private:
        score::Score m_file;
        ui::Score m_score;
        musicgame::TimingJudge judger;
        double measureLength;
        double delay;
        double beatLength;
        Audio m_song;
        Audio beatSound;
        Audio hitSound;
//        Audio holdSound;
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
        
        void drawSongInfo(const Vec2& tlPos) const;
        void drawScore(const Vec2& trPos) const;
        
        void compressedDisplay(const s3d::Vec2 &pos, const s3d::Font &assetInfo, const s3d::String &string) const;
        
    public:
        Play(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Play_hpp */
