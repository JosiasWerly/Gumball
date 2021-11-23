#pragma once



struct PtrData{
	void *ptr = nullptr;
	unsigned int refCounter = 1;
};

template<class T>
class Var{
	template<class t> friend class Var;
	template<class t> friend Var<t> &&convertTo();

	PtrData *data = new PtrData;
	T **ptrCasted = (T **)&data->ptr;
protected:
	inline void ref() {
		data->refCounter++;
	}
	inline void unRef() {
		if (--data->refCounter == 0) {
			cout << "del: " << &data << endl;
			delete *ptrCasted;
			delete data;
		}
	}
	inline void setRef(PtrData *newData) {
		unRef();
		data = newData;
		ptrCasted = (T **)&data->ptr;
		ref();
	}
	
public:
	~Var() {
		unRef();
	}
	Var(T *init = nullptr){
		data->ptr = init;
		ptrCasted = (T**)&data->ptr;
		cout << "new: " << &data << endl;
	}
	Var(const Var &other) {
		setRef(other.data);
	}
	template<class t>Var(const Var<t>&other){
		setRef(other.data);
	}


	
	Var& operator=(const Var &other) {
		setRef(other.data);
		return *this;
	}
	
	unsigned int referenceCount() { return data->refCounter; }

	T *operator*() { return data->ptr; }
	T *operator->() { return data->ptr; }
	operator bool() { return data->ptr;	}
	bool operator ==(Var &other) { return data == other.data; }
};
