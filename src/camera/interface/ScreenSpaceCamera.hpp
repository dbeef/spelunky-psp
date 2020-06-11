#pragma once

#include <memory>
// Note: For both cameras coordinate system goes as following:
//
//   ------ +x
//   |
//   |
//   +y
//

class Viewport;

class ScreenSpaceCamera
{
public:
    
    ScreenSpaceCamera(std::shared_ptr<Viewport>);

    void update_gl_modelview_matrix() const;
    void update_gl_projection_matrix() const;

    void calculate_coefficients();

private:
    
    std::shared_ptr<Viewport> _viewport;
    
    float _projection_coefficient;
};
