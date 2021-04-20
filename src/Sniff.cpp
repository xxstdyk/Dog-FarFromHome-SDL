#include "Sniff.h"

Sniff::Sniff() {
	SetEnabled(false);
	TextureManager::Instance()->load("../Assets/textures/dogSniffImage.png", "dogSniff");
}
Sniff::~Sniff() {

}

// Inherited via GameObject
void Sniff::Draw(){
	TextureManager::Instance()->draw("dogSniff", GetTransform()->position.x, GetTransform()->position.y);
}

void Sniff::Update(){

}

void Sniff::Clean(){

}

