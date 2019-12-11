//
//  MusicSelect.hpp
//  MusicGame
//
//  Created by Shimizu Yuta on 2018/10/08.
//

#ifndef MusicSelect_hpp
#define MusicSelect_hpp

#include "Common.hpp"
#include "../../GameInfo.hpp"
#include "../../System/SystemScore.hpp"

namespace ui{
    
    class MusicBox : IBox {
    private:
        const Texture albumArt;
        const String title;
        const String artist;
        Array<String> level;
        Array<RectF> diffBox;
        const size_t maxWidth;
        void compressedDisplay(const Vec2& centerPos, const Font& assetInfo, const String& string) const;
    public:
        MusicBox(const ScoreData& _info, const RectF& _entity);
        void draw(const Vec2& moveWidth) const;
    };

    class MusicSelect : public MyApp::Scene{
    private:
        const Vec2 boxSize;
        const RectF defaultEntity;
        Array<MusicBox> musics;
        Array<Rect> instructionBox;
        Timer countDown;
        Audio example;
        double chobeg, choend;
        void resetEx();
        void drawHighScore(Vec2 tlPos) const;
    public:
        MusicSelect(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* MusicSelect_hpp */
