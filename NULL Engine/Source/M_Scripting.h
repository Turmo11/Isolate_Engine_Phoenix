#ifndef __M_SCRIPTING__
#define __M_SCRIPTING__

#include "Module.h"

class ParsonNode;
struct WrenVM;																																			// Contains the different wren objects
struct WrenHandle;

class M_Scripting : public Module
{
public:

	M_Scripting(bool is_active = true);
	~M_Scripting();

	bool						Init					(ParsonNode& config)	override;																	// Initializes and creates the Wren Virtual Machine
	UPDATE_STATUS				PreUpdate				(float dt)				override;
	UPDATE_STATUS				Update					(float dt)				override;
	UPDATE_STATUS				PostUpdate				(float dt)				override;
	bool						CleanUp					()						override;

	bool						LoadConfiguration		(ParsonNode& root)		override;
	bool						SaveConfiguration		(ParsonNode& root)		const override;

	bool						InterpretModule			(const char* module, const char* content);
	WrenHandle*					SetClassHandle		(const char* module, const char* class_name);
	std::vector<std::string>	GetMethodsFromClass		(WrenHandle* w_class);
	void						ReleaseHandle			(WrenHandle* handle_to_release);

private:

	WrenVM* virtual_machine;

	// Global signatures
	WrenHandle* start_signature = nullptr;
	WrenHandle* update_signature = nullptr;

	WrenHandle* test_class = nullptr;
};



#endif //!__M_SCRIPTING__
