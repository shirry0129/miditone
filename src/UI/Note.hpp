//
//  Note.hpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/22.
//

#ifndef Note_hpp
#define Note_hpp

#include <Siv3D.hpp>

namespace ui {
    class Note {
    protected:
        const size_t laneNum;
        const float wakeUpTime;
        Quad note;
        bool isJudged;
        bool isJudging;
        
    public:
        Note(size_t _laneNum, float _wakeUpTime);
        ~Note() = default;
        virtual void makeJudged() final;
        virtual void makeJudging() final;
        
        virtual void update(double currentTime) = 0;
        virtual void draw() const = 0;
    };
}

#endif /* Note_hpp */
