#pragma once

class CLehmerRand
{
	static const unsigned long a = 48271;
	static const unsigned long m = 2147483647;
	static const unsigned long r = 3399;    /* r = m mod a */
	static const unsigned long q = 44488;   /* q = m div a */
	int Seed;
public:
	unsigned int U()
	{
		if ((Seed = Seed % q * a - Seed / q * r) <= 0)
			Seed += m;
		return Seed - 1;
	}
	float F()
	{
		return U()/(float)m;
	}
	float N()
	{
		return F()-0.5f;
	}
	float F10()
	{
		float r=0;
		for(unsigned int i=0;i<10;i++)
		{
			r+=U();
		}
		return r/10.0f/(float)m;
	}
	float N10()
	{
		return F10()-0.5f;
	}
	void SetSeed(unsigned int newSeed){Seed=newSeed;}
	unsigned int GetSeed()const{return Seed;} // useful for saving game state
	CLehmerRand(){}
	CLehmerRand(unsigned int seed):Seed(seed){}
	operator float(){return F();}
};
