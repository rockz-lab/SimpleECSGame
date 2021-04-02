//#include "App.h"

#include <fstream>

#include "ECS/ECS.h"
#include "utils/math.h"
#include "Utils/serialization.h"

#include "Components/gameComponents.h"
// initialize the counter
CompType BaseComponent::m_counter = 0;

extern ECSManager manager;

int main()
{
	//App application;

	//application.Run();


	//std::ostream& os = std::cout;

	Transform2D tform;

	Gravity gravity;
	gravity.g = 9.81;
	Circle circle;
	circle.radius = 20;

	manager.Init();
	eID testEnTT = manager.CreateEntity();
	//eID otherEntity = manager.CreateEntity();

	manager.RegisterComponent<Gravity>();
	manager.RegisterComponent<Circle>();
	manager.RegisterComponent<Transform2D>();

	manager.AddComponent(testEnTT, gravity);
	manager.AddComponent(testEnTT, circle);
	manager.AddComponent(testEnTT, tform);

	//manager.AddComponent(otherEntity, circle);
	//manager.AddComponent(otherEntity, tform);

	manager.Serialize<Gravity, Circle, Transform2D>("test.txt");


	manager.DestroyEntity(testEnTT);
	//manager.DestroyEntity(otherEntity);


	eID newTest = manager.CreateEntity();
	eID newOtherTest = manager.CreateEntity();
	manager.Deserialize<Gravity, Circle, Transform2D>("test.txt");
	

	auto& circleHandle = manager.GetComponent<Circle>(0);
	std::cout << "circle:\n";
	std::cout << circleHandle.radius;

	//std::ofstream fs;
	////fs.exceptions(std::ostream::badbit || std::ostream::failbit);

	//try
	//{
	//	fs.open("test.csv", std::ifstream::trunc);
	//}
	//catch (std::fstream::failure e)
	//{
	//	std::cerr << e.what();
	//}

	///*tform.serialize(fs);
	//circle.serialize(fs);
	//gravity.serialize(fs);*/

	
	std::ifstream inFile;

	//inFile.open("test.txt");

	//std::string discard;
	//std::getline(inFile, discard);
	//std::getline(inFile, discard);

	//auto result = getProperty(inFile, "pos");
	//
	//std::cout << result[0] << std::endl;
	//std::cout << result[1] << std::endl;

}