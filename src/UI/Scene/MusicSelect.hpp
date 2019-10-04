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
        FilePath scoreFile;
        Texture albumArt;
        Array<RectF> diffBox;
        score::Header musicInfo;
        const size_t maxWidth;
        void compressedDisplay(const Vec2& centerPos, const Font& assetInfo, const String& string) const;
    public:
        MusicBox(const FilePath& _info, const RectF& _entity);
        FilePath getScoreFile() const;
        score::Header getMusicInfo() const;
        void draw(const Vec2& moveWidth) const;
    };

    class MusicSelect : public MyApp::Scene{
    private:
        const Vec2 boxSize;
        const RectF defaultEntity;
        Array<MusicBox> music;
        Array<Rect> instructionBox;
        Timer countDown;
        Audio example;
        double chobeg, choend;
        void resetEx();
        
        Array<FileFilter> scoreFilter;
        Array<FileFilter> musicFilter;
    public:
        MusicSelect(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* MusicSelect_hpp */
