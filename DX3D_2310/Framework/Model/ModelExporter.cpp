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

void ModelExporter::ExportClip(string clipName)
{
	FOR(scene->mNumAnimations)
	{
		Clip* clip = ReadClip(scene->mAnimations[i]);
		WriteClip(clip, clipName, i);
	}
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

// ���뿡 ���� �������� �̱�.
void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
	FOR(mesh->mNumBones)
	{
		UINT boneIndex = 0;
		string name = mesh->mBones[i]->mName.C_Str();

		// ���밡 �̹� �ִ��� Ȯ��
		if (boneMap.count(name) == 0)
		{
			boneIndex = boneCount++;
			boneMap[name] = boneIndex;

			BoneData* boneData = new BoneData();
			boneData->name = name;
			boneData->index = boneIndex;

			Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);// �迭�� �����ϱ�� ������, ��������� �� ���� ���� ���� ��.
			boneData->offset = XMMatrixTranspose(matrix);

			bones.push_back(boneData);
		}
		else
		{
			boneIndex = boneMap[name];
		}

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
			vertexWeights[index].Add(boneIndex,
				mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
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
		vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
		ReadBone(srcMesh, vertexWeights);


		// Mesh�� ����(Vertex) �����͸� ����
		mesh->vertices.resize(srcMesh->mNumVertices);

		for(UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			ModelVertex vertex;

			// ���� ��ġ ������ ����
			memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

			// �ؽ�ó ��ǥ ������ �ִ� ���, ����
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

			// ���� ���� ������ �ִ� ���, ����
			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

			// ���� ���� �� ������ ���� ������ �ִ� ���, ����
			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

			if (!vertexWeights.empty())
			{
				vertexWeights[v].Normalize();
				
				vertex.indices.x = (float)vertexWeights[v].indices[0];
				vertex.indices.y = (float)vertexWeights[v].indices[1];
				vertex.indices.z = (float)vertexWeights[v].indices[2];
				vertex.indices.w = (float)vertexWeights[v].indices[3];

				vertex.weights.x = vertexWeights[v].weights[0];
				vertex.weights.y = vertexWeights[v].weights[1];
				vertex.weights.z = vertexWeights[v].weights[2];
				vertex.weights.w = vertexWeights[v].weights[3];
			}
			// MeshData�� ���� ��Ͽ� ���� ���� ������ �߰�
			mesh->vertices[v] = vertex;
		}

		// Mesh�� �ε��� �����͸� ���� �����ϱ����� �迭�� resize�� �÷���.
		mesh->indices.resize(srcMesh->mNumFaces * 3);
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			// face = �׸������ ��, �� �̶�� �����ϸ��.
			aiFace& face = srcMesh->mFaces[f];

			// �� ��(Face)�� �ε����� MeshData ��ü�� �ε��� ��Ͽ� �߰�
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				mesh->indices[f * 3 + j] = face.mIndices[j];
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
	
	// �� ����� �����͸� �ۼ��ؼ� �����Ѵ�.
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

	// �� ������ �����͸� �ۼ��ؼ� �����Ѵ�.
	writer->UInt(bones.size());
	for (BoneData* bone : bones)
	{
		writer->Int(bone->index);
		writer->String(bone->name);
		writer->Matrix(bone->offset);

		delete bone;
	}
	bones.clear();

	delete writer;
}

Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
	// ���ο� Clip ��ü�� �������� �����մϴ�.
	Clip* clip = new Clip();

	// �ִϸ��̼��� �̸��� Clip ��ü�� �Ҵ��մϴ�.
	clip->name = animation->mName.C_Str();

	// �ִϸ��̼��� �ʴ� ƽ ���� Clip ��ü�� �Ҵ��մϴ�.
	clip->tickPerSecond = (float)animation->mTicksPerSecond;

	// �ִϸ��̼��� �� ������ ���� Clip ��ü�� �Ҵ��մϴ�.
	clip->frameCount = (UINT)(animation->mDuration) + 1;

	// �ִϸ��̼� ä��(���)�� ����ŭ ClipNode ��ü�� ���� ���͸� �Ҵ��մϴ�.
	vector<ClipNode> clipNodes;
	clipNodes.reserve(animation->mNumChannels);

	// �� �ִϸ��̼� ä��(���)�� ���� �ݺ��մϴ�.
	FOR(animation->mNumChannels)
	{
		// ���� ä��(���)�� �����ɴϴ�.
		aiNodeAnim* srcNode = animation->mChannels[i];

		// ���ο� ClipNode ��ü�� �����ϰ� �̸��� �����մϴ�.
		ClipNode node;
		node.name = srcNode->mNodeName;

		// KeyData ��ü�� �����Ͽ� �� Ű�������� ��ġ, ȸ��, �������� �����մϴ�.
		KeyData data;

		// ��ġ Ű������ ���� ����
		data.positions.resize(srcNode->mNumPositionKeys);
		for (UINT k = 0; k < srcNode->mNumPositionKeys; k++)
		{
			KeyVector keyPos;
			aiVectorKey key = srcNode->mPositionKeys[k];
			keyPos.time = key.mTime;
			memcpy_s(&keyPos.value, sizeof(Float3),
				&key.mValue, sizeof(aiVector3D));

			data.positions[k] = keyPos;
		}

		// ȸ�� Ű������ ���� ����
		data.rotations.resize(srcNode->mNumRotationKeys);
		for (UINT k = 0; k < srcNode->mNumRotationKeys; k++)
		{
			KeyQuat keyRot;
			aiQuatKey key = srcNode->mRotationKeys[k];
			keyRot.time = key.mTime;

			keyRot.value.x = (float)key.mValue.x;
			keyRot.value.y = (float)key.mValue.y;
			keyRot.value.z = (float)key.mValue.z;
			keyRot.value.w = (float)key.mValue.w;

			data.rotations[k] = keyRot;
		}

		// ������ Ű������ ���� ����
		data.scales.resize(srcNode->mNumScalingKeys);
		for (UINT k = 0; k < srcNode->mNumScalingKeys; k++)
		{
			KeyVector keyScale;
			aiVectorKey key = srcNode->mScalingKeys[k];
			keyScale.time = key.mTime;
			memcpy_s(&keyScale.value, sizeof(Float3),
				&key.mValue, sizeof(aiVector3D));

			data.scales[k] = keyScale;
		}

		// SetClipNode �Լ��� ���� Ű������ �����͸� ClipNode�� �����մϴ�.
		SetClipNode(data, clip->frameCount, node);

		// ���� ó���� ClipNode�� ���Ϳ� �߰��մϴ�.
		clipNodes.push_back(node);
	}

	// �о�� Ű������ �����͸� �̿��Ͽ� �ִϸ��̼��� ������ �����͸� �����մϴ�.
	ReadKeyFrame(clip, scene->mRootNode, clipNodes);

	// ������ Clip ��ü�� ��ȯ�մϴ�.
	return clip;
}

