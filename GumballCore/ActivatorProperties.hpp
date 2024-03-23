#pragma once
#ifndef _activatorproperties
#define _activatorproperties

#include "Activator.hpp"

namespace FieldSchema {
	template<> class FieldValue<string> : public Field {
	public:
		string *get(const intptr_t &obj) { 
			return reinterpret_cast<string *>(obj); 
		}
		SerialStream stream(const intptr_t &obj) {
			string *str = get(obj);
			return JsonValue(str->c_str(), str->size());
		}
		void stream(const intptr_t &obj, SerialStream &stream) {
			string *str = get(obj);
			*str = stream.getJsonValue().to_str();
		}
	};
};
#endif //_activatorproperties