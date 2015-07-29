#include "ObjModel.h"
#include <string>

int main(int argc, char* argv[])
{
	if (argc < 2)
		return 0;

	std::string fileIn(argv[1]);
	ObjModel::PreReadObjModel(fileIn, fileIn + "x", false, false, true, false);
	return 0;
}