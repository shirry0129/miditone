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
        Audio holdSound;
        EventTimer time;
        DecisionInfo decision;
        double pointEachNote;
        double point;
        musicgame::judge_results_t results;
        Effect decisionEffect;
        Texture shine;
        size_t combo;
        bool isDrawable;
        
    public:
        Play(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Play_hpp */
