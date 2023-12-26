#pragma once

#include "EntityRegistry.hpp"
#include "CameraType.hpp"
#include "ScreenSpaceCamera.hpp"
#include "ModelViewCamera.hpp"

#include "System.hpp"

class RenderingSystem final : public System
{
public:

    explicit RenderingSystem(const std::shared_ptr<Viewport>& viewport);

    void update(std::uint32_t delta_time_ms) override;
    static void sort();

    ModelViewCamera& get_model_view_camera() { return _cameras.model_view; }
    ScreenSpaceCamera& get_screen_space_camera() { return _cameras.screen_space; }

private:

    void update_opengl(std::uint32_t delta_time_ms);
    void update_imgui();
    void use_camera(CameraType camera_type);
    void use_model_view_camera();
    void use_screen_space_camera();
    void update_blinking(std::uint32_t delta_time_ms) const;

    struct
    {
        CameraType current = CameraType::NONE;
        ModelViewCamera model_view;
        ScreenSpaceCamera screen_space;
    } _cameras;
};
