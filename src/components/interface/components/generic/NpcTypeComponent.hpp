#pragma once

#include "other/NpcType.hpp"

struct NpcTypeComponent
{
    explicit NpcTypeComponent(NpcType npc_type) : npc_type(npc_type) {}
    NpcType npc_type;
};
