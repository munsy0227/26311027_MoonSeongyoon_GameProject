#include "StoryProgress.h"

void StoryProgress::Reset()
{
    m_block = StoryBlock::Prologue;
    m_line = 0;
    m_choosing = m_complete = false;
    m_hasKey = m_hasPhoto = m_hasRecord = false;
    m_appliedEvents = 0;
}

const StorySection& StoryProgress::Section() const
{
    return GetStorySection(m_block);
}

const StoryLine& StoryProgress::Line() const
{
    return Section().lines[m_line];
}

bool StoryProgress::Matches(StoryCondition condition) const
{
    switch (condition)
    {
    case StoryCondition::HasPhoto: return m_hasPhoto;
    case StoryCondition::NoPhoto: return !m_hasPhoto;
    case StoryCondition::HasRecord: return m_hasRecord;
    case StoryCondition::NoRecord: return !m_hasRecord;
    default: return true;
    }
}

void StoryProgress::Apply(StoryEvent event, Player& player)
{
    if (event == StoryEvent::None)
        return;
    const auto mask = std::uint32_t(1) << static_cast<unsigned>(event);
    if (m_appliedEvents & mask)
        return;
    m_appliedEvents |= mask;
    switch (event)
    {
    case StoryEvent::Key: m_hasKey = true; break;
    case StoryEvent::Photo: m_hasPhoto = true; player.AddScore(20); break;
    case StoryEvent::Record: m_hasRecord = true; player.AddScore(20); break;
    case StoryEvent::LockerOpened:
    case StoryEvent::SafeFloor: player.AddScore(20); break;
    case StoryEvent::FloorDamage: player.TakeDamage(20); break;
    case StoryEvent::FirstEmpathy:
    case StoryEvent::SecondEmpathy: player.AddScore(10); break;
    default: break;
    }
}

void StoryProgress::Enter(StoryBlock block, Player& player)
{
    m_block = block;
    m_line = 0;
    m_choosing = false;
    FindVisibleLine(player);
}

void StoryProgress::FindVisibleLine(Player& player)
{
    // Blocks form an acyclic story graph. Conditional lines never change state.
    while (m_line < Section().lines.size() && !Matches(Line().condition))
        ++m_line;
    if (m_line < Section().lines.size())
    {
        Apply(Line().event, player);
        return;
    }
    // Keep the last displayed line valid while presenting its choices/summary.
    m_line = Section().lines.size() - 1;
    if (Section().choice != StoryChoice::None)
        m_choosing = true;
    else if (Section().next == StoryBlock::Count)
        m_complete = true;
    else
        Enter(Section().next, player);
}

bool StoryProgress::Advance(Player& player)
{
    if (m_choosing || m_complete)
        return false;
    ++m_line;
    FindVisibleLine(player);
    return true;
}

bool StoryProgress::Choose(int option, Player& player)
{
    if (!m_choosing || option < 0 || option > 1)
        return false;
    if (Section().choice == StoryChoice::Locker && option == 0 && !m_hasKey)
        return false;
    if (option == 0)
    {
        if (Section().choice == StoryChoice::FirstTrust)
            Apply(StoryEvent::FirstEmpathy, player);
        else if (Section().choice == StoryChoice::SecondTrust)
            Apply(StoryEvent::SecondEmpathy, player);
    }
    Enter(Section().branches[option], player);
    return true;
}
