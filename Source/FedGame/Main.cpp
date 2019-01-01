#include <FedPCH.h>
int main()
{
	Fed::Log::Init();
	LOG("Initialized Logger");
	std::cin.get();
	return 0;
}