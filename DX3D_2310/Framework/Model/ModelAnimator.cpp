#include "Framework.h"

ModelAnimator::ModelAnimator(string name) : Model(name)
{
    frameBuffer = new FrameBuffer();

    SetShader(L"Model/Model.hlsl");
}

ModelAnimator::~ModelAnimator()
{
    for (ModelClip* clip : clips)
        delete clip;

    delete frameBuffer;

    delete[] clipTransforms;
    delete[] nodeTransforms;

    texture->Release();
    srv->Release();
}

void ModelAnimator::Update()
{
    int clip = frameBuffer->GetData()->clip;
    frameBuffer->GetData()->curFrame = (int)nowFrame;
    frameBuffer->GetData()->nextcurFrame = ((int)nowFrame + 1) % (clips[clip]->frameCount - 1);
    frameBuffer->GetData()->time = nowFrame - frameBuffer->GetData()->curFrame;

    if (AutoAnimation)
    {
        nowFrame += DELTA * clips[clip]->tickPerSecond * frameBuffer->GetData()->scale;
    }
    if (nowFrame > clips[clip]->frameCount - 1)
        nowFrame = 0;

    UpdateWorld();
}

void ModelAnimator::Render()
{
    frameBuffer->SetVS(3);
    DC->VSSetShaderResources(0, 1, &srv);

    Model::Render();
}

void ModelAnimator::GUIRender()
{
    int clip = frameBuffer->GetData()->clip;

    ImGui::SliderInt("Clip", (int*)&frameBuffer->GetData()->clip, 0, clips.size() - 1);
    ImGui::SliderFloat("Frame", &nowFrame, 0, clips[clip]->frameCount - 1);
    ImGui::SliderFloat("Speed", &frameBuffer->GetData()->scale, 0, 3.0f);

    ImGui::Text(to_string((clips[clip]->frameCount - 1)).c_str());
    ImGui::Text(to_string(frameBuffer->GetData()->nextcurFrame).c_str());
    ImGui::Text(to_string(nowFrame - frameBuffer->GetData()->curFrame).c_str());
    ImGui::Checkbox("AutoAnimation", &AutoAnimation);

    __super::GUIRender();
}

void ModelAnimator::ReadClip(string clipName, UINT clipNum, UINT count)
{
    // 재시도 횟수가 2보다 작거나 같은지 확인
    assert(count <= 2);

    // 클립 파일의 경로 구성
    string file = "Models/Clips/" + name + "/" + clipName + to_string(clipNum) + ".clip";

    // BinaryReader 객체를 사용하여 클립 파일 읽기 시도
    BinaryReader* reader = new BinaryReader(file);

    // 클립 파일 읽기에 실패하면서 재시도 횟수가 남아 있을 경우
    if (reader->IsFailed())
    {
        // FBX 파일에서 클립을 내보내기 위한 ModelExporter 객체 생성
        file = "Models/Animations/" + name + "/" + clipName + ".fbx";
        ModelExporter* exporter = new ModelExporter(name, file);

        // 클립 내보내기 수행
        exporter->ExportClip(clipName);
        delete exporter; // ModelExporter 객체 해제

        // 재귀적으로 ReadClip 함수 호출하여 클립을 다시 시도
        ReadClip(clipName, clipNum, count + 1);
        return;
    }

    // 성공적으로 클립 파일을 읽어왔을 때의 처리
    ModelClip* clip = new ModelClip(); // 새로운 ModelClip 객체 생성
    clip->name = reader->String(); // 클립의 이름 설정
    clip->frameCount = reader->Int(); // 클립의 프레임 수 설정
    clip->tickPerSecond = reader->Float(); // 클립의 초당 틱 수 설정

    UINT boneCount = reader->UInt(); // 클립에 포함된 본의 수 설정
    FOR(boneCount)
    {
        KeyFrame* keyFrame = new KeyFrame(); // 새로운 KeyFrame 객체 생성

        keyFrame->boneName = reader->String(); // 키프레임에 속한 본의 이름 설정
        UINT size = reader->UInt(); // 키프레임의 변환 매트릭스 크기 설정

        if (size > 0)
        {
            keyFrame->transforms.resize(size); // 변환 매트릭스 배열 크기 설정

            // 변환 매트릭스 데이터를 바이트 단위로 읽어와서 keyFrame->transforms에 저장
            void* ptr = (void*)keyFrame->transforms.data();
            reader->Byte(&ptr, sizeof(KeyTransform) * size);
        }

        // 클립의 키프레임 맵에 현재 키프레임 추가
        clip->keyFrames[keyFrame->boneName] = keyFrame;
    }

    // 읽어온 클립을 모델 애니메이터의 클립 벡터에 추가
    clips.push_back(clip);

    delete reader; // BinaryReader 객체 해제
}

