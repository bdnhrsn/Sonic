void mixDown()
{
	for (int i = 0; i < 2 * NFFT; i++)
	{
		for (int j = 0; j < MAX_AOBJ; j++)
		{
			result[i] += output[i][j];
		}
		result[i] /= objNum;
	}
}