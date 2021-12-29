#ifndef __math
#define __math

struct Color {
	union {
		unsigned char R, G, B, A;
		int hexColor;	
	};
};
#endif // !__math
