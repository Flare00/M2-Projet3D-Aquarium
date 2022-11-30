#ifndef __WATER_PHYSICS_HPP__
#define __WATER_PHYSICS_HPP__

#include <glm/glm.hpp>
#include <GL/glut.h>
#include <vector>
#include <Physics/IPhysics.hpp>
#include <Graphics/Framebuffer.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Tools/ModelGenerator.hpp>

class WaterPhysics : public IPhysics  {
protected :
	struct Drop{
		glm::vec2 pos;
		float radius;
    	float strength;
		Drop(glm::vec2 pos, float radius, float strength){
			this->pos = pos;
			this->radius = radius;
			this->strength = strength;
		}
	};
	Shader * physicShader;
	Framebuffer framebuffer;
	std::vector<Drop> drops;
	int resolution;
	float invRes;
	
	std::vector<glm::vec3> quad;
public :
	WaterPhysics(Shader * physicShader, int resolution = 1024) {
		this->resolution = resolution;
		this->invRes = 1.0f / (float) resolution;
		this->quad = ModelGenerator::QuadPoints(resolution, resolution);
	}

	void PostAttachment() override {
		framebuffer.Generate();
		this->texture = this->framebuffer.GetTexColor();
	}

	void AddDrop(glm::vec2 pos, float radius, float strength){
		this->drops.push_back(Drop(pos, radius, strength));
	}

	void Compute(double delta) override {
		//Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer.GetFramebuffer());
		glUseProgram(physicShader->GetProgram());
		//Set Data
		glUniform1f(glGetUniformLocation(physicShader->GetProgram(), "deltaTime"), delta);
		glUniform2f(glGetUniformLocation(physicShader->GetProgram(), "deltaMove"), invRes, invRes);

		// Set Drop
		if(drops.size() > 0){
			glUniform2f(glGetUniformLocation(physicShader->GetProgram(), "drop.center"), drops[0].pos.x, drops[0].pos.y);
			glUniform1f(glGetUniformLocation(physicShader->GetProgram(), "drop.radius"), drops[0].radius);
			glUniform1f(glGetUniformLocation(physicShader->GetProgram(), "drop.strength"), drops[0].strength);
			glUniform1d(glGetUniformLocation(physicShader->GetProgram(), "drop.isDrop"), 1);
		} else {
			glUniform1d(glGetUniformLocation(physicShader->GetProgram(), "drop.isDrop"), 0);
		}

		//Bind Texture

		glUniform1d(glGetUniformLocation(physicShader->GetProgram(), "texture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->framebuffer.GetTexColor());

		//Draw Mesh
		glVertexPointer( 3, GL_FLOAT, 0, &quad[0]);
		glDrawArrays(GL_POINTS, 0, quad.size());

		glFlush();

		//Release
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint GetHeightmap(){
		return this->framebuffer.GetTexColor();
	}
};

#endif
