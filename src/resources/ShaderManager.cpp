#include "ShaderManager.h"

void ShaderManager::Load(Assets::ShaderID id, const std::filesystem::path& filepath, sf::Shader::Type type)
{
	sf::Shader shader;

	if (!shader.loadFromFile(filepath, type))
	{
		throw std::runtime_error("Failed to load shader: " + filepath.string());
	}

	const auto [_, inserted] = shaders.emplace(id, std::move(shader));
	if (!inserted)
	{
		throw std::runtime_error("Shader already loaded.");
	}
}

sf::Shader& ShaderManager::Get(Assets::ShaderID id)
{
	if (!shaders.contains(id))
	{
		throw std::runtime_error("Requested shader was not loaded.");
	}

	return shaders.at(id);
}

const sf::Shader& ShaderManager::Get(Assets::ShaderID id) const
{
	if (!shaders.contains(id))
	{
		throw std::runtime_error("Requested shader was not loaded.");
	}

	return shaders.at(id);
}