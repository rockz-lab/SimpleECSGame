//#include "App.h"

#include <fstream>

#include "TestComponents.h"

#include "ECS/ECS.h"
#include "utils/math.h"


// initialize the counter
CompType BaseComponent::m_counter = 0;

ECSManager manager;

int main()
{
	//App application;

	//application.Run();


	//std::ostream& os = std::cout;

	Transform2D tform;
	tform.pos = Vec2{ 1.0f, 2.0f };

	manager.Init();
	eID entityOne = manager.CreateEntity();
	eID entityTwo = manager.CreateEntity();
	eID entityThree = manager.CreateEntity();

	{
		Gravity gravity;
		gravity.g = 9.81f;
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

	manager.Serialize<Gravity, Circle, Transform2D>("test.json");

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


	manager.Deserialize<Gravity, Circle, Transform2D>("test.json");
	
	
	auto& circleHandle = manager.GetComponent<Circle>(1);

	circleHandle.radius += 2;
	std::cout << "circle:\n";
	std::cout << circleHandle.radius << "\n";

	auto& handle = manager.GetComponent<Transform2D>(1);
	std::cout << "pos:\n";
	std::cout << handle.pos.x << "\n";

	

}