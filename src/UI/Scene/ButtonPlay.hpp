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
#include "../ButtonScore.hpp"
#include "../ButtonNote.hpp"
#include "../../System/SystemScore.hpp"
#include "../../System/EventTimer.hpp"
#include "../../System/TimingJudge.hpp"

namespace ui{

    class ButtonPlay : public MyApp::Scene {
    private:
        const int beamHeight;
        
        score::SystemScore m_file;
        ui::ButtonScore m_score;
        musicgame::TimingJudge judger;
        double measureLength;
        double delay;
        double beatLength;
        const Audio m_song;
        const Audio beatSound;
        const Audio hitSound;
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
        Array<Quad> keyBeam;
        
        void drawSongInfo(const Vec2& tlPos) const;
        void drawScore(const Vec2& trPos) const;
        
        void compressedDisplay(const s3d::Vec2 &pos, const s3d::Font &assetInfo, const s3d::String &string) const;
        
    public:
        ButtonPlay(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Play_hpp */
