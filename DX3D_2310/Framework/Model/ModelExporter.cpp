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
		aiMesh* srcMesh = scene->mMeshes[i];

		// Mesh의 재질(Material) 인덱스를 MeshData 객체에 할당
		mesh->materialIndex = srcMesh->mMaterialIndex;

		// Mesh의 정점(Vertex) 데이터를 복사
		mesh->vertices.resize(srcMesh->mNumVertices);

		FOR(srcMesh->mNumVertices)
		{
			ModelVertex vertex;

			// 정점 위치 정보를 복사
			memcpy(&vertex.pos, &srcMesh->mVertices[i], sizeof(Float3));

			// 텍스처 좌표 정보가 있는 경우, 복사
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][i], sizeof(Float2));

			// 법선 벡터 정보가 있는 경우, 복사
			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[i], sizeof(Float3));

			// 접선 벡터 및 이접선 벡터 정보가 있는 경우, 복사
			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[i], sizeof(Float3));

			// MeshData의 정점 목록에 현재 정점 정보를 추가
			mesh->vertices[i] = vertex;
		}

		// Mesh의 인덱스 데이터를 복사 복사하기위해 배열을 resize로 늘려줌.
		mesh->indices.resize(srcMesh->mNumFaces * 3);
		FOR(srcMesh->mNumFaces)
		{
			// face = 그리기순서 얼굴, 면 이라고 생각하면됨.
			aiFace& face = srcMesh->mFaces[i];

			// 각 면(Face)의 인덱스를 MeshData 객체의 인덱스 목록에 추가
			for (UINT j = 0; j < face.mNumIndices; j++)
			{
				mesh->indices[i * 3 + j] = face.mIndices[j];
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