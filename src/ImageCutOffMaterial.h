#pragma once

#include "ImageMaterial.h"

class ImageCutOffMaterial : public ImageMaterial
{
public:
    ImageCutOffMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~ImageCutOffMaterial();

    virtual void SetUniforms(MeshRendererComponent* renderer);

    void SetCutOffTexture(Texture2D* value) { cutOffTexture = value; }
    void SetCutOff(float value) { cutOff = value; }

private:
    Texture2D* cutOffTexture;
    float cutOff;
};
