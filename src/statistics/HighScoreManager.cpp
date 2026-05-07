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

    HighScoreEntry entry;

    while (file >> entry.playerName >> entry.score)
    {
        records.push_back(entry);
    }
}

void HighScoreManager::Save() const
{
    std::ofstream file(filepath);

    for (const HighScoreEntry& entry : records)
    {
        file << entry.playerName << ' ' << entry.score << '\n';
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

const std::vector<HighScoreEntry>& HighScoreManager::GetRecords() const
{
    return records;
}