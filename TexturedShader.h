#pragma once
#include "Shader.h"


class TexturedShader : public Shader
{
public:
	TexturedShader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights) : Shader(projectionMatrix, cameraMatrix, lights)
	{
		vertexFile = "Resources/Shaders/simpleModel.vert";
		fragmentFile = "Resources/Shaders/simpleTextured.frag";
	}

	void CompileShader() override;
	void bindUniformLocations() override;

	void SetVertexVBOData() override;
};
