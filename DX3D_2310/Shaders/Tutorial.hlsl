float4 VS( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}
// VS에서는 받은 pos를 그냥 넘겨주기만 한다.
float4 PS() : SV_TARGET
{
    return float4(1, 1, 0, 1);
}
// PS에서는 받은 픽셀값을 그냥 1,1,0,1로 칠해버린다.