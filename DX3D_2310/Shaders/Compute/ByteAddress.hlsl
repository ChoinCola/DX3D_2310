struct ComputeInput
{
	uint3 groupID : SV_GroupID;
	// 시스템에서 제공해주는 정보
	uint3 groupThreadID : SV_GroupThreadID;
	uint3 dispatchThreadID : SV_DispatchThreadID;
	uint groupIndex : SV_GroupIndex;
};

// GPU와 CPU가 통신하기 위한 데이터구조체 라고 생각하면 된다.
RWByteAddressBuffer output : register(u0);

[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
	uint index = input.groupID * 10 * 8 * 3 + input.groupIndex;
	uint address = index * 10 * 4;
   
	output.Store3(address + 0, input.groupID);
	output.Store3(address + 12, input.groupThreadID);
	output.Store3(address + 24, input.dispatchThreadID);
	output.Store(address + 36, input.groupIndex);
}