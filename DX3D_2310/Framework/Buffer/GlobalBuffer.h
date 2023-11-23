#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
	MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
	{
		matrix = XMMatrixIdentity();
	}
	
	void Set(Matrix value)
	{
		matrix = XMMatrixTranspose(value);
	}

private:
	Matrix matrix;
};

class ViewBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix view;
		Matrix invView;
	};
public:
	ViewBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.view = XMMatrixIdentity();
		data.invView = XMMatrixIdentity();
	}

	void Set(Matrix view, Matrix invView)
	{
		data.view = XMMatrixTranspose(view);
		data.invView = XMMatrixTranspose(invView);
	}

private:
	Data data;
};

class LightBuffer : public ConstBuffer
{
public:
	enum LightType
	{
		Directional,
		Point,
		Spot
	};

	struct Light
	{
		Float4 color = { 1, 1, 1, 1 };

		Float3 direction = { 0, -1, 1 };
		int type;

		Float3 position = { 0, 0, 0 };
		float range = 100.0f;

		float inner = 55.0f;
		float outer = 70.0f;
		int isActive;
		float attentionIntensity = 1.5f;
	};
private:
	struct Data
	{
		Light lights[MAX_LIGHT];

		int lightCount = 1;
		Float3 ambientLight = { 0.1f, 0.1f, 0.1f };

		Float3 ambientCeil = { 0.1f, 0.1f, 0.1f };
		float padding;
	};

public:
	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

