#include "CheckPointManager.h"

CheckPointManager::CheckPointManager(sf::Texture *cpTexture) {	
	checkpoints.push_back(CheckPoint(0, 432, sf::Vector2f(1, 2), RT, cpTexture));
	checkpoints.push_back(CheckPoint(1, 432, sf::Vector2f(3, 1), LT, cpTexture));	
	checkpoints.push_back(CheckPoint(2, 432, sf::Vector2f(4, 0), BL, cpTexture));
	checkpoints.push_back(CheckPoint(3, 432, sf::Vector2f(5, 1), LB, cpTexture));	
	checkpoints.push_back(CheckPoint(4, 432, sf::Vector2f(5, 2), BR, cpTexture));
	checkpoints.push_back(CheckPoint(5, 432, sf::Vector2f(4, 3), LB, cpTexture));
	checkpoints.push_back(CheckPoint(6, 432, sf::Vector2f(2, 2), RB, cpTexture));
	checkpoints.push_back(CheckPoint(7, 432, sf::Vector2f(1, 3), LB, cpTexture));
}

void CheckPointManager::Draw(sf::RenderTarget& window) {
	for (auto& c : checkpoints)
		c.Draw(window);
}

CheckPoint::CheckPoint(int id, int ts, sf::Vector2f pos, posInTile pit, sf::Texture* tex) : ID(id), tileSize(ts), position(pos), PIT(pit), texture(tex) {
	isStart = ID == 0 ? true : false;

	float interval = (float)ts / 8;
	position.x = ts * pos.x;
	position.y = ts * pos.y;

	//sprite setup
	sprite.setPosition(position);
	sprite.setTexture(*texture);
	sprite.scale((float)ts / 216, (float)ts / 216);
	sprite.setOrigin((float)tileSize/4, (float)tileSize/4);
	if (isStart) sprite.setTextureRect(sf::IntRect(texture->getSize().x / 2, 0, texture->getSize().x / 2, texture->getSize().y));
	else sprite.setTextureRect(sf::IntRect(0, 0, texture->getSize().x / 2, texture->getSize().y));
	sprite.rotate(90 * (1 + (PIT / 2)));
	switch (PIT) {
	case(TL):sprite.setPosition(sprite.getPosition().x - interval * 3, sprite.getPosition().y); break;
	case(TR):sprite.setPosition(sprite.getPosition().x - interval, sprite.getPosition().y); break;
	case(RT):sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - interval * 3); break;
	case(RB):sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - interval); break;
	case(BR):sprite.setPosition(sprite.getPosition().x + interval * 3, sprite.getPosition().y); break;
	case(BL):sprite.setPosition(sprite.getPosition().x + interval, sprite.getPosition().y); break;
	case(LB):sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + interval * 3); break;
	case(LT):sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + interval); break;
	}
}

void CheckPoint::Draw(sf::RenderTarget& window){
	window.draw(sprite);
}

