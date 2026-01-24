
// Implémentation de la classe Player (joueur)
#include "Player.h"


namespace SI3LN
{

	// Constructeur du joueur, initialise la position, la texture et les limites de déplacement
	Player::Player(float x, float y, SDL_Texture *texture, int screenWidth, int screenHeight)
		: Entity(x, y, PLAYER_PORTRAIT_SIZE, PLAYER_PORTRAIT_SIZE), texture(texture), screenWidth(screenWidth), screenHeight(screenHeight), speed(PLAYER_SPEED), lastShootTime(0), shootCooldown(0) // Pas de cooldown
	{
		// Définir les limites de déplacement du joueur
		minX = 0;
		maxX = screenWidth;
		minY = screenHeight / 2; // Le joueur ne peut pas monter au-dessus du milieu
		maxY = screenHeight - 20;
	}

	// Met à jour la position du joueur en fonction de la vélocité et des limites
	void Player::update(float deltaTime)
	{
		// Mise à jour de la position selon la vélocité
		position += velocity * deltaTime;

		// Empêche le joueur de sortir des limites de l'écran
		if (position.x - width / 2 < minX)
			position.x = minX + width / 2;
		if (position.x + width / 2 > maxX)
			position.x = maxX - width / 2;
		if (position.y - height / 2 < minY)
			position.y = minY + height / 2;
		if (position.y + height / 2 > maxY)
			position.y = maxY - height / 2;
	}

	// Affiche le joueur à l'écran
	void Player::render(SDL_Renderer *renderer)
	{
		if (!texture)
			return;

		SDL_Rect destRect = getRect();
		SDL_RenderCopy(renderer, texture, nullptr, &destRect);
	}

	// Applique un déplacement au joueur (gère la normalisation diagonale)
	void Player::move(float dx, float dy)
	{
		// Normalise le déplacement diagonal pour garder la même vitesse
		if (dx != 0.0f && dy != 0.0f)
		{
			dx *= 0.707f;
			dy *= 0.707f;
		}

		velocity.x = dx * speed;
		velocity.y = dy * speed;
	}

	// Gère les entrées clavier pour déplacer le joueur
	void Player::handleInput(const uint8_t *keyState)
	{
		float dx = 0.0f;
		float dy = 0.0f;

		// Mouvement horizontal
		if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
		{
			dx = -5.0f;
		}
		if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
		{
			dx = 5.0f;
		}

		// Mouvement vertical
		if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
		{
			dy = -5.0f;
		}
		if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
		{
			dy = 5.0f;
		}

		move(dx, dy);
	}

	// Vérifie si le joueur peut tirer (cooldown respecté)
	bool Player::canShoot() const
	{
		// Pas de cooldown : le joueur peut toujours tirer
		return true;
	}

	// Réinitialise le cooldown de tir après un tir
	void Player::resetShootCooldown()
	{
		// Pas de cooldown : ne rien faire
	}

} // namespace SI3LN
