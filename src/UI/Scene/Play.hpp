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
#include "../../System/ScoreManager.hpp"
#include "../../System/EventTimer.hpp"
#include "../../System/TimingJudge.hpp"

namespace ui{

    class Play : public MyApp::Scene {
    private:
        score::ScoreManager m_file;
        ui::Score m_score;
        musicgame::TimingJudge judger;
        double measureLength;
        Audio m_song;
        Audio hitSound;
        Audio holdSound;
        EventTimer time;
        size_t combo;
        const Font comboCount;
        musicgame::judge_results_t results;
        
    public:
        Play(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* Play_hpp */
