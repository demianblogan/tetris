#include "HighScoreManager.h"

#include <algorithm>
#include <fstream>

HighScoreManager::HighScoreManager(const std::filesystem::path& filepath)
	: filepath(filepath)
{
	// No code
}

void HighScoreManager::Load()
{
	records.clear();

	std::ifstream file(filepath);
	if (!file.is_open())
	{
		return;
	}

	while (true)
	{
		HighScoreEntry entry;
		file >> entry.score;

		if (!file)
		{
			break;
		}

		file.ignore();

		std::string utf8Name;
		std::getline(file, utf8Name);
		entry.playerName = sf::String::fromUtf8(utf8Name.begin(), utf8Name.end());

		records.push_back(entry);
	}
}

void HighScoreManager::Save() const
{
	std::ofstream file(filepath);

	for (const HighScoreEntry& entry : records)
	{
		file << entry.score << '\n';
		file << entry.playerName.toUtf8().c_str() << '\n';
	}
}

void HighScoreManager::AddRecord(const HighScoreEntry& entry)
{
	records.push_back(entry);

	std::ranges::sort(records,
		[](const HighScoreEntry& a, const HighScoreEntry& b)
		{
			return a.score > b.score;
		}
	);

	if (records.size() > MAX_RECORDS)
	{
		records.resize(MAX_RECORDS);
	}
}

void HighScoreManager::Clear()
{
	records.clear();
}

bool HighScoreManager::IsHighScore(int score) const
{
	if (score <= 0)
	{
		return false;
	}

	if (records.size() < MAX_RECORDS)
	{
		return true;
	}

	return score > records.back().score;
}

const std::vector<HighScoreEntry>& HighScoreManager::GetRecords() const
{
	return records;
}