void ModelExporter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes)
{
	// Ű������ ���� �ϳ� �����
	KeyFrame* keyFrame = new KeyFrame();
	keyFrame->boneName = node->mName.C_Str();
	keyFrame->transforms.reserve(clip->frameCount);

	// Ŭ���� �� �����ӿ� ���� ó��
	FOR(clip->frameCount)
	{
		ClipNode* clipNode = nullptr;

		// ���� ��尡 Ŭ�� ��� ���Ϳ� �ִ��� Ȯ��
		for (ClipNode& temp : clipNodes)
		{
			if (temp.name == node->mName)
			{
				clipNode = &temp;
				break;
			}
		}

		// Ű�������� ��ȯ ������ ������ ����ü ����
		KeyTransform keyTransform;

		// Ŭ�� ��� ���Ϳ� ���� ��尡 ���� ���
		if (clipNode == nullptr)
		{
			// ���� ����� ���� ��ȯ ����� �о�ͼ� ����
			Matrix transform(node->mTransformation[0]);
			transform = XMMatrixTranspose(transform);

			Vector3 S, R, T;
			XMMatrixDecompose(S.GetValue(), R.GetValue(), T.GetValue(), transform);
			keyTransform.scale = S;
			XMStoreFloat4(&keyTransform.rot, R);
			keyTransform.pos = T;
		}
		else
		{
			// Ŭ�� ��� ���Ϳ� ���� ��尡 �ִ� ��� �ش� ����� ��ȯ ���� ���
			keyTransform = clipNode->transforms[i];
		}

		// Ű�����ӿ� ��ȯ ���� �߰�
		keyFrame->transforms.push_back(keyTransform);
	}

	// ������ Ű�������� Ŭ���� �߰�
	clip->keyFrames.push_back(keyFrame);

	// ����� �ڽĿ� ���� ��������� ó��
	FOR(node->mNumChildren)
		ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}

