#ifndef __MOVEMENT_SCRIPT_HPP__
#define __MOVEMENT_SCRIPT_HPP__

#include <Engine/Engine.hpp>
#include <Graphics/Graphics.hpp>
#include <Engine/Component/Script.hpp>
#include <Engine/Component/Transformation.hpp>
#include <GLFW/glfw3.h>

class MovementScript : public Script
{
private:

	float tSpeed = 1.0f;
	float rSpeed = 1.0f;
	float mouseSensivity = 4.0f;

	double lastXPos, lastYPos;
	bool firstMouse = true;

	bool rightClick = false;
	bool leftClick = false;

	//Movement
	int keyUp = GLFW_KEY_SPACE, keyDown = GLFW_KEY_LEFT_SHIFT;
	int keyFront = GLFW_KEY_W, keyBack = GLFW_KEY_S;
	int keyLeft = GLFW_KEY_A, keyRight = GLFW_KEY_D;

	//Rotations
	int keyRZneg = GLFW_KEY_E, keyRZpos = GLFW_KEY_Q;

	GameObject* lastHittedGO = nullptr;

public:
	MovementScript() {
	}

	virtual void loop(double deltaTime)
	{
		keyboard(deltaTime);
		mouse(deltaTime);

		this->attachment->GetTransform()->Update();
	}

	void keyboard(double deltaTime) {
		//Translation X
		if (glfwGetKey(global.global_window, keyUp) == GLFW_PRESS) {
			this->attachment->GetTransform()->Translate(glm::vec3(0, tSpeed * deltaTime, 0));
		}
		if (glfwGetKey(global.global_window, keyDown) == GLFW_PRESS) {
			this->attachment->GetTransform()->Translate(glm::vec3(0, -tSpeed * deltaTime, 0));
		}
		//Translation Y
		if (glfwGetKey(global.global_window, keyFront) == GLFW_PRESS) {
			this->attachment->GetTransform()->Translate(glm::vec3(0, 0, tSpeed * deltaTime));
		}
		if (glfwGetKey(global.global_window, keyBack) == GLFW_PRESS) {
			this->attachment->GetTransform()->Translate(glm::vec3(0, 0, -tSpeed * deltaTime));
		}
		//Translation Z
		if (glfwGetKey(global.global_window, keyRight) == GLFW_PRESS) {
			this->attachment->GetTransform()->Translate(glm::vec3(tSpeed * deltaTime, 0, 0));
		}
		if (glfwGetKey(global.global_window, keyLeft) == GLFW_PRESS) {
			this->attachment->GetTransform()->Translate(glm::vec3(-tSpeed * deltaTime, 0, 0));
		}

		//Rotation Z
		if (glfwGetKey(global.global_window, keyRZneg) == GLFW_PRESS) {
			this->attachment->GetTransform()->Rotate(glm::vec3(0, 0, -rSpeed * deltaTime));
		}
		if (glfwGetKey(global.global_window, keyRZpos) == GLFW_PRESS) {
			this->attachment->GetTransform()->Rotate(glm::vec3(0, 0, rSpeed * deltaTime));
		}
		//Enable camera rotation
		if (glfwGetMouseButton(global.global_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !rightClick) {
			// Hide the mouse
			glfwSetInputMode(global.global_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			this->lastXPos = global.mouseX;
			this->lastYPos = global.mouseY;
			rightClick = true;
		}
		else if (glfwGetMouseButton(global.global_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rightClick) {
			// Show the mouse
			glfwSetInputMode(global.global_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			rightClick = false;
		}

		//Enable left click raytracing
		if (glfwGetMouseButton(global.global_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !leftClick) {
			leftClick = true;
		}
		else if (glfwGetMouseButton(global.global_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && leftClick) {
			leftClick = false;
		}
	}

	void mouse(double deltaTime) {
		if (!firstMouse) {
			if (rightClick) {
				double dx = global.mouseX - lastXPos, dy = global.mouseY - lastYPos;
				//Rotation X
				if (dx != 0) {
					this->attachment->GetTransform()->Rotate(glm::vec3(0, rSpeed * dx * deltaTime * mouseSensivity, 0));
				}
				//Rotation Y
				if (dy != 0) {
					this->attachment->GetTransform()->Rotate(glm::vec3(rSpeed * dy * deltaTime * mouseSensivity, 0, 0));
				}
			}
			if (leftClick) {
				if (global.mouseX != this->lastXPos || global.mouseY != this->lastYPos) {
					if(lastHittedGO == nullptr){
						//Create a hit if hit something
						//Graphics * g = engine->GetGraphics();

						// The Idea here is to send a Ray Cast (or a Sphere Cast) to grab object or create drop on the water

						//If the Gameobject is a camera we can use the depth stencil
						Camera* camera = this->attachment->getFirstComponentByType<Camera>();
						GameObject* hitted = nullptr;
						if(camera != nullptr){
							//hitted = graphics.RaycastStencil(camera, engine->GetActiveSceneRoot()->GetChildsWithoutComponentRecursive());
						}
						else {
							// First we get the Mouse coordinate on the screen, here global.mouseX | Y
							glm::vec4 coord = glm::vec4(global.mouseX / (double)global.screen_width, global.mouseY / (double)global.screen_height, 0, 1);
							// Then we cast it to the 3D world coordinate
							coord = coord * this->attachment->GetTransform()->getMatrix();
							// Then we sent the ray that start with the 3D world coordinate and have the Object front vector as direction.
							//hitted = engine->GetPhysicsEngine()->Raycast(engine->GetActiveSceneRoot()->GetChildsWithoutComponentRecursive(), glm::vec3(coord.x, coord.y, coord.z), this->attachment->GetTransform()->getFrontVector());
						}
					} else {
						//Move the element if he have a grab function

					}
				}
			} else {
				lastHittedGO = nullptr;
			}
		}
		else {
			firstMouse = false;
		}
		this->lastXPos = global.mouseX;
		this->lastYPos = global.mouseY;
	}

};

#endif