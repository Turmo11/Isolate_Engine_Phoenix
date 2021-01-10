#include "JSONParser.h"

#include "GameObject.h"

#include "C_Script.h"

C_Script::C_Script(GameObject* owner) : Component(owner, COMPONENT_TYPE::SCRIPT)
{

}

C_Script::~C_Script()
{

}

bool C_Script::Update()
{
	bool ret = true;

	return ret;
}

bool C_Script::CleanUp()
{
	bool ret = true;

	return ret;
}

bool C_Script::SaveState(ParsonNode& root) const
{
	bool ret = true;

	root.SetNumber("Type", (uint)GetType());

	return ret;
}

bool C_Script::LoadState(ParsonNode& root)
{
	bool ret = true;



	return ret;
}