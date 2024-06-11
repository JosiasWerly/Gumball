#include "GLUtils.hpp"


#define impKey(key) else if(k == #key) return GLFW_KEY_##key;
constexpr int getKeyCode(const char *k) {
	if (false);
	impKey(A)

		//delcKey(A), delcKey(B), delcKey(C),
		//delcKey(D), delcKey(E), delcKey(F),
		//delcKey(G), delcKey(H), delcKey(I),
		//delcKey(J), delcKey(K), delcKey(L),
		//delcKey(M), delcKey(N), delcKey(O),
		//delcKey(P), delcKey(Q), delcKey(R),
		//delcKey(S), delcKey(T), delcKey(U),
		//delcKey(V), delcKey(X), delcKey(Z),
		//delcKey(W), delcKey(Y),
		//delcKey(UP), delcKey(DOWN),
		//delcKey(RIGHT), delcKey(LEFT),
		return -1;
}