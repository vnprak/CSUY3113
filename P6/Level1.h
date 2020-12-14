#include "Scene.h"

class Level1 : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
	void setLives(int lives) override;
	int getLives() override;
	void setProjectiles(int count) override;
	int getProjectiles() override;
	bool isGameLost() override;
	bool isGameWon() override;
	int projectileMax() override;
};