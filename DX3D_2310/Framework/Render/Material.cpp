#include "Framework.h"

UINT Material::material_Key = 0;

Material::Material(wstring shderFIle, string name)
	:	name(name)
{
	SetShader(shderFIle);
	
	this->name += material_Key + 48;
	buffer = new MaterialBuffer();
	Load();
	material_Key++;
}

Material::~Material()
{
	Save();
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

		if (ImGui::Button("SaveData"))
			Save();

		if (ImGui::Button("OpenSaveData"))
			DIALOG->OpenDialog(this->name + "OpenSaveData", "OpenSaveData", ".mat", ".");

		if (DIALOG->Display(this->name + "OpenSaveData"))
		{
			if (DIALOG->IsOk())
			{
				string file = DIALOG->GetFilePathName();
				Load(file);
			}
			DIALOG->Close();
		}
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

void Material::Save()
{
	BinaryWriter* writer = new BinaryWriter("TextData/Material/" + name + ".mat");

	writer->String(name);

	SaveTexture(diffuseMap, writer);
	SaveTexture(specularMap, writer);
	SaveTexture(normalMap, writer);

	writer->Float_4(buffer->GetData()->diffuse);
	writer->Float_4(buffer->GetData()->specular);
	writer->Float_4(buffer->GetData()->ambient);

	writer->Float(buffer->GetData()->shininess);

	delete writer;
}

void Material::Load(string file)
{
	BinaryReader* reader;
	if (file == "")
		reader = new BinaryReader("TextData/Material/" + name + ".mat");
	else
		reader = new BinaryReader(file);

	if (reader->IsFailed())
	{
		diffuseMap = Texture::Add(L"Textures/Colors/White.png");
		specularMap = Texture::Add(L"Textures/Colors/White.png");
		normalMap = Texture::Add(L"Textures/Colors/Blue.png");
		delete reader;
		return;
	}

	name = reader->String();

	LoadTexture(diffuseMap, reader, DIFFUSE);
	LoadTexture(specularMap, reader, SPECULAR);
	LoadTexture(normalMap, reader, NORMAL);
	
	buffer->GetData()->diffuse	= reader->Float_4();
	buffer->GetData()->specular = reader->Float_4();
	buffer->GetData()->ambient	= reader->Float_4();

	buffer->GetData()->shininess = reader->Float();

	Set();

	delete reader;
}

void Material::SaveTexture(Texture*& data, BinaryWriter*& writer)
{
	if (data == nullptr) {
		writer->WString(L"NULLSET");
		return;
	}
	writer->WString(data->GetFilename());
}

void const Material::LoadTexture (Texture*& data, BinaryReader* reader, MapType num)
{
	wstring defstring = reader->WString();
	switch (num)
	{
	case Material::DIFFUSE:
		if (defstring != L"NULLSET")
			data = Texture::Add(defstring);

		break;
	case Material::SPECULAR:
		if (defstring != L"NULLSET")
			data = Texture::Add(defstring);

		break;
	case Material::NORMAL:
		if (defstring != L"NULLSET") {
			data = Texture::Add(defstring);
			buffer->GetData()->hasNormalMap = true;
		}
		else
			buffer->GetData()->hasNormalMap = false;
		break;
	}

}
