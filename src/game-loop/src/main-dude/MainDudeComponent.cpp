#include "prefabs/main-dude/MainDude.hpp"

#include "NeighbouringTiles.hpp"
#include "Level.hpp"
#include "Collisions.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/ClimbingComponent.hpp"
#include "MapTileType.hpp"
#include "CameraType.hpp"
#include "Input.hpp"
#include "other/Inventory.hpp"
#include "main-dude/states/MainDudeRunningState.hpp"
#include "EntityRegistry.hpp"
#include "Vector2D.hpp"

#include <cmath>

namespace
{
    // Main dude sprites are of 16x16 pixels, but the main dude fits
    // into the portion with 2px margin on the left and right side of a sprite,
    // thus making collision box smaller:
    const float MAIN_DUDE_PHYSICAL_WIDTH = 1.0f - 2 * (2.0f / 16.0f);
    // Main dude height is 16 pixels, but making its collision box smaller by a quarter of pixel top and down,
    // so it would be smoother to jump through slits between the tiles:
    const float MAIN_DUDE_PHYSICAL_HEIGHT = 1.0f - 2 * (0.25f / 16.0f);
    // No offset is applied on main-dude's quad that would account for this collision box change - FIXME.

    const float MAIN_DUDE_QUAD_WIDTH = 1.0f;
    const float MAIN_DUDE_QUAD_HEIGHT = 1.0f;
}

MainDudeComponent::MainDudeComponent(entt::entity owner) : _owner(owner)
{
    auto& registry = EntityRegistry::instance().get_registry();

    _states.current = &_states.standing;
    _states.current->enter(*this);

    _climbing_observer = std::make_shared<MainDudeClimbingObserver>(owner);
    _fall_observer = std::make_shared<MainDudeFallObserver>(owner);
    _death_observer = std::make_shared<MainDudeDeathObserver>(owner);
    _npc_damage_observer = std::make_shared<MainDudeNpcDamageObserver>(owner);
    _explosion_observer = std::make_shared<MainDudeExplosionDamageObserver>(owner);
    _projectile_observer = std::make_shared<MainDudeProjectileDamageObserver>(owner);
    _spikes_observer = std::make_shared<MainDudeSpikesDamageObserver>(owner);
}

MainDudeComponent::MainDudeComponent(const MainDudeComponent& other) : _owner(other._owner)
{
    _states.current = &_states.standing;
    _climbing_observer = other._climbing_observer;
    _fall_observer = other._fall_observer;
    _death_observer = other._death_observer;
    _npc_damage_observer = other._npc_damage_observer;
    _explosion_observer = other._explosion_observer;
    _projectile_observer = other._projectile_observer;
    _spikes_observer = other._spikes_observer;
}

void MainDudeComponent::update(uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();

    // Update generic properties:

    auto& physics = registry.get<PhysicsComponent>(_owner);
    auto& quad = registry.get<QuadComponent>(_owner);

    // Update current state:

    MainDudeBaseState* new_state;

    assert(_states.current);
    new_state = _states.current->update(*this, delta_time_ms);
    enter_if_different(new_state);
}

MapTile* MainDudeComponent::is_overlaping_tile(MapTileType type, PhysicsComponent& physics, PositionComponent& position) const
{
    MapTile* neighbours[9] = {nullptr};
    Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);

    for (const auto neighbour : neighbours)
    {
        if (neighbour == nullptr || neighbour->map_tile_type != type)
        {
            continue;
        }

        if (collisions::overlaps(neighbour, position.x_center, position.y_center, physics.get_width(), physics.get_height()))
        {
            return neighbour;
        }
    }

    return nullptr;
}

void MainDudeComponent::enter_standing_state()
{
    enter_if_different(&_states.standing);
}

void MainDudeComponent::enter_stunned_state()
{
    enter_if_different(&_states.stunned);
}

void MainDudeComponent::enter_climbing_state()
{
    enter_if_different(&_states.climbing);
}

void MainDudeComponent::enter_throwing_state()
{
    enter_if_different(&_states.throwing);
}

