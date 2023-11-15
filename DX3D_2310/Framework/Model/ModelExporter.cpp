#include "Framework.h"

ModelExporter::ModelExporter(string name, string file)
	:	name(name)
{
	importer = new Assimp::Importer();

	importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

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

void ModelExporter::ExportMesh()
{
	ReadNode(scene->mRootNode, -1, -1);
	// rootnode��-1���� �������.
	ReadMesh(scene->mRootNode);
	WriteMesh();
}

void ModelExporter::ReadMaterial()
{
	FOR(scene->mNumMaterials)
	{
		// FBX ���Ͽ��� Material �����͸� �����ɴϴ�.
		aiMaterial* srcMaterial = scene->mMaterials[i];

		// Material Ŭ������ �ν��Ͻ��� �����մϴ�.
		Material* material = new Material();

		// Material�� �̸��� ���� (Material�� �̸��� FBX���� �����ɴϴ�).
		material->SetName(srcMaterial->GetName().C_Str());

		// MaterialBuffer Ŭ������ ���� Material�� �����Ϳ� �����մϴ�.
		MaterialBuffer::Data* data = material->GetBuffer();

		// Material�� ���ݻ� (Diffuse) �÷� ����
		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		data->diffuse = Float4(color.r, color.g, color.b, 1.0f);

		// Material�� ����ŧ�� (Specular) �÷� ����
		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		data->specular = Float4(color.r, color.g, color.b, 1.0f);

		// Material�� �����Ʈ (Ambient) �÷� ����
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		data->ambient = Float4(color.r, color.g, color.b, 1.0f);

		// Material�� ��ü�߱� (Emissive) �÷� ����
		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		data->emissive = Float4(color.r, color.g, color.b, 1.0f);

		// Material�� �� �ݻ��� ��ī�ο� (Shininess) ����
		srcMaterial->Get(AI_MATKEY_SHININESS, data->shininess);

		// Material�� ���ݻ� (Diffuse) �ؽ�ó ����
		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		// Material�� ����ŧ�� (Specular) �ؽ�ó ����
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		// Material�� ���� (Normals) �� �ؽ�ó ����
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		// Material ��ü�� materials ���Ϳ� �߰�
		materials.push_back(material);
	}
}

void ModelExporter::WriterMaterial()
{
	// Material �����͸� ������ ���͸� ��� ���� (��: "Models/Materials/���̸�/")
	string savePath = "Models/Materials/" + name + "/";

	// Material �����͸� ������ ���� �̸� ���� (��: "���̸�.mats")
	string file = name + ".mats";

	// �ʿ��� ���� ������ �����ϴ� �Լ� ȣ��
	CreateFolders(savePath);

	// BinaryWriter�� ����Ͽ� ������ �����ϰ� ����
	BinaryWriter* writer = new BinaryWriter(savePath + file);

	// Material �������� ������ ���Ͽ� ���� (materials ������ ũ��)
	writer->UInt(materials.size());

	// materials ���Ϳ� �ִ� �� Material ��ü�� ó��
	for (Material* material : materials)
	{
		// Material�� ������ ���� ��� ���� (��: "Models/Materials/���̸�/�����̸�.mat")
		string path = savePath + material->GetName() + ".mat";

		// Material ��ü�� �ش� ��ο� ����
		material->Save(path);

		// Material ������ ��θ� ���Ͽ� ���ڿ� ���·� ����
		writer->String(path);

		// ���� ó���� Material ��ü�� ���� (�޸� ����)
		delete material;
	}

	// materials ���͸� ����, BinaryWriter�� �ݰ� ����
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

void ModelExporter::ReadNode(aiNode* node, int index, int parent)
{
	NodeData* nodeData = new NodeData();
	nodeData->index = index;
	nodeData->parent = parent;
	nodeData->name = node->mName.C_Str();

	Matrix matrix(node->mTransformation[0]);
	nodeData->transform = XMMatrixTranspose(matrix); // ��Ʈ���� ��ȯ�ؾ���.

	nodes.push_back(nodeData);

	FOR(node->mNumChildren)
		ReadNode(node->mChildren[i], nodes.size(), index);
}

void ModelExporter::ReadMesh(aiNode* node)
{
	FOR(node->mNumMeshes)
	{
		// ���ο� MeshData ��ü�� ����
		MeshData* mesh = new MeshData();

		// Mesh�� �̸��� ����, node->mName.C_Str()�� C ���ڿ��� Mesh�� �̸��� �����մϴ�.
		mesh->name = node->mName.C_Str();

		// ���� ó�� ���� Mesh�� ���� �����͸� �����ɴϴ�.
		aiMesh* srcMesh = scene->mMeshes[i];

		// Mesh�� ����(Material) �ε����� MeshData ��ü�� �Ҵ�
		mesh->materialIndex = srcMesh->mMaterialIndex;

		// Mesh�� ����(Vertex) �����͸� ����
		mesh->vertices.resize(srcMesh->mNumVertices);

		FOR(srcMesh->mNumVertices)
		{
			ModelVertex vertex;

			// ���� ��ġ ������ ����
			memcpy(&vertex.pos, &srcMesh->mVertices[i], sizeof(Float3));

			// �ؽ�ó ��ǥ ������ �ִ� ���, ����
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][i], sizeof(Float2));

			// ���� ���� ������ �ִ� ���, ����
			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[i], sizeof(Float3));

			// ���� ���� �� ������ ���� ������ �ִ� ���, ����
			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[i], sizeof(Float3));

			// MeshData�� ���� ��Ͽ� ���� ���� ������ �߰�
			mesh->vertices[i] = vertex;
		}

		// Mesh�� �ε��� �����͸� ���� �����ϱ����� �迭�� resize�� �÷���.
		mesh->indices.resize(srcMesh->mNumFaces * 3);
		FOR(srcMesh->mNumFaces)
		{
			// face = �׸������ ��, �� �̶�� �����ϸ��.
			aiFace& face = srcMesh->mFaces[i];

			// �� ��(Face)�� �ε����� MeshData ��ü�� �ε��� ��Ͽ� �߰�
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				mesh->indices[i * 3 + j] = face.mIndices[j];
			}
		}

		// MeshData ��ü�� meshes ���Ϳ� �߰�
		meshes.push_back(mesh);
	}

	FOR(node->mNumChildren)
	{
		// ���� ����� �ڽ� ��忡 ���� ��������� Mesh�� �а� ó�� �޽��� Ʈ�����·� �Ǿ��ֱ⿡ ��ͷ� �о����.
		ReadMesh(node->mChildren[i]);
	}
}

