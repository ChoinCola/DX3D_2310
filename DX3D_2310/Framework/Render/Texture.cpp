#include "Framework.h"

unordered_map<wstring, Texture*> Texture::textures;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file)
    : srv(srv), image(move(image)), file(file)
{
}

Texture::~Texture()
{
    srv->Release();
}

void Texture::PSSet(UINT slot)
{
    // 배열형대로 받을 수도 있다.
    DC->PSSetShaderResources(slot, 1, &srv);
}

void Texture::ReadPixels(vector<Float4>& pixels)
{
    uint8_t* colors = image.GetPixels();
    UINT size = image.GetPixelsSize();

    pixels.resize(size / 4);

    float scale = 1.0f / 255.0f;
    
    for (UINT i = 0; i < pixels.size(); i++) {
        pixels[i].x = colors[i * 4 + 0] * scale;
        pixels[i].y = colors[i * 4 + 1] * scale;
        pixels[i].z = colors[i * 4 + 2] * scale;
        pixels[i].w = colors[i * 4 + 3] * scale;

    }
}

Texture* Texture::Add(wstring file)
{
    if (textures.count(file) > 0)
        return textures[file];

    ScratchImage image;
    LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image);
    ID3D11ShaderResourceView* srv;

    // srv할당
    CreateShaderResourceView(DEVICE, image.GetImages(),
        image.GetImageCount(), image.GetMetadata(), &srv);

    textures[file] = new Texture(srv, image, file);
    return textures[file];
}

void Texture::Delete()
{
    for (pair<wstring, Texture*> texture : textures)
        delete texture.second;
}
