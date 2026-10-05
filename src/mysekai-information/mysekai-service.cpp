#include "mysekai-information/mysekai-service.h"
#include "common/common-enums.h"

#include <algorithm>

std::unordered_set<int> MySekaiService::getMysekaiCanvasBonusCards()
{
    auto& userMysekaiCanvas = this->dataProvider.userData->userMysekaiCanvases;
    std::unordered_set<int> result = {};
    for (auto& it : userMysekaiCanvas)
        result.insert(it.cardId);
    return result;
}

std::vector<UserMysekaiFixtureGameCharacterPerformanceBonus> MySekaiService::getMysekaiFixtureBonuses()
{
    return this->dataProvider.userData->userMysekaiFixtureGameCharacterPerformanceBonuses;
}

std::vector<MysekaiGateBonus> MySekaiService::getMysekaiGateBonuses()
{
    auto& userMysekaiGates = this->dataProvider.userData->userMysekaiGates;
    auto& mysekaiGates = this->dataProvider.masterData->mysekaiGates;
    auto& mysekaiGateLevels = this->dataProvider.masterData->mysekaiGateLevels;
    std::vector<MysekaiGateBonus> result = {};
    for (auto& it : userMysekaiGates) {
        auto gate = std::find_if(mysekaiGates.begin(), mysekaiGates.end(), [&](const MysekaiGate& g) {
            return g.id == it.mysekaiGateId;
        });
        auto gateLevel = std::find_if(mysekaiGateLevels.begin(), mysekaiGateLevels.end(), [&](const MysekaiGateLevel& l) {
            return l.mysekaiGateId == it.mysekaiGateId && l.level == it.mysekaiGateLevel;
        });
        result.push_back(MysekaiGateBonus{
            it.mysekaiGateId,
            gate != mysekaiGates.end() ? gate->unit : Enums::Unit::none,
            it.mysekaiGateLevel,
            gateLevel != mysekaiGateLevels.end() ? gateLevel->powerBonusRate : 0.0
        });
    }
    return result;
}
