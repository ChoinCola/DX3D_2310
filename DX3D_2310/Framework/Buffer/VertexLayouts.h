#pragma once

struct Vertex
{
    Float3 pos = {};

    Vertex() {}

    Vertex(float x, float y, float z)
        : pos(x, y, z)
    {}
};

struct VertexColor
{
    Float3 pos = {};
    Float4 color = { 1, 1, 1, 1 };

    VertexColor() {}

    VertexColor(float x, float y, float z, float r, float g, float b)
        : pos(x, y, z), color(r, g, b, 1)
    {
    }

    VertexColor(float x, float y, float z)
        : pos(x, y, z), color(1, 1, 1, 1)
    {
    }

};