void ModelExporter::WriteMesh()
{
	// ������ ���� ��θ� ���� (��: "Models/Meshes/���̸�.mesh")
	string path = "Models/Meshes/" + name + ".mesh";

	// �ʿ��� ���� ������ �����ϴ� �Լ� ȣ��
	CreateFolders(path);

	// BinaryWriter�� ����Ͽ� ������ �����ϰ� ����
	BinaryWriter* writer = new BinaryWriter(path);

	// Mesh �������� ������ ���Ͽ� ���� (meshes ������ ũ��)
	writer->UInt(meshes.size());

	/*
		�̸� ������ ������ ���� ����,
		����ȭ�� �����͸� �����ϴµ� �� ȿ�����̱� ����,
		������ ������ ����� ������ ������ �̸� ���°�ó��
	*/

	// meshes ���Ϳ� �ִ� �� MeshData ��ü�� ó��
	for (MeshData* mesh : meshes)
	{
		// Mesh�� �̸��� ���Ͽ� ���ڿ� ���·� ����
		writer->String(mesh->name);

		// Mesh�� ����(Material) �ε����� ���Ͽ� ����
		writer->UInt(mesh->materialIndex);

		// Mesh�� ���� ������ ������ ���Ͽ� ����
		writer->UInt(mesh->vertices.size());

		// Mesh�� ���� �����͸� ���Ͽ� ���� ���·� ���� (���� ����ü�� �迭)
		writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

		// Mesh�� �ε��� ������ ������ ���Ͽ� ����
		writer->UInt(mesh->indices.size());

		// Mesh�� �ε��� �����͸� ���Ͽ� ���� ���·� ���� (���� �迭)
		writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		// ���� ó���� MeshData ��ü�� ���� (�޸� ����)
		delete mesh;
	}
	// ������ ���������ʹ� �о �ľ��� ����� ������, ���������� ���·� ������� �پ�� �����Ѵ�.
	// meshes ���͸� ����, BinaryWriter�� �ݰ� ����
	meshes.clear();

	writer->UInt(nodes.size());
	for (NodeData* node : nodes)
	{
		writer->Int(node->index);
		writer->String(node->name);
		writer->Int(node->parent);
		writer->Matrix(node->transform);

		delete node;
	}
	nodes.clear();

	delete writer;
}