#include "components/CameraComponent.hpp"

#include "main-dude/MainDude.hpp"
#include "Camera.hpp"

#include <cmath>
#include <logger/log.h>

void CameraComponent::update(const MainDude& dude, uint32_t delta_time_ms)
{
    auto x = dude._physics.get_x_position();
    auto y = dude._physics.get_y_position();

    auto& camera = Camera::instance();
    camera.adjust_to_bounding_box(x, y);
}
