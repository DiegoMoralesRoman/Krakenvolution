#include <iostream>

#include "messages/test.pb.h"

int main() {
	std::cout << "Hello world" << std::endl;
	Person p;
	p.set_name("Kekosaurio");
	p.set_id(123);

	auto serialized = p.SerializeAsString();

	Person deser;
	deser.ParseFromString(serialized);

	std::cout << "Name: " << deser.name() << ", id: " << deser.id() << std::endl;
}
