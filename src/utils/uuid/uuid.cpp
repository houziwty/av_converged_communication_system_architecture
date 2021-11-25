#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "utils/uuid/uuid.h"
using namespace framework::utils::uuid;

Uuid::Uuid()
{}

Uuid::~Uuid()
{}

const std::string Uuid::createNew()
{
	return boost::uuids::to_string(boost::uuids::random_generator()());
}