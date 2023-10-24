#pragma once
//VertexLayouts.h

struct Vertex
{
    Float3 pos = {};

    Vertex()
    {
    }

    Vertex(float x, float y, float z)
        : pos(x, y, z)
    {
    }
};

struct VertexColor
{
    Float3 pos = {};
    Float4 color = { 1, 1, 1, 1 };

    VertexColor()
    {
    }

    VertexColor(float x, float y, float z, float r, float g, float b)
        : pos(x, y, z), color(r, g, b, 1)
    {
    }
};

struct VertexUV
{
    Float3 pos = {};
    Float2 uv = {};

    VertexUV()
    {
    }

    VertexUV(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct VertexUVNormal
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};

    VertexUVNormal()
    {
    }

    VertexUVNormal(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct VertexUVNormalTangent
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};
    Float3 tangent = {};

    VertexUVNormalTangent()
    {
    }

    VertexUVNormalTangent(float x, float y, float z)
        : pos(x, y, z)
    {
    }

    VertexUVNormalTangent(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};