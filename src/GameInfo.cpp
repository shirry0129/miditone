//
//  GameInfo.cpp
//  empty
//
//  Created by Shimizu Yuta on 2019/10/17.
//

#include "GameInfo.hpp"

const Size gameinfo::originalResolution = {1920, 1080};

const Size gameinfo::originalScreenCenter = gameinfo::originalResolution/2;

const String gameinfo::title = U"miditone";

const Color gameinfo::backGroundColor = Palette::Yellow;

const Color gameinfo::fadeColor = Palette::White;

const Color gameinfo::fontColor = Palette::Paleturquoise;

const Color gameinfo::easy = Palette::Limegreen;
const Color gameinfo::normal = Palette::Darkorange;
const Color gameinfo::hard = Palette::Crimson;

const int32 gameinfo::titleFontSize = 120;

const int32 gameinfo::defalutFontSize = 50;

int32 gameinfo::totalTrack;

const Key gameinfo::prev = KeyD;
const Key gameinfo::next = KeyF;
const Key gameinfo::decide = KeyJ;
const Key gameinfo::back = KeyK;

const size_t gameinfo::maxSpeed = 15;

const size_t gameinfo::maxPoint = 1000000;

const int32 gameinfo::fadeTime = 1000;

const SecondsF gameinfo::exFadeTime = 3s;
