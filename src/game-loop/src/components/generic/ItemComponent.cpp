#include "components/generic/ItemComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

#include <cassert>

ItemCarrierComponent& ItemComponent::get_item_carrier() const
{
    assert(is_carried());
    auto& registry = EntityRegistry::instance().get_registry();
    return registry.get<ItemCarrierComponent>(_carrier);
}