void ModelExporter::WriteClip(Clip* clip, string clipName, UINT index)
{
	// Ŭ���� ������ ���� ��� ����
	string file = "Models/Clips/" + name + "/" + clipName + to_string(index) + ".clip";

	// ������ �������� ������ ����
	CreateFolders(file);

	// BinaryWriter ��ü�� ����Ͽ� ���Ͽ� ����
	BinaryWriter* writer = new BinaryWriter(file);

	// Ŭ�� ���� ���
	writer->String(clip->name);
	writer->UInt(clip->frameCount);
	writer->Float(clip->tickPerSecond);

	// Ű������ ���� �� �� Ű������ ���� ���
	writer->UInt(clip->keyFrames.size());
	for (KeyFrame* keyFrame : clip->keyFrames)
	{
		writer->String(keyFrame->boneName);
		writer->UInt(keyFrame->transforms.size());
		writer->Byte(keyFrame->transforms.data(), sizeof(KeyTransform) * keyFrame->transforms.size());

		// ����� �Ϸ�� Ű������ ��ü ����
		delete keyFrame;
	}

	// ����� �Ϸ�� Ŭ�� ��ü ����
	delete clip;

	// BinaryWriter ��ü ����
	delete writer;
}

void ModelExporter::SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode)
{
	// Ŭ�� ��忡 Ű�������� ��ȯ(transform)�� ������ ���͸� ������ ����ŭ �Ҵ��մϴ�.
	clipNode.transforms.resize(frameCount);

	UINT posCount = 0;
	UINT rotCount = 0;
	UINT scaleCount = 0;

	// �� �����ӿ� ���� ������ ��ġ, ȸ��, ������ ���� ����Ͽ� Ŭ�� ��忡 �����մϴ�.
	FOR(frameCount)
	{
		clipNode.transforms[i].pos = CalcInterpolationVector(keyData.positions, posCount, i);
		clipNode.transforms[i].rot = CalcInterpolationQuat(keyData.rotations, rotCount, i);
		clipNode.transforms[i].scale = CalcInterpolationVector(keyData.scales, scaleCount, i);
	}
}

Float3 ModelExporter::CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame)
{
	// Ű������ �������� ũ�Ⱑ count���� �۰ų� ���ų�, Ű������ �����Ͱ� 1�� ������ ���,
	// ���� ������ Ű�������� ������ ��ȯ�մϴ�.
	if (keyData.size() <= count || keyData.size() == 1)
		return keyData.back().value;

	KeyVector curValue = keyData[count];
	KeyVector nextValue = curValue;

	// ���� Ű�������� �����ϴ� ���, nextValue�� ���� Ű������ ������ �����մϴ�.
	if (keyData.size() > count + 1)
		nextValue = keyData[count + 1];

	// ���� �������� Ű������ ���̿� ��ġ�� ������ ����մϴ�.
	float t = ((float)curFrame - curValue.time) / (nextValue.time - curValue.time);

	// ���� �������� ���� Ű�����Ӱ� ��ġ�ϴ� ���, ���� Ű���������� �Ѿ�ϴ�.
	if (curFrame == (int)nextValue.time)
		count++;

	// ���� ������ ���� ���� �������� ��ġ ���� ��ȯ�մϴ�.
	return MATH->Lerp(curValue.value, nextValue.value, t);
}

Float4 ModelExporter::CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame)
{
	// Ű������ �������� ũ�Ⱑ count���� �۰ų� ���ų�, Ű������ �����Ͱ� 1�� ������ ���,
	// ���� ������ Ű�������� ������ ��ȯ�մϴ�.
	if (keyData.size() <= count || keyData.size() == 1)
		return keyData.back().value;

	KeyQuat curValue = keyData[count];
	KeyQuat nextValue = curValue;

	// ���� Ű�������� �����ϴ� ���, nextValue�� ���� Ű������ ������ �����մϴ�.
	if (keyData.size() > count + 1)
		nextValue = keyData[count + 1];

	// ���� �������� Ű������ ���̿� ��ġ�� ������ ����մϴ�.
	float t = ((float)curFrame - curValue.time) / (nextValue.time - curValue.time);

	// ���� �������� ���� Ű�����Ӱ� ��ġ�ϴ� ���, ���� Ű���������� �Ѿ�ϴ�.
	if (curFrame == (int)nextValue.time)
		count++;

	// ���ʹϾ��� ���� ������ �����ϰ� ����� ��ȯ�մϴ�.
	Vector4 cur = XMLoadFloat4(&curValue.value);
	Vector4 next = XMLoadFloat4(&nextValue.value);

	Vector4 rot = XMQuaternionSlerp(cur, next, t);

	Float4 result;
	XMStoreFloat4(&result, rot);

	return result;
}