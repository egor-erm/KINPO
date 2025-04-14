#include "Error.h"

using namespace std;

Error::Error(ErrorType type) {
	this->type = type;
}

string Error::generateErrorMessage() {
	return "";
}

bool Error::operator<(const Error& err) const {
	return true;
}