typedef unsigned int u32;
typedef int s32;

typedef char s8;
typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;

typedef long long s64;
typedef unsigned long long u64;

#define global_variable static //unique to the compilation unit
#define internal static 

inline int //will be substituted at the place where it's called
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}