void MainDudeComponent::enter_dead_state()
{
    enter_if_different(&_states.dead);
}

void MainDudeComponent::enter_level_summary_state()
{
    enter_if_different(&_states.level_summary);
}

bool MainDudeComponent::hang_off_cliff_right(PhysicsComponent& physics, PositionComponent& position)
{

    if (physics.is_right_collision())
    {
        MapTile *neighbours[9] = {nullptr};

        Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);

        auto* right_tile = neighbours[static_cast<int>(NeighbouringTiles::RIGHT_MIDDLE)];
        auto* right_upper_tile = neighbours[static_cast<int>(NeighbouringTiles::RIGHT_UP)];

        if (right_tile && right_tile->collidable && (!right_upper_tile || !right_upper_tile->collidable) &&
            // Main dude's center must be in margin of a quarter of a tile from its beginning:
            (position.y_center >= right_tile->y + 0.25f))
        {
            position.x_center = right_tile->x - (physics.get_width() / 2.0f);
            position.y_center = right_tile->y + 0.5f;
            return true;
        }
    }

    return false;
}

bool MainDudeComponent::hang_off_cliff_right_gloves(ItemCarrierComponent& items, PhysicsComponent& physics, PositionComponent& position)
{
    const auto& modifiers = items.get_modifiers();

    if (modifiers.can_climb_vertical_surfaces)
    {
        MapTile *neighbours[9] = {nullptr};

        Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);

        auto* right_tile = neighbours[static_cast<int>(NeighbouringTiles::RIGHT_MIDDLE)];
        auto* right_upper_tile = neighbours[static_cast<int>(NeighbouringTiles::RIGHT_UP)];

        if (right_tile->collidable && right_upper_tile->collidable)
        {
            position.x_center = right_tile->x - (physics.get_width() / 2.0f);
            position.y_center = right_tile->y + 0.5f;
            return true;
        }
    }

    return false;
}

bool MainDudeComponent::hang_off_cliff_left(PhysicsComponent& physics, PositionComponent& position)
{
    if (physics.is_left_collision())
    {
        MapTile *neighbours[9] = {nullptr};

        Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);

        auto* left_tile = neighbours[static_cast<int>(NeighbouringTiles::LEFT_MIDDLE)];
        auto* left_upper_tile = neighbours[static_cast<int>(NeighbouringTiles::LEFT_UP)];

        if (left_tile && left_tile->collidable && (!left_upper_tile || !left_upper_tile->collidable) &&
            // Main dude's center must be in margin of a quarter of a tile from its beginning:
            (position.y_center >= left_tile->y + 0.25f))
        {
            position.x_center = left_tile->x + MapTile::PHYSICAL_WIDTH + (physics.get_width() / 2.0f);
            position.y_center = left_tile->y + (MapTile::PHYSICAL_HEIGHT / 2.0f);
            return true;
        }
    }

    return false;
}

bool MainDudeComponent::hang_off_cliff_left_gloves(ItemCarrierComponent& items, PhysicsComponent& physics, PositionComponent& position)
{
    const auto& modifiers = items.get_modifiers();

    if (modifiers.can_climb_vertical_surfaces)
    {
        MapTile *neighbours[9] = {nullptr};

        Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);

        auto* left_tile = neighbours[static_cast<int>(NeighbouringTiles::LEFT_MIDDLE)];
        auto* left_upper_tile = neighbours[static_cast<int>(NeighbouringTiles::LEFT_UP)];

        if (left_tile->collidable && left_upper_tile->collidable)
        {
            position.x_center = left_tile->x + MapTile::PHYSICAL_WIDTH + (physics.get_width() / 2.0f);
            position.y_center = left_tile->y + (MapTile::PHYSICAL_HEIGHT / 2.0f);

            return true;
        }
    }

    return false;
}

void MainDudeComponent::enter_if_different(MainDudeBaseState* new_state)
{
    if (new_state != _states.current)
    {
        _states.current->exit(*this);
        _states.current = new_state;
        _states.current->enter(*this);
    }
}
