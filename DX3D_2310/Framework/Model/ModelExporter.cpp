#include "Framework.h"

ModelExporter::ModelExporter(string name, string file)
	:	name(name)
{
	importer = new Assimp::Importer();

	importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

	// 왼손자표계 컨버트 | 최대퀄리티로
	scene = importer->ReadFile(file,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

	// fbx가 없을경우, 정보가 잘못되었을 경우 터트리는게 더 낫다.
	assert(scene != nullptr);
	/*
		NumVertices = 버텍스 수
		NumFaces = 삼각형 그리는 수
		Tangents = 데이터 노멀맵
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
	// rootnode가-1임을 기억하자.
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
		// FBX 파일에서 Material 데이터를 가져옵니다.
		aiMaterial* srcMaterial = scene->mMaterials[i];

		// Material 클래스의 인스턴스를 생성합니다.
		Material* material = new Material();

		// Material의 이름을 설정 (Material의 이름은 FBX에서 가져옵니다).
		material->SetName(srcMaterial->GetName().C_Str());

		// MaterialBuffer 클래스를 통해 Material의 데이터에 접근합니다.
		MaterialBuffer::Data* data = material->GetBuffer();

		// Material의 난반사 (Diffuse) 컬러 설정
		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		data->diffuse = Float4(color.r, color.g, color.b, 1.0f);

		// Material의 스펙큘러 (Specular) 컬러 설정
		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		data->specular = Float4(color.r, color.g, color.b, 1.0f);

		// Material의 엠비언트 (Ambient) 컬러 설정
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		data->ambient = Float4(color.r, color.g, color.b, 1.0f);

		// Material의 자체발광 (Emissive) 컬러 설정
		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		data->emissive = Float4(color.r, color.g, color.b, 1.0f);

		// Material의 빛 반사의 날카로움 (Shininess) 설정
		srcMaterial->Get(AI_MATKEY_SHININESS, data->shininess);

		// Material의 난반사 (Diffuse) 텍스처 설정
		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		// Material의 스펙큘러 (Specular) 텍스처 설정
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		// Material의 법선 (Normals) 맵 텍스처 설정
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		// Material 객체를 materials 벡터에 추가
		materials.push_back(material);
	}
}

void ModelExporter::WriterMaterial()
{
	// Material 데이터를 저장할 디렉터리 경로 생성 (예: "Models/Materials/모델이름/")
	string savePath = "Models/Materials/" + name + "/";

	// Material 데이터를 저장할 파일 이름 생성 (예: "모델이름.mats")
	string file = name + ".mats";

	// 필요한 폴더 구조를 생성하는 함수 호출
	CreateFolders(savePath);

	// BinaryWriter를 사용하여 파일을 생성하고 열기
	BinaryWriter* writer = new BinaryWriter(savePath + file);

	// Material 데이터의 개수를 파일에 쓰기 (materials 벡터의 크기)
	writer->UInt(materials.size());

	// materials 벡터에 있는 각 Material 객체를 처리
	for (Material* material : materials)
	{
		// Material을 저장할 파일 경로 생성 (예: "Models/Materials/모델이름/재질이름.mat")
		string path = savePath + material->GetName() + ".mat";

		// Material 객체를 해당 경로에 저장
		material->Save(path);

		// Material 파일의 경로를 파일에 문자열 형태로 쓰기
		writer->String(path);

		// 현재 처리한 Material 객체를 삭제 (메모리 해제)
		delete material;
	}

	// materials 벡터를 비우고, BinaryWriter를 닫고 삭제
	materials.clear();
	delete writer;
}

string ModelExporter::CreateTexture(string file)
{
	// 입력된 파일명의 길이가 0일 경우, 빈 문자열("")을 반환한다.
	if (file.length() == 0)
		return "";
	
	// 입력된 파일명에서 확장자를 제외한 부분을 가져와서 ".png확장자를 붙여준다. 원래 확장자는 무시한다.
	string fileName = GetFileNameWithoutExtension(file) + ".png"; // 확장자 설정
	string path = "Textures/Model/" + name + "/" + fileName;

	// 경로에 해당하는 폴더를 생성하는 함수를 호출
	CreateFolders(path); // 폴더 만들기

	// Assimp라이브러리를 사용하여 입력된 파일명에 해당하는 내장 텍스처를 긁어온다.
	const aiTexture* srcTexture = scene->GetEmbeddedTexture(file.c_str());

	// 내장 텍스처가 존재하지 않는경우 빈 문자열("")을 반환한다.
	// 프레임워크상 흰 텍스쳐가 적용된다.
	if (srcTexture == nullptr)
		return "";

	// 내장텍스처의 높이가 1보다 작은경우, 바이너리파일로 저장한다
	// 내장텍스처의 높이가 1보다 클 경우, 3D텍스처를 의미한다.
	// 일반적인 텍스처 와는 다르게 3D공간에 지정하여 렌더링해준다.
	if (srcTexture->mHeight < 1)
	{
		BinaryWriter w(path);
		w.Byte(srcTexture->pcData, srcTexture->mWidth);
		// 그냥 그대로 한줄씩 읽어버린뒤에 Byte로 밀어서 저장해버리면 png로 읽으면 읽어진다!
	}
	else
	{
		// 이미지 객체를 생성하고 텍스처 데이터를 이 객체에 저장한다.
		Image image; 
		image.width = srcTexture->mWidth; // 이미지의 너비크기(가로픽셀수)
		image.height = srcTexture->mHeight; // 이미지의 높이크기(세로픽셀수)
		image.pixels = (uint8_t*)(srcTexture->pcData); // 이미지데이터의 포인터 설정

		// 이미지 데이터의 각 행(row)이 차지하는 바이트 수를 설정.
		// 4는 일반적으로 RGBA색상 형식을 나타낸다. 각 픽셀당 4바이트를 사용.
		image.rowPitch = image.width * 4;

		// 이미지 데이터의 전체 크기를 나타내는 값으로 너비 * 높이 * 4바이트를 계산하여 설정한다.
		// 이미지 전체 데이터이의 크기를 나타낸다.
		image.slicePitch = image.width * image.height * 4;

		// WIC를 사용하여 이미지를 파일로 저장한다.
		SaveToWICFile(
			image,						// 1. 이미지 데이터를 저장하는 Image 구조체
			WIC_FLAGS_NONE,				// 2. WIC에 대한 플래그 설정. 여기서는 아무 특별한 플래그가 사용되지 않음을 나타낸다.
			GetWICCodec(WIC_CODEC_PNG), // 3. WIC 코덱을 얻어온다. 이 코드에서는 PNG 형식을 사용하여 이미지를 저장.
			ToWString(path).c_str()		// 4. 이미지를 저장할 파일 경로를 유니코드 문자열로 변환하여 전달한다.
		);
	}

	return path;
}

// 뼈대에 대한 보간정보 뽑기.
void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
	FOR(mesh->mNumBones)
	{
		UINT boneIndex = 0;
		string name = mesh->mBones[i]->mName.C_Str();

		// 뼈대가 이미 있는지 확인
		if (boneMap.count(name) == 0)
		{
			boneIndex = boneCount++;
			boneMap[name] = boneIndex;

			BoneData* boneData = new BoneData();
			boneData->name = name;
			boneData->index = boneIndex;

			Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);// 배열로 제공하기는 하지만, 통상적으로 맨 앞의 값만 쓰게 됨.
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
	nodeData->transform = XMMatrixTranspose(matrix); // 메트릭스 변환해야함.

	nodes.push_back(nodeData);

	FOR(node->mNumChildren)
		ReadNode(node->mChildren[i], nodes.size(), index);
}

void ModelExporter::ReadMesh(aiNode* node)
{
	FOR(node->mNumMeshes)
	{
		// 새로운 MeshData 객체를 생성
		MeshData* mesh = new MeshData();

		// Mesh의 이름을 설정, node->mName.C_Str()는 C 문자열로 Mesh의 이름을 제공합니다.
		mesh->name = node->mName.C_Str();

		// 현재 처리 중인 Mesh에 대한 포인터를 가져옵니다.
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];


		// Mesh의 재질(Material) 인덱스를 MeshData 객체에 할당
		mesh->materialIndex = srcMesh->mMaterialIndex;
		vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
		ReadBone(srcMesh, vertexWeights);


		// Mesh의 정점(Vertex) 데이터를 복사
		mesh->vertices.resize(srcMesh->mNumVertices);

		for(UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			ModelVertex vertex;

			// 정점 위치 정보를 복사
			memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

			// 텍스처 좌표 정보가 있는 경우, 복사
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

			// 법선 벡터 정보가 있는 경우, 복사
			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

			// 접선 벡터 및 이접선 벡터 정보가 있는 경우, 복사
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
			// MeshData의 정점 목록에 현재 정점 정보를 추가
			mesh->vertices[v] = vertex;
		}

		// Mesh의 인덱스 데이터를 복사 복사하기위해 배열을 resize로 늘려줌.
		mesh->indices.resize(srcMesh->mNumFaces * 3);
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			// face = 그리기순서 얼굴, 면 이라고 생각하면됨.
			aiFace& face = srcMesh->mFaces[f];

			// 각 면(Face)의 인덱스를 MeshData 객체의 인덱스 목록에 추가
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				mesh->indices[f * 3 + j] = face.mIndices[j];
			}
		}

		// MeshData 객체를 meshes 벡터에 추가
		meshes.push_back(mesh);
	}

	FOR(node->mNumChildren)
	{
		// 현재 노드의 자식 노드에 대해 재귀적으로 Mesh를 읽고 처리 메쉬는 트리형태로 되어있기에 재귀로 읽어야함.
		ReadMesh(node->mChildren[i]);
	}
}

void ModelExporter::WriteMesh()
{
	// 저장할 파일 경로를 구성 (예: "Models/Meshes/모델이름.mesh")
	string path = "Models/Meshes/" + name + ".mesh";

	// 필요한 폴더 구조를 생성하는 함수 호출
	CreateFolders(path);

	// BinaryWriter를 사용하여 파일을 생성하고 열기
	BinaryWriter* writer = new BinaryWriter(path);

	// Mesh 데이터의 개수를 파일에 쓰기 (meshes 벡터의 크기)
	writer->UInt(meshes.size());

	/*
		미리 데이터 개수를 쓰는 이유,
		구조화된 데이터를 관리하는데 더 효율적이기 때문,
		파일의 구조상 헤더에 데이터 범위를 미리 적는것처럼
	*/

	// meshes 벡터에 있는 각 MeshData 객체를 처리
	for (MeshData* mesh : meshes)
	{
		// Mesh의 이름을 파일에 문자열 형태로 쓰기
		writer->String(mesh->name);

		// Mesh의 재질(Material) 인덱스를 파일에 쓰기
		writer->UInt(mesh->materialIndex);

		// Mesh의 정점 데이터 개수를 파일에 쓰기
		writer->UInt(mesh->vertices.size());

		// Mesh의 정점 데이터를 파일에 이진 형태로 쓰기 (정점 구조체의 배열)
		writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

		// Mesh의 인덱스 데이터 개수를 파일에 쓰기
		writer->UInt(mesh->indices.size());

		// Mesh의 인덱스 데이터를 파일에 이진 형태로 쓰기 (정수 배열)
		writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		// 현재 처리한 MeshData 객체를 삭제 (메모리 해제)
		delete mesh;
	}
	// 어차피 정점데이터는 읽어도 파악이 힘들기 때문에, 이진데이터 형태로 압축률이 뛰어나게 저장한다.
	// meshes 벡터를 비우고, BinaryWriter를 닫고 삭제
	meshes.clear();
	
	// 갹 노드의 데이터를 작성해서 저장한다.
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

	// 갹 뼈대의 데이터를 작성해서 저장한다.
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

/*
*	언리얼 부분에서 사용가능
Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
	// 새로운 Clip 객체를 동적으로 생성합니다.
	Clip* clip = new Clip();

	// 애니메이션의 이름을 Clip 객체에 할당합니다.
	clip->name = animation->mName.C_Str();

	// 애니메이션의 초당 틱 수를 Clip 객체에 할당합니다.
	clip->tickPerSecond = (float)animation->mTicksPerSecond;

	// 애니메이션의 총 프레임 수를 Clip 객체에 할당합니다.
	clip->frameCount = (UINT)(animation->mDuration) + 1;

	// 애니메이션 채널(노드)의 수만큼 ClipNode 객체를 담을 벡터를 할당합니다.
	vector<ClipNode> clipNodes;
	clipNodes.reserve(animation->mNumChannels);

	// 각 애니메이션 채널(노드)에 대해 반복합니다.
	FOR(animation->mNumChannels)
	{
		// 현재 채널(노드)을 가져옵니다.
		aiNodeAnim* srcNode = animation->mChannels[i];

		// 새로운 ClipNode 객체를 생성하고 이름을 설정합니다.
		ClipNode node;
		node.name = srcNode->mNodeName;

		// KeyData 객체를 생성하여 각 키프레임의 위치, 회전, 스케일을 저장합니다.
		KeyData data;

		// 위치 키프레임 정보 저장
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

		// 회전 키프레임 정보 저장
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

		// 스케일 키프레임 정보 저장
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

		// SetClipNode 함수를 통해 키프레임 데이터를 ClipNode에 설정합니다.
		SetClipNode(data, clip->frameCount, node);

		// 현재 처리한 ClipNode을 벡터에 추가합니다.
		clipNodes.push_back(node);
	}

	// 읽어온 키프레임 데이터를 이용하여 애니메이션의 프레임 데이터를 설정합니다.
	ReadKeyFrame(clip, scene->mRootNode, clipNodes);

	// 생성한 Clip 객체를 반환합니다.
	return clip;
}
*/

Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
	// 새로운 Clip 객체를 동적으로 생성합니다.
	Clip* clip = new Clip();

	// 애니메이션의 이름을 Clip 객체에 할당합니다.
	clip->name = animation->mName.C_Str();

	// 애니메이션의 초당 틱 수를 Clip 객체에 할당합니다.
	clip->tickPerSecond = (float)animation->mTicksPerSecond;

	// 애니메이션의 총 프레임 수를 Clip 객체에 할당합니다.
	clip->frameCount = (UINT)(animation->mDuration) + 1;

	// 애니메이션 채널(노드)의 수만큼 ClipNode 객체를 담을 벡터를 할당합니다.
	vector<ClipNode> clipNodes;
	clipNodes.reserve(animation->mNumChannels);

	// 각 애니메이션 채널(노드)에 대해 반복합니다.
	FOR(animation->mNumChannels)
	{
		// 현재 채널(노드)을 가져옵니다.
		aiNodeAnim* srcNode = animation->mChannels[i];

		// 새로운 ClipNode 객체를 생성하고 이름을 설정합니다.
		ClipNode node;
		node.name = srcNode->mNodeName;

		KeyTransform transform;

		for (UINT k = 0; k < clip->frameCount; k++)
		{
			bool isFound = false;
			float t = node.transforms.size();
			// 포지션 키의 크기. k가 포지션키보다 크면 배열 벗어났다는 으미
			if(k < srcNode->mNumPositionKeys &&
				MATH->NearlyEqual((float)srcNode->mPositionKeys[k].mTime, t))
			{
				aiVectorKey key = srcNode->mPositionKeys[k];
				memcpy_s(&transform.pos, sizeof(Float3),
					&key.mValue, sizeof(aiVector3D));

				isFound = true; // 데이터가 하나라도 존재하는지 검사.
			}

			if (k < srcNode->mNumRotationKeys &&
				MATH->NearlyEqual((float)srcNode->mRotationKeys[k].mTime, t))
			{
				aiQuatKey key = srcNode->mRotationKeys[k];

				transform.rot.x = (float)key.mValue.x;
				transform.rot.y = (float)key.mValue.y;
				transform.rot.z = (float)key.mValue.z;
				transform.rot.w = (float)key.mValue.w;

				isFound = true; // 데이터가 하나라도 존재하는지 검사.
			}

			if (k < srcNode->mNumScalingKeys &&
				MATH->NearlyEqual((float)srcNode->mScalingKeys[k].mTime, t))
			{
				aiVectorKey key = srcNode->mScalingKeys[k];
				memcpy_s(&transform.scale, sizeof(Float3),
					&key.mValue, sizeof(aiVector3D));

				isFound = true; // 데이터가 하나라도 존재하는지 검사.
			}

			if (isFound)
				node.transforms.push_back(transform);
		}
		if (node.transforms.size() < clip->frameCount)
		{
			// 빠진 갯수가 여기에 들어간다. 비어있는 부분의 개수가
			UINT count = clip->frameCount - node.transforms.size();
			KeyTransform keyTransform = node.transforms.back();

			// 빠진 갯수만큼 보정해서 넣어준다.
			for(UINT n = 0; n < count; n++)
			{
				node.transforms.push_back(keyTransform);
			}
		}

		clipNodes.push_back(node);
	}
	// 읽어온 키프레임 데이터를 이용하여 애니메이션의 프레임 데이터를 설정합니다.
	ReadKeyFrame(clip, scene->mRootNode, clipNodes);

	// 생성한 Clip 객체를 반환합니다.
	return clip;
}

void ModelExporter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes)
{
	// 키프레임 정보 하나 만들기
	KeyFrame* keyFrame = new KeyFrame();
	keyFrame->boneName = node->mName.C_Str();
	keyFrame->transforms.reserve(clip->frameCount);

	// 클립의 각 프레임에 대한 처리
	FOR(clip->frameCount)
	{
		ClipNode* clipNode = nullptr;

		// 현재 노드가 클립 노드 벡터에 있는지 확인
		for (ClipNode& temp : clipNodes)
		{
			if (temp.name == node->mName)
			{
				clipNode = &temp;
				break;
			}
		}

		// 키프레임의 변환 정보를 저장할 구조체 생성
		KeyTransform keyTransform;

		// 클립 노드 벡터에 현재 노드가 없는 경우
		if (clipNode == nullptr)
		{
			// 현재 노드의 전역 변환 행렬을 읽어와서 분해
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
			// 클립 노드 벡터에 현재 노드가 있는 경우 해당 노드의 변환 정보 사용
			keyTransform = clipNode->transforms[i];
		}

		// 키프레임에 변환 정보 추가
		keyFrame->transforms.push_back(keyTransform);
	}

	// 생성한 키프레임을 클립에 추가
	clip->keyFrames.push_back(keyFrame);

	// 노드의 자식에 대해 재귀적으로 처리
	FOR(node->mNumChildren)
		ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}

