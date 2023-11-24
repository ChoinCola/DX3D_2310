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
    UpdateFrame();
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
    int clip = frameBuffer->GetData()->cur.clip;

    ImGui::SliderInt("Clip", &frameBuffer->GetData()->next.clip, 0, clips.size() - 1);
    ImGui::SliderInt("Frame", &frameBuffer->GetData()->cur.curFrame, 0, clips[clip]->frameCount - 1);
    ImGui::SliderFloat("Speed", &frameBuffer->GetData()->cur.scale, 0, 3.0f);
    ImGui::SliderFloat("Animation Transtime", &frameBuffer->GetData()->takeTime, 0, 1.0f);

    ImGui::Checkbox("AutoAnimation", &IsPlay);

    __super::GUIRender();
}

void ModelAnimator::ReadClip(string clipName, UINT clipNum, UINT count)
{
    // ��õ� Ƚ���� 2���� �۰ų� ������ Ȯ��
    assert(count <= 2);

    // Ŭ�� ������ ��� ����
    string file = "Models/Clips/" + name + "/" + clipName + to_string(clipNum) + ".clip";

    // BinaryReader ��ü�� ����Ͽ� Ŭ�� ���� �б� �õ�
    BinaryReader* reader = new BinaryReader(file);

    // Ŭ�� ���� �б⿡ �����ϸ鼭 ��õ� Ƚ���� ���� ���� ���
    if (reader->IsFailed())
    {
        // FBX ���Ͽ��� Ŭ���� �������� ���� ModelExporter ��ü ����
        file = "Models/Animations/" + name + "/" + clipName + ".fbx";
        ModelExporter* exporter = new ModelExporter(name, file);

        // Ŭ�� �������� ����
        exporter->ExportClip(clipName);
        delete exporter; // ModelExporter ��ü ����

        // ��������� ReadClip �Լ� ȣ���Ͽ� Ŭ���� �ٽ� �õ�
        ReadClip(clipName, clipNum, count + 1);
        return;
    }

    // ���������� Ŭ�� ������ �о���� ���� ó��
    ModelClip* clip = new ModelClip(); // ���ο� ModelClip ��ü ����
    clip->name = reader->String(); // Ŭ���� �̸� ����
    clip->frameCount = reader->Int(); // Ŭ���� ������ �� ����
    clip->tickPerSecond = reader->Float(); // Ŭ���� �ʴ� ƽ �� ����

    UINT boneCount = reader->UInt(); // Ŭ���� ���Ե� ���� �� ����
    FOR(boneCount)
    {
        KeyFrame* keyFrame = new KeyFrame(); // ���ο� KeyFrame ��ü ����

        keyFrame->boneName = reader->String(); // Ű�����ӿ� ���� ���� �̸� ����
        UINT size = reader->UInt(); // Ű�������� ��ȯ ��Ʈ���� ũ�� ����

        if (size > 0)
        {
            keyFrame->transforms.resize(size); // ��ȯ ��Ʈ���� �迭 ũ�� ����

            // ��ȯ ��Ʈ���� �����͸� ����Ʈ ������ �о�ͼ� keyFrame->transforms�� ����
            void* ptr = (void*)keyFrame->transforms.data();
            reader->Byte(&ptr, sizeof(KeyTransform) * size);
        }

        // Ŭ���� Ű������ �ʿ� ���� Ű������ �߰�
        clip->keyFrames[keyFrame->boneName] = keyFrame;
    }

    // �о�� Ŭ���� �� �ִϸ������� Ŭ�� ���Ϳ� �߰�
    clips.push_back(clip);

    delete reader; // BinaryReader ��ü ����
}

