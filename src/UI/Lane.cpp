#include"Lane.hpp"

namespace ui {
    
    /**
     Constructor
     */
    LaneBG::LaneBG():
    decisionLine(210,1000,1710,1000),
    laneBegin(Window::Width()/2, -200),
    interval(300, 0),
    leftEnd(360,Window::Height()),
    rightEnd(leftEnd + interval * 4)
    {
        for(auto i:step(5)){
            guide.emplace_back(laneBegin, leftEnd + interval * i);
        }
        
        for(auto g: guide){
            slope.push_back(g.vector().x / g.vector().y);
        }
        
        for(auto s: slope){
            intercept.push_back(laneBegin.x - s * laneBegin.y);
        }
    }
    
    /**
     シングルトンのインスタンスを取得
     
     @return インスタンスへの参照
     */
    LaneBG& LaneBG::getInstance(){
        return *instance;
    }
    
    /**
     インスタンスの生成
     */
    void LaneBG::create(){
        if (!instance) {
            instance = new LaneBG;
        }
    }
    
    /**
     インスタンスの削除
     */
    void LaneBG::destroy(){
        delete instance;
        instance = nullptr;
    }
    
    /**
     描画情報の更新
     */
    void LaneBG::update(){}
    
    /**
     レーンとガイドを描画
     */
    void LaneBG::draw(){
        // draw background
        Triangle(laneBegin,leftEnd,rightEnd).draw(Color(65, 65, 65));
        
        // draw guide
        for(auto i:step(5)){
            guide[i].draw(LineStyle::RoundCap, (i == 0 || i == 4) ? 8 : 2, Palette::Orange);
        }
        decisionLine.draw(LineStyle::RoundCap, 8, Palette::Orange);
    }
    
    
    /**
     ガイドの係数取得

     @param guide ガイド数
     @return 係数
     */
    factor LaneBG::getFactor(int guide) {
        return {slope[guide], intercept[guide]};
    }    
    
    LaneBG* LaneBG::instance = nullptr;
    
}
