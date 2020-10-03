#include "Platform/Platform.hpp"
#include "genetic.hpp"

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	//std::cout << "< --- Debug Mode --- >" << std::endl;
#endif

	Jumper_GeneticAlgorithm jga;
	jga.exec();

	return 0;
}
