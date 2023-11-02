#include "Framework.h"

ModelExporter::ModelExporter(string name, string file)
	:	name(name)
{
	importer = new Assimp::Importer();

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

void ModelExporter::ReadMaterial()
{
	FOR(scene->mNumMaterials)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		Material* material = new Material();

		material->SetName(srcMaterial->GetName().C_Str());

		MaterialBuffer::Data* data = material->GetBuffer();

		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);// 난반사 컬러 설정
		data->diffuse = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);// 스펙큘러
		data->specular = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);// 엠비언트
		data->ambient = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);// 자체발광
		data->emissive = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, data->shininess); // 빛 반사의 날카로움.

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

	// 배열 저장시 무조건 배열 길이부터 저장해야한다.
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