	void Save(string file, UINT num)
	{
		tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
		tinyxml2::XMLElement* Light = document->NewElement("Light");
		Light->SetAttribute("Name", file.c_str());
		document->InsertFirstChild(Light);

		tinyxml2::XMLElement* property = document->NewElement("Property");
		tinyxml2::XMLElement* color = document->NewElement("Color");
		color->SetAttribute("R", data.lights[num].color.x);
		color->SetAttribute("G", data.lights[num].color.y);
		color->SetAttribute("B", data.lights[num].color.z);
		color->SetAttribute("A", data.lights[num].color.w);
		property->InsertEndChild(color);

		tinyxml2::XMLElement* direction = document->NewElement("direction");
		direction->SetAttribute("X", data.lights[num].direction.x);
		direction->SetAttribute("Y", data.lights[num].direction.y);
		direction->SetAttribute("Z", data.lights[num].direction.z);
		property->InsertEndChild(direction);

		tinyxml2::XMLElement* position = document->NewElement("position");
		position->SetAttribute("X", data.lights[num].position.x);
		position->SetAttribute("Y", data.lights[num].position.y);
		position->SetAttribute("Z", data.lights[num].position.z);
		property->InsertEndChild(position);

		tinyxml2::XMLElement* type = document->NewElement("type");
		type->SetAttribute("type", data.lights[num].type);
		property->InsertEndChild(type);

		tinyxml2::XMLElement* range = document->NewElement("range");
		range->SetAttribute("range", data.lights[num].range);
		property->InsertEndChild(range);

		tinyxml2::XMLElement* inner = document->NewElement("inner");
		inner->SetAttribute("inner", data.lights[num].inner);
		property->InsertEndChild(inner);

		tinyxml2::XMLElement* outer = document->NewElement("outer");
		outer->SetAttribute("outer", data.lights[num].outer);
		property->InsertEndChild(outer);

		tinyxml2::XMLElement* isActive = document->NewElement("isActive");
		isActive->SetAttribute("isActive", data.lights[num].isActive);
		property->InsertEndChild(isActive);

		tinyxml2::XMLElement* attentionIntensity = document->NewElement("attentionIntensity");
		attentionIntensity->SetAttribute("attentionIntensity", data.lights[num].attentionIntensity);
		property->InsertEndChild(attentionIntensity);

		tinyxml2::XMLElement* AmbientLight = document->NewElement("AmbientLight");
		AmbientLight->SetAttribute("R", data.ambientLight.x);
		AmbientLight->SetAttribute("G", data.ambientLight.y);
		AmbientLight->SetAttribute("B", data.ambientLight.z);
		property->InsertEndChild(AmbientLight);

		tinyxml2::XMLElement* AmbientCeil = document->NewElement("AmbientCeil");
		AmbientCeil->SetAttribute("R", data.ambientCeil.x);
		AmbientCeil->SetAttribute("G", data.ambientCeil.y);
		AmbientCeil->SetAttribute("B", data.ambientCeil.z);
		property->InsertEndChild(AmbientCeil);

		Light->InsertEndChild(property);

		document->SaveFile((file + to_string(num) + ".lu").c_str());
		delete document;
	}
	void Load(string file, UINT num)
	{
		tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
		document->LoadFile((file + to_string(num) + ".lu").c_str());

		if(document->Error())
			document->LoadFile(file.c_str());

		if (document->Error()) return;

		tinyxml2::XMLElement* Light = document->FirstChildElement();
		tinyxml2::XMLElement* property = Light->FirstChildElement();

		tinyxml2::XMLElement* color = property->FirstChildElement();
		data.lights[num].color.x = color->FloatAttribute("R");
		data.lights[num].color.y = color->FloatAttribute("G");
		data.lights[num].color.z = color->FloatAttribute("B");
		data.lights[num].color.w = color->FloatAttribute("A");

		tinyxml2::XMLElement* direction = color->NextSiblingElement();
		data.lights[num].direction.x = direction->FloatAttribute("X");
		data.lights[num].direction.y = direction->FloatAttribute("Y");
		data.lights[num].direction.z = direction->FloatAttribute("Z");

		tinyxml2::XMLElement* position = direction->NextSiblingElement();
		data.lights[num].position.x = position->FloatAttribute("X");
		data.lights[num].position.y = position->FloatAttribute("Y");
		data.lights[num].position.z = position->FloatAttribute("Z");

		tinyxml2::XMLElement* type = position->NextSiblingElement();
		data.lights[num].type = type->IntAttribute("type");

		tinyxml2::XMLElement* range = type->NextSiblingElement();
		data.lights[num].range = range->FloatAttribute("range");

		tinyxml2::XMLElement* inner = range->NextSiblingElement();
		data.lights[num].inner = inner->FloatAttribute("inner");

		tinyxml2::XMLElement* outer = inner->NextSiblingElement();
		data.lights[num].outer = outer->FloatAttribute("outer");

		tinyxml2::XMLElement* isActive = outer->NextSiblingElement();
		data.lights[num].isActive = isActive->FloatAttribute("isActive");

		tinyxml2::XMLElement* attentionIntensity = isActive->NextSiblingElement();
		data.lights[num].attentionIntensity = attentionIntensity->FloatAttribute("attentionIntensity");

		tinyxml2::XMLElement* AmbientLight = attentionIntensity->NextSiblingElement();
		data.ambientLight.x = AmbientLight->FloatAttribute("R");
		data.ambientLight.y = AmbientLight->FloatAttribute("G");
		data.ambientLight.z = AmbientLight->FloatAttribute("B");

		tinyxml2::XMLElement* AmbientCeil = AmbientLight->NextSiblingElement();
		data.ambientCeil.x = AmbientCeil->FloatAttribute("R");
		data.ambientCeil.y = AmbientCeil->FloatAttribute("G");
		data.ambientCeil.z = AmbientCeil->FloatAttribute("B");

		delete document;
	}

	Data* GetData() { return &data; }

private:
	Data data;
};

class MaterialBuffer : public ConstBuffer
{
public :
	struct Data
	{
		Float4 diffuse = { 1, 1, 1, 1 };
		Float4 specular = { 1, 1, 1, 1 };
		Float4 ambient = { 1, 1, 1, 1 };
		// 맞을때 빤짝거리는 것을 구현하기 위한 Data
		Float4 emissive = { 0, 0, 0, 0 };

		float shininess = 24.0f;
		float hasNormalMap = 0;
		float padding[2];
	};

public:
	MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

	Data* GetData() { return &data; }

private:
	Data data;
};