#pragma once

#include "EntityRegistry.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "entt/entt.hpp"
#include "patterns/Subject.hpp"

#include <string>

struct ShoppingTransactionEvent
{
    enum class Type
    {
        TRANSACTION_ABORTED, // TODO
        TRANSACTION_POSSIBLE,
        TRANSACTION_SUCCESS,
        TRANSACTION_FAILED
    };

    std::string message;
};

namespace prefabs
{
    class WalletScript final : public ScriptBase, public Subject<ShoppingTransactionEvent>
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override {}
    };

    struct Wallet
    {
        static entt::entity create();
    };
}