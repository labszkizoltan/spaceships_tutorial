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
	sf::Music m_CurrentMusic;
	std::vector<std::string> m_SoundTrackPaths;
	std::vector<sf::Sound> m_Sounds;

};








