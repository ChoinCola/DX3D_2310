#include "Framework.h"

TerrainEditScene::TerrainEditScene()
{
	terrainEditor = new TerrainEditer();
}

TerrainEditScene::~TerrainEditScene()
{
	delete terrainEditor;
}

void TerrainEditScene::Update()
{
	terrainEditor->Update();
}

void TerrainEditScene::PreRender()
{
}

void TerrainEditScene::Render()
{
	terrainEditor->Render();
}

void TerrainEditScene::PostRender()
{
}

void TerrainEditScene::GUIRender()
{
    terrainEditor->GUIRender();
}

void TerrainEditScene::ByteAddress()
{
    ComputeShader* shader = Shader::AddCS(L"Compute/ByteAddress.hlsl");

    struct Output
    {
        UINT groupID[3];
        UINT groupThreadID[3];
        UINT dispatchThreadID[3];
        UINT groupIndex;
    };

    UINT grouptX = 2;
    UINT size = 10 * 8 * 3 * grouptX;

    Output* output = new Output[size];
    RawBuffer* buffer = new RawBuffer(sizeof(Output) * size);

    DC->CSSetUnorderedAccessViews(0, 1, &buffer->GetUAV(), nullptr);
    shader->Set();

    DC->Dispatch(grouptX, 1, 1);

    buffer->Copy(output, sizeof(Output) * size);

    FILE* file;
    fopen_s(&file, "TextData/RawTest.csv", "w");

    for (UINT i = 0; i < size; i++)
    {
        Output temp = output[i];

        fprintf(
            file,
            "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
            i,
            temp.groupID[0], temp.groupID[1], temp.groupID[2],
            temp.groupThreadID[0], temp.groupThreadID[1], temp.groupThreadID[2],
            temp.dispatchThreadID[0], temp.dispatchThreadID[1], temp.dispatchThreadID[2],
            temp.groupIndex
        );
    }
}
