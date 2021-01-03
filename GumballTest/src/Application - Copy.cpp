#ifdef expDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

int main() {

	return 0;
}