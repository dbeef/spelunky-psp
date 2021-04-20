#include "Cube.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

void Cube::write()
{
    _base_quad.write();

    int num_sides = 0;

    {
        Quad upper_side = _base_quad;
        upper_side.set_translation(0, 0, 0);
        upper_side.write();

        glm::mat4x4 translation_matrix = glm::translate(glm::identity<glm::mat4x4>(),
                                                      glm::vec3{_base_quad.get_translation_x(),
                                                                   _base_quad.get_translation_y() + 0.6f,
                                                                   _base_quad.get_translation_z()});

        for (int i = 0; i < Quad::get_vertices_count(); i++)
        {
            Vertex v_original = *(upper_side.get_vertices_base() + i);
            glm::vec4 v_rotated = {v_original.x, v_original.y, v_original.z, 1.0f};

            glm::mat4x4 rotation_matrix = glm::rotate<float>(glm::radians<float>(90),
                                                             glm::vec3(1.0f, 0.0f, 0.0f));

            v_rotated = translation_matrix * rotation_matrix *  v_rotated;

            Vertex v_out(v_rotated.x, v_rotated.y, v_rotated.z, v_original.u, v_original.v);
            _vertices.push_back(v_out);
        }

        for (int i = 0; i < Quad::get_indices_count(); i++)
        {
            _indices.push_back(*(upper_side.get_indices() + i) + (num_sides * 4));
        }

        num_sides++;
    }
//
//    {
//        Quad bottom_side = _base_quad;
//        bottom_side.write(num_sides * 4);
//
//        glm::mat4x4 translation_matrix = glm::translate(glm::identity<glm::mat4x4>(),
//                                                      glm::vec3{_base_quad.get_translation_x(),
//                                                                   _base_quad.get_translation_y() - 1.0f,
//                                                                   _base_quad.get_translation_z()});
//
//        for (int i = 0; i < Quad::get_vertices_count(); i++)
//        {
//            Vertex v_original = *(bottom_side.get_vertices_base() + i);
//            glm::vec4 v_rotated = {v_original.x, v_original.y, v_original.z, 1.0f};
//
//            glm::mat4x4 rotation_matrix = glm::rotate<float>(glm::radians<float>(90),
//                                                             glm::vec3(1.0f, 0.0f, 0.0f));
//
//            v_rotated = translation_matrix * rotation_matrix *  v_rotated;
//
//            Vertex v_out(v_rotated.x, v_rotated.y, v_rotated.z, v_original.u, v_original.v);
//            _vertices.push_back(v_out);
//        }
//
//        for (int i = 0; i < Quad::get_indices_count(); i++) {
//            _indices.push_back(*(bottom_side.get_indices() + i));
//        }
//    }
    
    {
        Quad front_side = _base_quad;
        front_side.write();

        glm::mat4x4 translation_matrix = glm::translate(glm::identity<glm::mat4x4>(),
                                                      glm::vec3{_base_quad.get_translation_x(),
                                                                   _base_quad.get_translation_y(),
                                                                   _base_quad.get_translation_z() - 1});

        for (int i = 0; i < Quad::get_vertices_count(); i++)
        {
            Vertex v_original = *(front_side.get_vertices_base() + i);
            glm::vec4 v_rotated = {v_original.x, v_original.y, v_original.z, 1.0f};

            glm::mat4x4 rotation_matrix = glm::rotate<float>(glm::radians<float>(0),
                                                             glm::vec3(0.0f, 0.0f, 0.0f));

            v_rotated = translation_matrix *  v_rotated;

            Vertex v_out(v_rotated.x, v_rotated.y, v_rotated.z, v_original.u, v_original.v);
            _vertices.push_back(v_out);
        }

        for (int i = 0; i < Quad::get_indices_count(); i++)
        {
            _indices.push_back(*(front_side.get_indices() + i) + (num_sides * 4));
        }
    }
}

Cube::Cube(const Quad& quad) : _base_quad(quad)
{
}
