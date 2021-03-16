#include "CheckPoint.h"

CheckPoint::CheckPoint(int id, int ts, sf::Vector2f pos, posInTile pit, sf::Texture* tex, ConsoleManager* cm) : ID(id), tileSize(ts), position(pos), PIT(pit), texture(tex), console(cm) {
	isStart = ID == 0 ? true : false;

	float interval = (float)ts / 8;
	position.x = ts * pos.x;
	position.y = ts * pos.y;

	//sprite setup
	sprite.setPosition(position);
	sprite.setTexture(*texture);
	sprite.scale((float)ts / 216, (float)ts / 216);
	sprite.setOrigin((float)tileSize / 4, (float)tileSize / 4);
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

	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	if (spriteBounds.width < spriteBounds.height) {
		collisionRect.setPosition(spriteBounds.left + spriteBounds.width / 2, spriteBounds.top);
		collisionRect.setSize(sf::Vector2f(1, spriteBounds.height));
	}
	else {
		collisionRect.setPosition(spriteBounds.left, spriteBounds.top + spriteBounds.height / 2);
		collisionRect.setSize(sf::Vector2f(spriteBounds.width, 1));
	}
	collisionRect.setFillColor(sf::Color::Red);

	center = sf::Vector2f(collisionRect.getPosition().x + collisionRect.getSize().x / 2, collisionRect.getPosition().y + collisionRect.getSize().y / 2);
	c.setFillColor(sf::Color::Green);
	c.setRadius(5);
	c.setPosition(center);
	c.setOrigin(5, 5);
}

void CheckPoint::Draw(sf::RenderTarget& window) {
	window.draw(sprite);
	if (console->IsDisplayed()) {
		window.draw(collisionRect);
		window.draw(c);
	}
}
