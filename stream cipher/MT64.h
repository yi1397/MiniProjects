#define MT64_W 64
#define MT64_N 312
#define MT64_M 156
#define MT64_R 31

#define MT64_U 29
#define MT64_S 17
#define MT64_T 37
#define MT64_L 43

#define MT64_A 0xb5026f5aa96619e9UL
#define MT64_B 0x71d67fffeda60000UL
#define MT64_C 0xfff7eee000000000UL
#define MT64_D 0x5555555555555555UL

#define MT64_UPPER_MASK 0xffffffff80000000UL
#define MT64_LOWER_MASK 0x7fffffffUL

#define MT64_F 6364136223846793005UL

static unsigned long long int x[MT64_N] = {0UL};

static int k = 0;

void init_mt(int seed)
{
	int i;
	k = 0;
	
	x[0] = seed;
	for (i = 1; i < MT64_N; i++)
		x[i] = MT64_F * (x[i - 1] ^ (x[i - 1] >> (MT64_W - 2))) + i;
}

unsigned long long int gen_rand(void)
{
	unsigned long long int y, z;

	z = (x[k] & MT64_UPPER_MASK) | (x[(k + 1) % MT64_N] & MT64_LOWER_MASK);
	x[k] = x[(k + MT64_M) % MT64_N] ^ (z >> 1) ^ (!(z & 1UL) ? 0UL : MT64_A);
	y = x[k];
	y ^= (y >> MT64_U) & MT64_D;
	y ^= (y << MT64_S) & MT64_B;
	y ^= (y << MT64_T) & MT64_C;
	y ^=  y >> MT64_L;

	k = (k + 1) % MT64_N;
	return y;
}
