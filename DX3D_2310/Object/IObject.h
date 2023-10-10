#pragma once

struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT4 color;

    Vertex(float x, float y, float z, float r, float g, float b)
        : pos(x, y, z), color(r, g, b, 1)
    {

    }

    Vertex()
        : pos(0, 0, 0), color(0, 0, 0, 1)
    {

    }

    Vertex(float x, float y, float z, XMFLOAT3 color)
        : pos(x, y, z), color(color.x, color.y, color.z, 1)
    {

    }
    
};

class Iobject
{
private:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Delete() = 0;
};