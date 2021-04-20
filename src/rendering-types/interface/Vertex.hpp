#pragma once

struct Vertex
{
    Vertex() = default;
    Vertex(float v) : x(v), y(v), z(v), u(0.0f), v(0.0f) {}
    explicit Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
    explicit Vertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {}

    float x{};
    float y{};
    float z{};

    float u{};
    float v{};
};
