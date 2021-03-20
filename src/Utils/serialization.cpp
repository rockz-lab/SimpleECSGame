#include "serialization.h"

#include "Components/gameComponents.h"



template<typename T>
void serialize_comp(const T& comp, std::vector<uint8_t>& data)
{

	size_t size = sizeof comp;
	size_t oldSize = data.size();
	size_t newSize = oldSize + size;

	data.resize(newSize);
	std::memcpy(&comp, data.begin() + oldSize, size);

}

//void serialize_entity(eID entity)
//{
//	auto signature = manager.GetSystemSignature(entity);
//
//	// Loop over the bit set
//	// This is aaproach is not very efficient, it can be done faster using the trailing zero instruction
//	// As this requires compiler specific instructions and the code here is not performance critical, a naive simple loop is suffucuent
//
//	for (CompType i = 0; i < maxCompType; i++)
//	{
//		if (i == 1)
//		{
//			auto retStruct = manager.GetComponent(entity, i);
//		}
//	}
//}

