float4 VS( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}
// VS������ ���� pos�� �׳� �Ѱ��ֱ⸸ �Ѵ�.
float4 PS() : SV_TARGET
{
    return float4(1, 1, 0, 1);
}
// PS������ ���� �ȼ����� �׳� 1,1,0,1�� ĥ�ع�����.