void ModelAnimator::CreateTexture()
{
    // 애니메이션 클립의 개수를 얻습니다.
    UINT clipCount = clips.size();

    // clipTransforms 및 nodeTransforms 배열에 메모리를 할당합니다.
    clipTransforms = new ClipTransform[clipCount];
    nodeTransforms = new ClipTransform[clipCount];

    // 각 애니메이션 클립에 대해 clipTransforms 및 nodeTransforms를 생성합니다.
    FOR(clipCount)
        CreateClipTransform(i);

    // 텍스처에 대한 설명을 설정합니다.
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = MAX_BONE * 4;
    desc.Height = MAX_FRAME;
    desc.MipLevels = 1;
    desc.ArraySize = clipCount;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    UINT pitchSize = MAX_BONE * sizeof(Matrix);
    UINT pageSize = pitchSize * MAX_FRAME;

    // 가상 메모리를 할당하여 텍스처 데이터를 저장할 공간을 만듭니다.
    void* p = VirtualAlloc(nullptr, pageSize * clipCount,
        MEM_RESERVE, PAGE_READWRITE);

    // 각 클립에 대해 텍스처 데이터를 생성합니다.
    FOR(clipCount)
    {
        UINT start = i * pageSize;

        for (UINT y = 0; y < MAX_FRAME; y++)
        {
            void* temp = (BYTE*)p + pitchSize * y + start;

            // 텍스처 데이터를 메모리에 복사합니다.
            VirtualAlloc(temp, pitchSize, MEM_COMMIT, PAGE_READWRITE);
            memcpy(temp, clipTransforms[i].transform[y], pitchSize);
        }
    }

    // D3D11_SUBRESOURCE_DATA 배열을 생성하여 텍스처 데이터를 담습니다.
    D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

    FOR(clipCount)
    {
        void* temp = (BYTE*)p + i * pageSize;

        // D3D11_SUBRESOURCE_DATA에 텍스처 데이터 및 크기 정보를 설정합니다.
        subResource[i].pSysMem = temp;
        subResource[i].SysMemPitch = pitchSize;
        subResource[i].SysMemSlicePitch = pageSize;
    }

    // 텍스처를 생성합니다.
    DEVICE->CreateTexture2D(&desc, subResource, &texture);

    // 할당된 자원을 정리합니다.
    delete[] subResource;
    VirtualFree(p, 0, MEM_RELEASE);

    // 셰이더 리소스 뷰를 생성합니다.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = 1;
    srvDesc.Texture2DArray.ArraySize = clipCount;

    DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
    // 클립의 정보를 가져옵니다.
    ModelClip* clip = clips[index];

    // 클립의 프레임 수만큼 반복
    FOR(clip->frameCount)
    {
        UINT nodeIndex = 0;

        // 모든 노드에 대해 반복
        for (NodeData node : nodes)
        {
            Matrix animation;
            // 현재 프레임에서 노드의 키프레임을 가져옵니다.
            KeyFrame* frame = clip->GetKeyFrame(node.name);

            // 키프레임이 존재하면 해당 키프레임의 변환 정보를 사용하여 변환 매트릭스 생성
            if (frame != nullptr)
            {
                KeyTransform& transform = frame->transforms[i];

                // XMMatrixTransformation 함수를 사용하여 변환 매트릭스 생성
                animation = XMMatrixTransformation(
                    XMVectorZero(),            // 스케일 중심점
                    XMQuaternionIdentity(),    // 회전을 나타내는 쿼터니언
                    Vector3(transform.scale),  // 스케일 벡터
                    XMVectorZero(),            // 회전 중심점
                    XMLoadFloat4(&transform.rot),   // 회전 쿼터니언을 로드
                    Vector3(transform.pos)     // 이동 벡터
                );
            }
            else
            {
                // 키프레임이 없으면 변환 매트릭스를 단위 행렬로 초기화
                animation = XMMatrixIdentity();
            }

            // 부모 노드의 변환 매트릭스를 가져옵니다.
            Matrix parent;
            int parentIndex = node.parent;

            if (parentIndex < 0)
                parent = XMMatrixIdentity();
            else
                parent = nodeTransforms[index].transform[i][parentIndex];

            // 현재 노드의 변환 매트릭스를 계산하여 저장
            nodeTransforms[index].transform[i][nodeIndex] = animation * parent;

            // 현재 노드가 본에 해당한다면 클립 변환 매트릭스에도 저장
            int boneIndex = -1;
            if (boneMap.count(node.name))
                boneIndex = boneMap[node.name];

            if (boneIndex >= 0)
            {
                Matrix offset = bones[boneIndex].offset;
                offset *= nodeTransforms[index].transform[i][nodeIndex];

                clipTransforms[index].transform[i][boneIndex] = offset;
            }
            nodeIndex++;
        }
    }
}