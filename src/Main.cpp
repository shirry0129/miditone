
# include <Siv3D.hpp> // OpenSiv3D v0.2.8
#include "GameInfo.hpp"
#include "UI/Scene/Common.hpp"
#include "UI/Scene/Title.hpp"
#include "UI/Scene/MusicSelect.hpp"
#include "UI/Scene/Preference.hpp"
#include "UI/Scene/Play.hpp"
#include "UI/Scene/Result.hpp"
#include "UI/Scene/TotalResult.hpp"
#include "UI/Scene/GameOver.hpp"

void Main() {
    Window::Resize(1920, 1080);
    Window::SetTitle(gameinfo::title);
    Graphics::SetBackground(gameinfo::backGroundColor);
    
    FontAsset::Register(U"songTitle", 30, Typeface::Bold);
    FontAsset::Register(U"prefFont", 50, Typeface::Bold);
    FontAsset::Register(U"infoFont", 50);
    FontAsset::Register(U"comboFont", 200);
    TextureAsset::Register(U"gameStart", Resource(U"resource/gamestart.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"boxTemplate", Resource(U"resource/box.png"), TextureDesc::Mipped);
//    TextureAsset::Register(U"titleBG", Resource(U"resource/miditonetitle.png"));
    
    ui::MyApp sceneManager;
    sceneManager.setFadeColor(gameinfo::fadeColor);
    sceneManager.add<ui::Title>(ui::SceneName::TITLE);
    sceneManager.add<ui::MusicSelect>(ui::SceneName::MUSICSELECT);
    sceneManager.add<ui::Preference>(ui::SceneName::PREFERENCE);
    sceneManager.add<ui::Play>(ui::SceneName::PLAY);
    sceneManager.add<ui::Result>(ui::SceneName::RESULT);
    sceneManager.add<ui::TotalResult>(ui::SceneName::TOTALRESULT);
    sceneManager.add<ui::GameOver>(ui::SceneName::GAMEOVER);
    
    while (System::Update()) {
        if(!sceneManager.update()){
            break;
        }
    }
}
