//#include<iostream>
#include<string>
#include<map>

class Type {
public:
	enum Kind {
		Base, Pointer, Error, Ok
	};
	enum Basetype {
		Int, Float, Void
	};
	Kind tag;
	union {
		Basetype basetype;
		Type* pointed;
	};
	Type();    // Default
	Type(Kind); // Error, Ok
	Type(Kind, Basetype); //Int, Float
	Type(Kind, Type*);   // Pointer
        void printType();
  //    ~Type();
};

// Type::Type() :
// 		tag(Ok) {
// }
// Type::Type(Kind kindval) :
// 		tag(kindval) {
// }

// Type::Type(Kind kindval, Basetype btype) :
// 		tag(kindval), basetype(btype) {
// }

// Type::Type(Kind kindval, Type* ptd) :
// 		tag(kindval), pointed(ptd) {
// }

// Type::~Type()
// {switch (tag) {
//    case Pointer: delete pointed;
//    break;
// }}

// bool equal(Type t1, Type t2) {
// 	if (t1.tag == Type::Base && t2.tag == Type::Base)
// 		return (t1.basetype == t2.basetype);
// 	else if (t1.tag == Type::Pointer && t2.tag == Type::Pointer)
// 		return equal(*t1.pointed, *t2.pointed);
// }

// bool compatible(Type t1, Type t2) {
// 	if ((t1.basetype == Type::Int || t1.basetype == Type::Float)
// 			&& (t2.basetype == Type::Int || t2.basetype == Type::Float))
// 		return 1;
// 	else
// 		return equal(t1, t2);
// }

// //typedef std::map<std::string, Type> SymbolTableType;

// std::map<std::string, Type> Symboltable = { { "p", Type(Type::Pointer, new Type(Type::Base, Type::Int)) },
// 		                                    { "two", Type(Type::Base, Type::Int) },
// 		                                    { "r", Type(Type::Base,	Type::Float) } };

// int main() {
//   Type t1, t2, t3, t4;
//   std::string x = "Hi there";
//   t1 = Type(Type::Pointer, new Type(Type::Pointer, new Type(Type::Base, Type::Int)));
//   t2 = Type(Type::Pointer, new Type(Type::Pointer, new Type(Type::Base, Type::Float)));
//   t3 = Type(Type::Base, Type::Float);
//   t4 = Type(Type::Base, Type::Int);
//   std::cout << x << std:: endl << compatible(Symboltable["x"], Symboltable["x"]) << std::endl;

// }

