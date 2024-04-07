#pragma once
#include <iostream>

class Anime
{
private:
	std::string name;
	std::string info;
public:
	Anime(std::string name, std::string info): name{name}, info{info} {};

	std::string getinfo(Anime &name) 
	{
		return name.info;
	}
	std::string getname(Anime& name) 
	{
		return name.name;
	}
};