void ModelExporter::WriteClip(Clip* clip, string clipName, UINT index)
{
	// 클립을 저장할 파일 경로 생성
	string file = "Models/Clips/" + name + "/" + clipName + to_string(index) + ".clip";

	// 폴더가 존재하지 않으면 생성
	CreateFolders(file);

	// BinaryWriter 객체를 사용하여 파일에 쓰기
	BinaryWriter* writer = new BinaryWriter(file);

	// 클립 정보 기록
	writer->String(clip->name);
	writer->UInt(clip->frameCount);
	writer->Float(clip->tickPerSecond);

	// 키프레임 개수 및 각 키프레임 정보 기록
	writer->UInt(clip->keyFrames.size());
	for (KeyFrame* keyFrame : clip->keyFrames)
	{
		writer->String(keyFrame->boneName);
		writer->UInt(keyFrame->transforms.size());
		writer->Byte(keyFrame->transforms.data(), sizeof(KeyTransform) * keyFrame->transforms.size());

		// 사용이 완료된 키프레임 객체 삭제
		delete keyFrame;
	}

	// 사용이 완료된 클립 객체 삭제
	delete clip;

	// BinaryWriter 객체 삭제
	delete writer;
}

void ModelExporter::SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode)
{
	// 클립 노드에 키프레임의 변환(transform)을 저장할 벡터를 프레임 수만큼 할당합니다.
	clipNode.transforms.resize(frameCount);

	UINT posCount = 0;
	UINT rotCount = 0;
	UINT scaleCount = 0;

	// 각 프레임에 대해 보간된 위치, 회전, 스케일 값을 계산하여 클립 노드에 저장합니다.
	FOR(frameCount)
	{
		clipNode.transforms[i].pos = CalcInterpolationVector(keyData.positions, posCount, i);
		clipNode.transforms[i].rot = CalcInterpolationQuat(keyData.rotations, rotCount, i);
		clipNode.transforms[i].scale = CalcInterpolationVector(keyData.scales, scaleCount, i);
	}
}

