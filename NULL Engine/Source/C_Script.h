#ifndef __C_Script_H__
#define __C_Script_H__

#include "Component.h"
#include "I_Scripts.h"

class ParsonNode;
class GameObject;

class C_Script : public Component
{
public:
	C_Script(GameObject* owner);
	~C_Script();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveState	(ParsonNode& root) const override;
	bool LoadState	(ParsonNode& root) override;

private:


};

#endif // !__C_Script_H__