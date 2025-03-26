#include "main-dude/MainDudeHelpers.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "EntityRegistry.hpp"
#include "Vector2D.hpp"

void main_dude_helpers::check_main_dude_proximity(const WhenDudeIsCloseCallback_t& callback,
                               const Vector2D& activation_distance,
                               const PositionComponent& position)
{
    auto& registry = EntityRegistry::instance().get_registry();

    Vector2D distance;

    const auto main_dudes = registry.view<MainDudeComponent, PositionComponent>();
    main_dudes.each([&](MainDudeComponent&, PositionComponent& dude_position)
    {
        distance.x = dude_position.x_center - position.x_center;
        distance.y = dude_position.y_center - position.y_center;

        if (distance.x < activation_distance.x && distance.y < activation_distance.y)
        {
            callback(dude_position, distance);
        }
    });
}
