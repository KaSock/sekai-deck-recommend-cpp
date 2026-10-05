#include "area-item-information/area-item-service.h"

#include <algorithm>

std::vector<AreaItemLevel> AreaItemService::getAreaItemLevels()
{
    auto& userAreas = this->dataProvider.userData->userAreas;
    std::vector<AreaItemLevel> areaItemLevels{};
    for (const auto& userArea : userAreas) {
        for (const auto& areaItem : userArea.areaItems) {
            auto rows = this->getAreaItemLevel(areaItem.areaItemId, areaItem.level);
            areaItemLevels.insert(areaItemLevels.end(), rows.begin(), rows.end());
        }
    }
    return areaItemLevels;
}

std::vector<AreaItemLevel> AreaItemService::getAreaItemLevel(int areaItemId, int level)
{
    auto& levels = this->dataProvider.masterData->areaItemLevels;
    int maxLevel = 0;
    for (const auto& it : levels)
        if (it.areaItemId == areaItemId)
            maxLevel = std::max(maxLevel, it.level);
    const int effectiveLevel = std::min(level, maxLevel);
    std::vector<AreaItemLevel> result{};
    for (const auto& it : levels)
        if (it.areaItemId == areaItemId && it.level == effectiveLevel)
            result.push_back(it);
    if (result.empty())
        throw ElementNoFoundError("Area item level not found for areaItemId=" + std::to_string(areaItemId) + " level=" + std::to_string(level));
    return result;
}

std::vector<AreaItemLevel> AreaItemService::getAreaItemNextLevel(const AreaItem &areaItem, std::optional<int> currentLevel)
{
    int maxLevel = 0;
    for (const auto& it : this->dataProvider.masterData->areaItemLevels)
        if (it.areaItemId == areaItem.id)
            maxLevel = std::max(maxLevel, it.level);
    const int level = currentLevel.has_value() ? std::min(currentLevel.value() + 1, maxLevel) : 1;
    return this->getAreaItemLevel(areaItem.id, level);
}

int AreaItemService::getShopItemId(int areaItemId, int level)
{
    if (areaItemId == 56)
        return 2100 + level;
    if (level <= 10)
        return 1000 + (areaItemId - 1) * 10 + level;
    if (level <= 15)
        return 1550 + (areaItemId - 1) * 5 + (level - 10);
    return 1825 + (areaItemId - 1) * 5 + (level - 15);
}

ShopItem AreaItemService::getShopItem(int areaItemId, int level)
{
    auto& shopItems = this->dataProvider.masterData->shopItems;
    const int id = getShopItemId(areaItemId, level);
    return findOrThrow(shopItems, [&](const ShopItem& it) {
        return it.id == id;
    }, [&]() {
        return "Shop item not found for areaItemId=" + std::to_string(areaItemId) +
            " level=" + std::to_string(level) + " shopItemId=" + std::to_string(id);
    } );
}
