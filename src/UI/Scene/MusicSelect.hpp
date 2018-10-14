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
#include "../../System/Score.hpp"

namespace ui{
    
    class MusicBox : IBox {
    private:
        FilePath scoreFile;
        Texture albumArt;
        score::Header musicInfo;
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
        size_t currentItem;
        Array<MusicBox> music;
    public:
        MusicSelect(const InitData& init);
        void update() override;
        void draw() const override;
    };
    
}

#endif /* MusicSelect_hpp */
