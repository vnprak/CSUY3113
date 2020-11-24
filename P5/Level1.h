#include "Scene.h"

class Level1 : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
	void setLives(int lives) override;
	int getLives() override;
};