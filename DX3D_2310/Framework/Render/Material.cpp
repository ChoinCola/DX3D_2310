#include "Framework.h"

Material::Material(wstring shderFIle)
{
	SetShader(shderFIle);

	buffer = new MaterialBuffer();

	diffuseMap = Texture::Add(L"Textures/Colors/White.png");
	specularMap = Texture::Add(L"Textures/Colors/White.png");
	normalMap = Texture::Add(L"Textures/Colors/Blue.png");
}

Material::~Material()
{
	SAFE_DELETE(buffer);
}

void Material::GUIRneder()
{
	string title = name + "_Material";

	if (ImGui::TreeNode(title.c_str()))
	{
		char str[128];
		strcpy_s(str, 128, editName.c_str());
		ImGui::InputText("Name", str, 128);

		editName = str;

		ImGui::SameLine();
		if (ImGui::Button("Edit"))
			name = editName;

		ImGui::ColorEdit3("Diffuse", (float*)&buffer->GetData()->diffuse);
		ImGui::ColorEdit3("Specular", (float*)&buffer->GetData()->specular);
		ImGui::ColorEdit3("Ambient", (float*)&buffer->GetData()->ambient);

		ImGui::SliderFloat("Shininess", (float*)&buffer->GetData()->shininess, 1, 50);

		SelectMap("DM", DIFFUSE);
		ImGui::SameLine();
		UnselectMap(DIFFUSE);

		SelectMap("SM", SPECULAR);
		ImGui::SameLine();
		UnselectMap(SPECULAR);

		SelectMap("NM", NORMAL);
		ImGui::SameLine();
		UnselectMap(NORMAL);

		ImGui::TreePop();
	}
}

void Material::Set()
{
	if (diffuseMap)
		diffuseMap->PSSet(0);

	if (specularMap)
		specularMap->PSSet(1);

	if (normalMap)
		normalMap->PSSet(2);

	buffer->SetPS(2);
	// 버퍼 2번에 저장 전달

	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(shaderFile);
	pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
	if (textureFile.size() > 0)
		diffuseMap = Texture::Add(textureFile);
	else
		diffuseMap = Texture::Add(L"DM");
}

void Material::SetSpecularMap(wstring textureFile)
{
	if (textureFile.size() > 0)
		specularMap = Texture::Add(textureFile);
	else
		specularMap = Texture::Add(L"SM");
}

void Material::SetNormalMap(wstring textureFile)
{
	if (textureFile.size() > 0) {
		normalMap = Texture::Add(textureFile);
		buffer->GetData()->hasNormalMap = true;
	}
	else
	{
		normalMap = Texture::Add(L"Textures/Colors/Blue.png");
		buffer->GetData()->hasNormalMap = false;
	}
}

void Material::DeleteDiffuseMap()
{
	diffuseMap = nullptr;
}

void Material::DeleteSpecularMap()
{
	specularMap = Texture::Add(L"Textures/Colors/White.png");
}

void Material::DeleteNormalMap()
{
	normalMap = Texture::Add(L"Textures/Colors/Blue.png");
}

void Material::SelectMap(string name, MapType mapType)
{
	ImGui::TextColored(ImVec4(1, 0.5f, 0.8f, 1), name.c_str());
	ImGui::SameLine();

	ImTextureID textureID = nullptr;

	switch (mapType)
	{
	case Material::DIFFUSE:
		textureID = diffuseMap->GetSRV();
		break;
	case Material::SPECULAR:
		textureID = specularMap->GetSRV();

		break;
	case Material::NORMAL:
		textureID = normalMap->GetSRV();
		break;
	}

	if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
		DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga", ".");

	if (DIALOG->Display(this->name + name))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			switch (mapType)
			{
			case Material::DIFFUSE:
				SetDiffuseMap(ToWString(file));
				break;
			case Material::SPECULAR:
				SetSpecularMap(ToWString(file));
				break;
			case Material::NORMAL:
				SetNormalMap(ToWString(file));
				break;
			}
		}

		DIALOG->Close();
	}
}

void Material::UnselectMap(MapType mapType)
{
	string str;

	switch (mapType)
	{
	case Material::DIFFUSE:
		str = "DM Clear";
		break;
	case Material::SPECULAR:
		str = "SM Clear";
		break;
	case Material::NORMAL:
		str = "NM Clear";
		break;
	}

	if (ImGui::Button(str.c_str()))
	{
		switch (mapType)
		{
		case Material::DIFFUSE:
			SetDiffuseMap(L"");
			break;
		case Material::SPECULAR:
			SetSpecularMap(L"");
			break;
		case Material::NORMAL:
			SetNormalMap(L"");
			break;
		default:
			break;
		}
	}

}
