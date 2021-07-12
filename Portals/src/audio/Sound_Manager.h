#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class SoundManager
{
public:

	SoundManager() = default;
	~SoundManager() = default;

	void LoadMusic(const std::string& filename);
	void PlayMusic(int idx);

	void LoadSound(const std::string& filename);
	void PlaySound(int idx);


public:
	std::vector<sf::Music> m_Musics;
	std::vector<sf::Sound> m_Sounds;

};








