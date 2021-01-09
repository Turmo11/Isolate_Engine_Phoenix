#include "M_Scripting.h"
#include "Wren/wren.hpp"

#include "Application.h"
#include "M_Input.h"
#include "M_FileSystem.h"


void ConsoleLog(WrenVM* vm);
void GetKey(WrenVM* vm);

WrenForeignClassMethods BindForeignClass(WrenVM* vm, const char* module, const char* class_name);											// Creates a foreign object
WrenForeignMethodFn BindForeignMethod(WrenVM* vm, const char* module, const char* class_name, bool is_static, const char* signature);	// Method readable with Wren but implemented in C
void Error(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message);
void Write(WrenVM* vm, const char* text);

M_Scripting::M_Scripting(bool is_active) : Module("Scripting", is_active),
virtual_machine(nullptr)
{

}

M_Scripting::~M_Scripting()
{

}

bool M_Scripting::Init(ParsonNode& config)
{
	WrenConfiguration wren_config;
	wrenInitConfiguration(&wren_config);

	wren_config.bindForeignClassFn = BindForeignClass;
	wren_config.bindForeignMethodFn = BindForeignMethod;
	wren_config.writeFn = Write;
	wren_config.errorFn = Error;

	virtual_machine = wrenNewVM(&wren_config);

	std::string wren_script = App->file_system->GetFileContentAsString("Assets/Scripts/alpha_script.wren"); // Manually loading test script
	WrenInterpretResult result = wrenInterpret(virtual_machine, "console_test", wren_script.c_str());		// Interpreting Wren code

	return true;
}

UPDATE_STATUS M_Scripting::PreUpdate(float dt)
{
	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_Scripting::Update(float dt)
{
	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_Scripting::PostUpdate(float dt)
{
	return UPDATE_STATUS::CONTINUE;
}

bool M_Scripting::CleanUp()
{
	wrenFreeVM(virtual_machine);
	return true;
}

bool M_Scripting::LoadConfiguration(ParsonNode& root)
{
	bool ret = true;


	return ret;
}

bool M_Scripting::SaveConfiguration(ParsonNode& root) const
{
	bool ret = true;

	return ret;
}

// --- C FUNCTIONS CALLED FROM WREN ---

void ConsoleLog(WrenVM* vm)																// Logs message to the console
{
	const char* message = wrenGetSlotString(vm, 1);

	LOG("[WREN] LOG: %s", message);														 
}

void GetKey(WrenVM* vm)
{																						// Used later on for Input receiving 
	int key = wrenGetSlotDouble(vm, 1);
	int state = wrenGetSlotDouble(vm, 2);

	bool pressed = App->input->GetKey(key) == App->input->GetStateFromInt(state);

	wrenSetSlotBool(vm, 0, pressed);
}

// --- CALLBACKS FROM WREN 

WrenForeignClassMethods BindForeignClass(WrenVM* vm, const char* module, const char* class_name)
{
	WrenForeignClassMethods w_class_methods;

	w_class_methods.allocate;
	w_class_methods.finalize;

	return w_class_methods;
}

WrenForeignMethodFn BindForeignMethod(WrenVM* vm, const char* module, const char* class_name, bool is_static, const char* signature) { // Wren foraign methods

	if (strcmp(module, "console_test") == 0) //Testing purposes
	{
		if (strcmp(class_name, "Engine") == 0)
		{
			if (is_static && strcmp(signature, "Log(_)") == 0)
			{
				return ConsoleLog; 
			}
			if (!is_static && strcmp(signature, "Log(_)") == 0)
			{
				return ConsoleLog;
			}
		}
	}
}

void Error(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message)
{
	if (type == WrenErrorType::WREN_ERROR_COMPILE)
	{
		LOG("[ERROR] Couldn't compile %s. %s error in %i line", module, message, line);
	}
	else if (type == WrenErrorType::WREN_ERROR_RUNTIME)
	{
		LOG("[ERROR] Runtime error while executing %s. %s error in %i line", module, message, line);
	}
}

void Write(WrenVM* vm, const char* text)
{
	LOG("%s", text);
}

char* LoadModule(WrenVM* vm, const char* name) 
{ 
	return nullptr;
}