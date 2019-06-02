#pragma once
#include "Shader.h"

class ColorShader : public Shader
{
public:
	ColorShader(QMatrix4x4* projectionMatrix, QMatrix4x4* cameraMatrix, std::vector<Light*>* lights) : Shader(projectionMatrix, cameraMatrix, lights)
	{
		vertexFile = "Resources/Shaders/simpleModel.vert";
		fragmentFile = "Resources/Shaders/simpleColored.frag";
	}

	void CompileShader() override;
	void LoadColor(int r, int g, int b) const;
	void LoadColor(QVector3D color) const;
	void bindUniformLocations() override;

	void SetVertexVBOData() override;
private:
	int colorLoc = 0;
};