void ModelAnimator::CreateTexture()
{
    // �ִϸ��̼� Ŭ���� ������ ����ϴ�.
    UINT clipCount = clips.size();

    // clipTransforms �� nodeTransforms �迭�� �޸𸮸� �Ҵ��մϴ�.
    clipTransforms = new ClipTransform[clipCount];
    nodeTransforms = new ClipTransform[clipCount];

    // �� �ִϸ��̼� Ŭ���� ���� clipTransforms �� nodeTransforms�� �����մϴ�.
    FOR(clipCount)
        CreateClipTransform(i);

    // �ؽ�ó�� ���� ������ �����մϴ�.
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

    // ���� �޸𸮸� �Ҵ��Ͽ� �ؽ�ó �����͸� ������ ������ ����ϴ�.
    void* p = VirtualAlloc(nullptr, pageSize * clipCount,
        MEM_RESERVE, PAGE_READWRITE);

    // �� Ŭ���� ���� �ؽ�ó �����͸� �����մϴ�.
    FOR(clipCount)
    {
        UINT start = i * pageSize;

        for (UINT y = 0; y < MAX_FRAME; y++)
        {
            void* temp = (BYTE*)p + pitchSize * y + start;

            // �ؽ�ó �����͸� �޸𸮿� �����մϴ�.
            VirtualAlloc(temp, pitchSize, MEM_COMMIT, PAGE_READWRITE);
            memcpy(temp, clipTransforms[i].transform[y], pitchSize);
        }
    }

    // D3D11_SUBRESOURCE_DATA �迭�� �����Ͽ� �ؽ�ó �����͸� ����ϴ�.
    D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

    FOR(clipCount)
    {
        void* temp = (BYTE*)p + i * pageSize;

        // D3D11_SUBRESOURCE_DATA�� �ؽ�ó ������ �� ũ�� ������ �����մϴ�.
        subResource[i].pSysMem = temp;
        subResource[i].SysMemPitch = pitchSize;
        subResource[i].SysMemSlicePitch = pageSize;
    }

    // �ؽ�ó�� �����մϴ�.
    DEVICE->CreateTexture2D(&desc, subResource, &texture);

    // �Ҵ�� �ڿ��� �����մϴ�.
    delete[] subResource;
    VirtualFree(p, 0, MEM_RELEASE);

    // ���̴� ���ҽ� �並 �����մϴ�.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = 1;
    srvDesc.Texture2DArray.ArraySize = clipCount;

    DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

void ModelAnimator::PlayClip(int clip, float scale, float takeTime)
{
    IsPlay = true;

    frameBuffer->GetData()->next.clip = clip;
    frameBuffer->GetData()->next.scale = scale;
    frameBuffer->GetData()->takeTime = takeTime;

    clips[clip]->Init();
}

Matrix ModelAnimator::GetTransformByNode(int nodeIndex)
{

    Matrix curAnim;

    {
        Frame& frame = frameBuffer->GetData()->cur;

        Matrix cur = nodeTransforms[frame.clip].transform[frame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[frame.clip].transform[frame.curFrame + 1][nodeIndex];

        curAnim = MATH->Lerp(cur, next, frame.time) * world;
    }

    {
        Frame& frame = frameBuffer->GetData()->next;

        if (frame.clip < 0)
            return curAnim;

        Matrix cur = nodeTransforms[frame.clip].transform[frame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[frame.clip].transform[frame.curFrame + 1][nodeIndex];

        Matrix nextAnim = MATH->Lerp(cur, next, frame.time) * world;

        return MATH->Lerp(curAnim, nextAnim, frameBuffer->GetData()->tweenTime);
    }
}

Matrix ModelAnimator::GetTransformByNode(int nodeIndex)
{
    Matrix curAnim;

    {
        Frame& frame = frameBuffer->GetData()->cur;
        Matrix cur = nodeTransforms[frame.clip].transform[frame.curFrame][nodeIndex].GetMatrix();
        Matrix next = nodeTransforms[frame.clip].transform[frame.curFrame + 1][nodeIndex].GetMatrix();
        curAnim = MATH->Lerp(cur, next, frame.time);
    }


    {
        Frame& frame = frameBuffer->GetData()->cur;
        if(frame.clip < 0)
            return curAnim;
        Matrix cur = nodeTransforms[frame.clip].transform[frame.curFrame][nodeIndex].GetMatrix();
        Matrix next = nodeTransforms[frame.clip].transform[frame.curFrame + 1][nodeIndex].GetMatrix();
        
        Matrix nextAnim = MATH->Lerp(cur, next, frame.time);

        curAnim = MATH->Lerp(curAnim, nextAnim, frameBuffer->GetData()->tweenTime);
    }
}

void ModelAnimator::CreateClipTransform(UINT index)
{
    // Ŭ���� ������ �����ɴϴ�.
    ModelClip* clip = clips[index];

    // Ŭ���� ������ ����ŭ �ݺ�
    FOR(clip->frameCount)
    {
        UINT nodeIndex = 0;

        // ��� ��忡 ���� �ݺ�
        for (NodeData node : nodes)
        {
            Matrix animation;
            // ���� �����ӿ��� ����� Ű�������� �����ɴϴ�.
            KeyFrame* frame = clip->GetKeyFrame(node.name);

            // Ű�������� �����ϸ� �ش� Ű�������� ��ȯ ������ ����Ͽ� ��ȯ ��Ʈ���� ����
            if (frame != nullptr)
            {
                KeyTransform& transform = frame->transforms[i];
                nodeTransforms[index].transform[i][nodeIndex].Scale = transform.scale;
                nodeTransforms[index].transform[i][nodeIndex].Rotation = Vector3(transform.rot.x, transform.rot.y, transform.rot.z);
                nodeTransforms[index].transform[i][nodeIndex].Transform = transform.pos;
            }
            else
            {
                // Ű�������� ������ ��ȯ ��Ʈ������ ���� ��ķ� �ʱ�ȭ
                nodeTransforms[index].transform[i][nodeIndex].Scale     = Vector3(+1, +1, +1);
                nodeTransforms[index].transform[i][nodeIndex].Rotation  = Vector3(+0, +0, +0);
                nodeTransforms[index].transform[i][nodeIndex].Transform = Vector3(+0, +0, +0);
            }

            // �θ� ����� ��ȯ ��Ʈ������ �����ɴϴ�.
            Matrix parent;
            int parentIndex = node.parent;

            if (parentIndex < 0)
                parent = XMMatrixIdentity();
            else
                parent = nodeTransforms[index].transform[i][parentIndex].GetMatrix();

            // ���� ����� ��ȯ ��Ʈ������ ����Ͽ� ����
            nodeTransforms[index].transform[i][nodeIndex] *= parent;

            // ���� ��尡 ���� �ش��Ѵٸ� Ŭ�� ��ȯ ��Ʈ�������� ����
            int boneIndex = -1;
            if (boneMap.count(node.name))
                boneIndex = boneMap[node.name];

            if (boneIndex >= 0)
            {
                Datainput offset = bones[boneIndex].offset;
                Datainput nodem = nodeTransforms[index].transform[i][nodeIndex];
                offset *= nodem;

                clipTransforms[index].transform[i][boneIndex] = offset;
            }
            nodeIndex++;
        }
    }
}

void ModelAnimator::UpdateFrame()
{
    if (!IsPlay) return;

    Motion* motion = frameBuffer->GetData();

    {
        Frame* frame = &motion->cur;
        ModelClip* clip = clips[frame->clip];

        frame->time += clip->tickPerSecond * frame->scale * DELTA;
        clip->playTime += frame->scale * DELTA;

        if (frame->time >= 1.0f)
        {
            frame->curFrame = (frame->curFrame + 1) % (clip->frameCount - 1);
            frame->time -= 1.0f;
        }

        clip->Excute();
    }

    {
        Frame* frame = &motion->next;

        if (frame->clip < 0) return;

        ModelClip* clip = clips[frame->clip];

        motion->tweenTime += DELTA / motion->takeTime;

        if (motion->tweenTime >= 1.0f)
        {
            motion->cur = motion->next;
            motion->tweenTime = 0.0f;

            motion->next.clip = -1;
            motion->next.curFrame = 0;
            motion->next.time = 0.0f;
            return;
        }

        frame->time += clip->tickPerSecond * frame->scale * DELTA;

        if (frame->time >= 1.0f)
        {
            frame->curFrame = (frame->curFrame + 1) % (clip->frameCount - 1);
            frame->time -= 1.0f;
        }
    }
}
