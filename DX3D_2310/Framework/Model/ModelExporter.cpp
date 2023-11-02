#include "Framework.h"

ModelExporter::ModelExporter(string name, string file)
	:	name(name)
{
	importer = new Assimp::Importer();

	// �޼���ǥ�� ����Ʈ | �ִ�����Ƽ��
	scene = importer->ReadFile(file,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	// fbx�� �������, ������ �߸��Ǿ��� ��� ��Ʈ���°� �� ����.
	assert(scene != nullptr);
	/*
		NumVertices = ���ؽ� ��
		NumFaces = �ﰢ�� �׸��� ��
		Tangents = ������ ��ָ�
	*/
}

ModelExporter::~ModelExporter()
{
}

void ModelExporter::ExportMaterial()
{
	ReadMaterial();
	WriterMaterial();
}

void ModelExporter::ReadMaterial()
{
	FOR(scene->mNumMaterials)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		Material* material = new Material();

		material->SetName(srcMaterial->GetName().C_Str());

		MaterialBuffer::Data* data = material->GetBuffer();

		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);// ���ݻ� �÷� ����
		data->diffuse = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);// ����ŧ��
		data->specular = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);// �����Ʈ
		data->ambient = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);// ��ü�߱�
		data->emissive = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, data->shininess); // �� �ݻ��� ��ī�ο�.

		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		materials.push_back(material);
	}
}

void ModelExporter::WriterMaterial()
{
	string savePath = "Models/Materials/" + name + "/";
	string file = name + ".mats";

	CreateFolders(savePath);

	// �迭 ����� ������ �迭 ���̺��� �����ؾ��Ѵ�.
	BinaryWriter* writer = new BinaryWriter(savePath + file);

	writer->UInt(materials.size());

	for (Material* material : materials)
	{
		string path = savePath + material->GetName() + ".mat";
		material->Save(path);

		writer->String(path);

		delete material;
		
	}
	materials.clear();

	delete writer;
}

string ModelExporter::CreateTexture(string file)
{
	// �Էµ� ���ϸ��� ���̰� 0�� ���, �� ���ڿ�("")�� ��ȯ�Ѵ�.
	if (file.length() == 0)
		return "";
	
	// �Էµ� ���ϸ��� Ȯ���ڸ� ������ �κ��� �����ͼ� ".pngȮ���ڸ� �ٿ��ش�. ���� Ȯ���ڴ� �����Ѵ�.
	string fileName = GetFileNameWithoutExtension(file) + ".png"; // Ȯ���� ����
	string path = "Textures/Model/" + name + "/" + fileName;

	// ��ο� �ش��ϴ� ������ �����ϴ� �Լ��� ȣ��
	CreateFolders(path); // ���� �����

	// Assimp���̺귯���� ����Ͽ� �Էµ� ���ϸ� �ش��ϴ� ���� �ؽ�ó�� �ܾ�´�.
	const aiTexture* srcTexture = scene->GetEmbeddedTexture(file.c_str());

	// ���� �ؽ�ó�� �������� �ʴ°�� �� ���ڿ�("")�� ��ȯ�Ѵ�.
	// �����ӿ�ũ�� �� �ؽ��İ� ����ȴ�.
	if (srcTexture == nullptr)
		return "";

	// �����ؽ�ó�� ���̰� 1���� �������, ���̳ʸ����Ϸ� �����Ѵ�
	// �����ؽ�ó�� ���̰� 1���� Ŭ ���, 3D�ؽ�ó�� �ǹ��Ѵ�.
	// �Ϲ����� �ؽ�ó �ʹ� �ٸ��� 3D������ �����Ͽ� ���������ش�.
	if (srcTexture->mHeight < 1)
	{
		BinaryWriter w(path);
		w.Byte(srcTexture->pcData, srcTexture->mWidth);
		// �׳� �״�� ���پ� �о�����ڿ� Byte�� �о �����ع����� png�� ������ �о�����!
	}
	else
	{
		// �̹��� ��ü�� �����ϰ� �ؽ�ó �����͸� �� ��ü�� �����Ѵ�.
		Image image; 
		image.width = srcTexture->mWidth; // �̹����� �ʺ�ũ��(�����ȼ���)
		image.height = srcTexture->mHeight; // �̹����� ����ũ��(�����ȼ���)
		image.pixels = (uint8_t*)(srcTexture->pcData); // �̹����������� ������ ����

		// �̹��� �������� �� ��(row)�� �����ϴ� ����Ʈ ���� ����.
		// 4�� �Ϲ������� RGBA���� ������ ��Ÿ����. �� �ȼ��� 4����Ʈ�� ���.
		image.rowPitch = image.width * 4;

		// �̹��� �������� ��ü ũ�⸦ ��Ÿ���� ������ �ʺ� * ���� * 4����Ʈ�� ����Ͽ� �����Ѵ�.
		// �̹��� ��ü ���������� ũ�⸦ ��Ÿ����.
		image.slicePitch = image.width * image.height * 4;

		// WIC�� ����Ͽ� �̹����� ���Ϸ� �����Ѵ�.
		SaveToWICFile(
			image,						// 1. �̹��� �����͸� �����ϴ� Image ����ü
			WIC_FLAGS_NONE,				// 2. WIC�� ���� �÷��� ����. ���⼭�� �ƹ� Ư���� �÷��װ� ������ ������ ��Ÿ����.
			GetWICCodec(WIC_CODEC_PNG), // 3. WIC �ڵ��� ���´�. �� �ڵ忡���� PNG ������ ����Ͽ� �̹����� ����.
			ToWString(path).c_str()		// 4. �̹����� ������ ���� ��θ� �����ڵ� ���ڿ��� ��ȯ�Ͽ� �����Ѵ�.
		);
	}

	return path;
}
