#pragma once

template <typename T, int N> class TAvg
{
	T A[N];
	T S;
	unsigned int i;
public:
	void Add(const T& v)
	{
		unsigned int c=i%N;
		S-=A[c];
		S+=v;
		A[c]=v;
		i++;
	}
	T Get()const
	{
		if(i>=N)
			return S/N;
		else
			if(i)
				return S/i;
		return 0;
	}
	TAvg()
	: S(0)
	, i(0)
	{
		for(unsigned int i=0;i<N;i++)
			A[i]=(T)0.0;
	}
};
