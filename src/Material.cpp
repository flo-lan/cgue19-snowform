#include "Material.h"
#include "ShaderProgram.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"

Material::Material(std::string const& _name, ShaderProgram* _shaderProgram) :
    name(_name),
    shaderProgram(_shaderProgram)
{
    fprintf(stdout, "Created material '%s'!\n", name.c_str());
}

Material::~Material()
{
    fprintf(stdout, "Deleted material '%s'!\n", name.c_str());
}

void Material::Use()
{
    if (shaderProgram)
    {
        shaderProgram->Use();
    }
}
