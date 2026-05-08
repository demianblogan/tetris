#pragma once

#include <vector>
#include <filesystem>
#include "HighScoreEntry.h"

class HighScoreManager
{
private:
	std::vector<HighScoreEntry> records;
	std::filesystem::path filepath;

public:
	static constexpr std::size_t MAX_RECORDS = 5;

	HighScoreManager(const std::filesystem::path& filepath);

	void Load();
	void Save() const;
	void AddRecord(const HighScoreEntry& entry);
	void Clear();

	[[nodiscard]] const std::vector<HighScoreEntry>& GetRecords() const;
};