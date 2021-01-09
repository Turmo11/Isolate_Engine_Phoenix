#ifndef __M_SCRIPTING__
#define __M_SCRIPTING__

#include "Module.h"

class ParsonNode;
struct WrenVM;																																			// Contains the different wren objects

class M_Scripting : public Module
{
public:

	M_Scripting(bool is_active = true);
	~M_Scripting();

	bool					Init					(ParsonNode& config)	override;																	// Initializes and creates the Wren Virtual Machine
	UPDATE_STATUS			PreUpdate				(float dt)				override;
	UPDATE_STATUS			Update					(float dt)				override;
	UPDATE_STATUS			PostUpdate				(float dt)				override;
	bool					CleanUp					()						override;

	bool					LoadConfiguration		(ParsonNode& root)		override;
	bool					SaveConfiguration		(ParsonNode& root)		const override;

public:

	//We'll try using the naming already established in Wren to avoid confusion, however we will still try to keep the naming conventions
	
private:

	WrenVM* virtual_machine;
};



#endif //!__M_SCRIPTING__
