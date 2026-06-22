#pragma once
#include "../Enums.h"
#include <vector>

struct StolenItemRecord
{
	ItemType type = ItemType::None;
	int count = 0;
};

class ItemInventory
{
public:
	void Clear();
	bool AddItem(ItemType type, int count = 1);
	bool RemoveItem(ItemType type, int count = 1);
	int GetCount(ItemType type) const;
	int GetSlotCount() const { return static_cast<int>(slots.size()); }
	ItemType GetSlotType(int index) const;
	int GetSlotItemCount(int index) const;
	int GetTotalItemCount() const;
	void CopyFrom(const ItemInventory& other);

	std::vector<StolenItemRecord> StealRandom(int minCount, int maxCount);
	void RestoreStolen(const std::vector<StolenItemRecord>& stolen);

private:
	struct Slot
	{
		ItemType type = ItemType::None;
		int count = 0;
	};

	int FindSlot(ItemType type) const;
	std::vector<Slot> slots;
};
