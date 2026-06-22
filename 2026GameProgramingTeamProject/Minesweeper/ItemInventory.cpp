#include "ItemInventory.h"

void ItemInventory::Clear()
{
	slots.clear();
}

int ItemInventory::FindSlot(ItemType type) const
{
	for (int i = 0; i < static_cast<int>(slots.size()); ++i)
	{
		if (slots[static_cast<size_t>(i)].type == type)
			return i;
	}
	return -1;
}

bool ItemInventory::AddItem(ItemType type, int count)
{
	if (type == ItemType::None || count <= 0)
		return false;

	int idx = FindSlot(type);
	if (idx >= 0)
	{
		slots[static_cast<size_t>(idx)].count += count;
		return true;
	}
	if (static_cast<int>(slots.size()) >= MAX_INVENTORY)
		return false;

	slots.push_back(Slot{ type, count });
	return true;
}

bool ItemInventory::RemoveItem(ItemType type, int count)
{
	int idx = FindSlot(type);
	if (idx < 0)
		return false;

	Slot& slot = slots[static_cast<size_t>(idx)];
	if (slot.count < count)
		return false;

	slot.count -= count;
	if (slot.count <= 0)
		slots.erase(slots.begin() + idx);
	return true;
}

int ItemInventory::GetCount(ItemType type) const
{
	int idx = FindSlot(type);
	return idx < 0 ? 0 : slots[static_cast<size_t>(idx)].count;
}

ItemType ItemInventory::GetSlotType(int index) const
{
	if (index < 0 || index >= static_cast<int>(slots.size()))
		return ItemType::None;
	return slots[static_cast<size_t>(index)].type;
}

int ItemInventory::GetTotalItemCount() const
{
	int total = 0;
	for (int i = 0; i < static_cast<int>(slots.size()); ++i)
		total += slots[static_cast<size_t>(i)].count;
	return total;
}

void ItemInventory::CopyFrom(const ItemInventory& other)
{
	Clear();
	for (int i = 0; i < other.GetSlotCount(); ++i)
		AddItem(other.GetSlotType(i), other.GetSlotItemCount(i));
}

int ItemInventory::GetSlotItemCount(int index) const
{
	if (index < 0 || index >= static_cast<int>(slots.size()))
		return 0;
	return slots[static_cast<size_t>(index)].count;
}

std::vector<StolenItemRecord> ItemInventory::StealRandom(int minCount, int maxCount)
{
	std::vector<StolenItemRecord> stolen;
	if (slots.empty())
		return stolen;

	int stealTotal = minCount;
	if (maxCount > minCount)
		stealTotal += rand() % (maxCount - minCount + 1);
	if (stealTotal > static_cast<int>(slots.size()))
		stealTotal = static_cast<int>(slots.size());

	for (int i = 0; i < stealTotal && !slots.empty(); ++i)
	{
		int idx = rand() % static_cast<int>(slots.size());
		Slot slot = slots[static_cast<size_t>(idx)];
		int stealAmount = 1;
		if (slot.count > 1 && rand() % 2 == 0)
			stealAmount = slot.count < 2 ? slot.count : 2;

		stolen.push_back(StolenItemRecord{ slot.type, stealAmount });
		RemoveItem(slot.type, stealAmount);
	}
	return stolen;
}

void ItemInventory::RestoreStolen(const std::vector<StolenItemRecord>& stolen)
{
	for (size_t i = 0; i < stolen.size(); ++i)
		AddItem(stolen[i].type, stolen[i].count);
}
