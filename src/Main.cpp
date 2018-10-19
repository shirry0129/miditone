
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
    //Graphics::SetFullScreen(true, {1920, 1080});
    Window::SetTitle(gameinfo::title);
    Graphics::SetBackground(gameinfo::backGroundColor);
    
    FontAsset::Register(U"songTitle", 50, Typeface::Bold);
    FontAsset::Register(U"musicInfo", 30);
    FontAsset::Register(U"trackFont", 80, Typeface::Bold);
    FontAsset::Register(U"prefFont", 50, Typeface::Bold);
    FontAsset::Register(U"infoFont", 50);
    FontAsset::Register(U"songInfo", 30, Typeface::Bold);
    FontAsset::Register(U"artistInfo", 15);
    FontAsset::Register(U"diffInfo", 45, Typeface::Bold);
    FontAsset::Register(U"scoreFont", 50, Typeface::Bold);
    FontAsset::Register(U"countFont", 15);
    FontAsset::Register(U"effectFont", 30, Typeface::Bold);
    FontAsset::Register(U"comboFont", 200);
    FontAsset::Register(U"resultScore", 100, Typeface::Bold);
    FontAsset::Register(U"decisionFont", 70);
    TextureAsset::Register(U"gameStart", Resource(U"resource/gamestart.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"boxTemplate", Resource(U"resource/box.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"title", Resource(U"resource/title.jpg"));
    TextureAsset::Register(U"select", Resource(U"resource/select.jpg"));
    TextureAsset::Register(U"preference", Resource(U"resource/preference.jpg"));
    TextureAsset::Register(U"play", Resource(U"resource/play.jpg"));
    TextureAsset::Register(U"result", Resource(U"resource/result.jpg"));
    TextureAsset::Register(U"totalResult", Resource(U"resource/totalresult.jpg"));
    TextureAsset::Register(U"track", Resource(U"resource/track.png"));
    TextureAsset::Register(U"score", Resource(U"resource/score_box.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"song", Resource(U"resource/song_box_long.png"), TextureDesc::Mipped);
    TextureAsset::Register(U"gameover", Resource(U"resource/end.png"));
    TextureAsset::Register(U"hitNote", Resource(U"resource/hitnote.png"));
    TextureAsset::Register(U"arrow", Resource(U"resource/ya.png"));
    TextureAsset::Register(U"return", Resource(U"resource/turn.png"));
    TextureAsset::Register(U"instBack", Resource(U"resource/back.png"));
    TextureAsset::Register(U"resultDecision", Resource(U"resource/result_box.png"));
    TextureAsset::Register(U"resultCombo", Resource(U"resource/max_combo_.png"));
    
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
