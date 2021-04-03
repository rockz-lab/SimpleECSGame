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
	tform.pos.m_vec = glm::vec2({ 1.0f, 2.0f });

	manager.Init();
	eID entityOne = manager.CreateEntity();
	eID entityTwo = manager.CreateEntity();
	eID entityThree = manager.CreateEntity();

	{
		Gravity gravity;
		gravity.g = 9.81;
		Circle circle;
		circle.radius = 20;
		Circle smallCircle;
		smallCircle.radius = 2;
		Circle bigCircle;
		bigCircle.radius = 60;


		manager.RegisterComponents<Gravity, Circle, Transform2D>();

		manager.AddComponent(entityOne, gravity);
		manager.AddComponent(entityOne, smallCircle);
		manager.AddComponent(entityOne, tform);

		manager.AddComponent(entityTwo, bigCircle);
		manager.AddComponent(entityTwo, tform);


		manager.AddComponent(entityThree, tform);
		manager.AddComponent(entityThree, circle);

	}

	manager.Serialize<Gravity, Circle, Transform2D>("test.txt");

	manager.DestroyEntity(entityOne);
	manager.DestroyEntity(entityThree);
	manager.DestroyEntity(entityTwo);

	eID newTest = manager.CreateEntity();
	eID newOtherTest = manager.CreateEntity();
	eID oneMoreTest = manager.CreateEntity();


	//manager.AddComponent(newTest, circle);
	//manager.AddComponent(newOtherTest, circle);

	//manager.AddComponent(newTest, gravity);

	//manager.AddComponent(newTest, tform);
	//manager.AddComponent(newOtherTest, tform);


	manager.Deserialize<Gravity, Circle, Transform2D>("test.txt");
	

	auto& circleHandle = manager.GetComponent<Circle>(1);
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