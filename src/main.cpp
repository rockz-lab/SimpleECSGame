//#include "App.h"

#include <fstream>

#include "ECS/ECS.h"
#include "utils/math.h"

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

	manager.RegisterComponent<Gravity>();
	manager.RegisterComponent<Circle>();
	manager.RegisterComponent<Transform2D>();

	manager.AddComponent(testEnTT, gravity);
	manager.AddComponent(testEnTT, circle);
	manager.AddComponent(testEnTT, tform);

	
	manager.Serialize("test.txt");
	std::ofstream fs;
	//fs.exceptions(std::ostream::badbit || std::ostream::failbit);

	try
	{
		fs.open("test.csv", std::ifstream::trunc);
	}
	catch (std::fstream::failure e)
	{
		std::cerr << e.what();
	}

	/*tform.serialize(fs);
	circle.serialize(fs);
	gravity.serialize(fs);*/

	
	//std::ifstream inFile;

	//inFile.open("test.csv");

	//tform;
	//tform.deserialize(inFile);
	
	std::array<float, 6> stuff{10, 20, 3.0, 1, -.2, 0.01231};

	std::stringstream ss;

	ss.write(reinterpret_cast<const char*>(stuff.data()), sizeof stuff);

	std::array<float, 6> other_array;

	ss.read(reinterpret_cast<char*>(other_array.data()), ss.str().size());

	
	std::cout << other_array[2] << std::endl;

}