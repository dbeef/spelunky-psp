#include "main-dude/states/MainDudeStanding.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

#include <cstring>
#include <main-dude/MainDude.hpp>

MainDudeStanding::MainDudeStanding()
{

}

void MainDudeStanding::enter(MainDude& main_dude)
{

}

MainDudeBaseState *MainDudeStanding::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    Input& input = Input::instance();

    // TODO: Input polling timer
    if (input.square())
    {
        main_dude._physics_component.add_velocity(-0.025f, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics_component.add_velocity(+0.025f, 0.0f);
    }
    if (input.triangle())
    {
        if (main_dude._physics_component.is_bottom_collision())
        {
            main_dude._physics_component.add_velocity(0.0f, -0.18f);
        }
    }

    main_dude._physics_component.update(main_dude, delta_time_ms);
//    main_dude._animation_update_delta_ms += delta_time_ms;

    const auto velocity_x = main_dude._physics_component.get_x_velocity();
    if (velocity_x != 0.0f)
    {
        return &main_dude._states.running;
    }

    const bool vflip = !main_dude._facing_left;
    main_dude._current_frame = MainDudeSpritesheetFrames::STAND_LEFT;

    // Update render entity:
    // TODO: Util for only updating position

    float offset_x = main_dude._x - main_dude._physics_component.get_width() / 2;
    float offset_y = main_dude._y - main_dude._physics_component.get_height() / 2;

    // TODO: Dirty flag

    const auto texture_region = TextureBank::instance().get_region(TextureType::MAIN_DUDE, static_cast<std::size_t>(main_dude._current_frame));
    const auto new_mesh = texture_region.get_quad_mesh(offset_x, offset_y, vflip, false);
    std::memcpy(main_dude._mesh.data(), new_mesh.data(), new_mesh.size() * sizeof(Vertex));

    return this;
}