Float3 ModelExporter::CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame)
{
	// 키프레임 데이터의 크기가 count보다 작거나 같거나, 키프레임 데이터가 1개 이하인 경우,
	// 가장 마지막 키프레임의 값으로 반환합니다.
	if (keyData.size() <= count || keyData.size() == 1)
		return keyData.back().value;

	KeyVector curValue = keyData[count];
	KeyVector nextValue = curValue;

	// 다음 키프레임이 존재하는 경우, nextValue를 다음 키프레임 값으로 설정합니다.
	if (keyData.size() > count + 1)
		nextValue = keyData[count + 1];

	// 현재 프레임이 키프레임 사이에 위치한 비율을 계산합니다.
	float t = ((float)curFrame - curValue.time) / (nextValue.time - curValue.time);

	// 현재 프레임이 다음 키프레임과 일치하는 경우, 다음 키프레임으로 넘어갑니다.
	if (curFrame == (int)nextValue.time)
		count++;

	// 선형 보간을 통해 현재 프레임의 위치 값을 반환합니다.
	return MATH->Lerp(curValue.value, nextValue.value, t);
}

Float4 ModelExporter::CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame)
{
	// 키프레임 데이터의 크기가 count보다 작거나 같거나, 키프레임 데이터가 1개 이하인 경우,
	// 가장 마지막 키프레임의 값으로 반환합니다.
	if (keyData.size() <= count || keyData.size() == 1)
		return keyData.back().value;

	KeyQuat curValue = keyData[count];
	KeyQuat nextValue = curValue;

	// 다음 키프레임이 존재하는 경우, nextValue를 다음 키프레임 값으로 설정합니다.
	if (keyData.size() > count + 1)
		nextValue = keyData[count + 1];

	// 현재 프레임이 키프레임 사이에 위치한 비율을 계산합니다.
	float t = ((float)curFrame - curValue.time) / (nextValue.time - curValue.time);

	// 현재 프레임이 다음 키프레임과 일치하는 경우, 다음 키프레임으로 넘어갑니다.
	if (curFrame == (int)nextValue.time)
		count++;

	// 쿼터니언의 선형 보간을 수행하고 결과를 반환합니다.
	Vector4 cur = XMLoadFloat4(&curValue.value);
	Vector4 next = XMLoadFloat4(&nextValue.value);

	Vector4 rot = XMQuaternionSlerp(cur, next, t);

	Float4 result;
	XMStoreFloat4(&result, rot);

	return result;
}