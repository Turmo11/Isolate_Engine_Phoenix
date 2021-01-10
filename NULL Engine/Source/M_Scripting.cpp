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

	LoadScript("Assets/Scripts/Move.wren"); 

	return true;
}

UPDATE_STATUS M_Scripting::PreUpdate(float dt)
{
	playing = App->play;
	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_Scripting::Update(float dt)
{
	if (playing)
	{
		wrenSetSlotHandle(virtual_machine, 0, wren_class);
		wrenCall(virtual_machine, update_signature);
	}
	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_Scripting::PostUpdate(float dt)
{
	return UPDATE_STATUS::CONTINUE;
}

bool M_Scripting::CleanUp()
{
	wrenReleaseHandle(virtual_machine, update_signature);
	wrenReleaseHandle(virtual_machine, wren_class);
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
	static int count = 1;
	LOG("[WREN] LOG(%d): %s", count, message);		
	count++;
}

void GetKey(WrenVM* vm)
{																						// Used later on for Input receiving 
	int key = wrenGetSlotDouble(vm, 1);
	int state = wrenGetSlotDouble(vm, 2);

	bool pressed = App->input->GetKey(key) == App->input->GetStateFromInt(state);

	wrenSetSlotBool(vm, 0, pressed);
}

// --- CALLBACKS FROM WREN ---

WrenForeignClassMethods BindForeignClass(WrenVM* vm, const char* module, const char* class_name)
{
	WrenForeignClassMethods w_class_methods;

	w_class_methods.allocate;
	w_class_methods.finalize;

	return w_class_methods;
}

WrenForeignMethodFn BindForeignMethod(WrenVM* vm, const char* module, const char* class_name, bool is_static, const char* signature) { // Wren foraign methods

	if (strcmp(module, "console_test") == 0)
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
		if (strcmp(class_name, "Input") == 0)
		{
			if (!is_static && strcmp(signature, "GetKey(_,_)") == 0)
			{
				return GetKey;
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

// --- WREN VM METHODS ---

void M_Scripting::LoadScript(const char* path)
{
	WrenConfiguration wren_config;
	wrenInitConfiguration(&wren_config);

	wren_config.bindForeignClassFn = BindForeignClass;
	wren_config.bindForeignMethodFn = BindForeignMethod;
	wren_config.writeFn = Write;
	wren_config.errorFn = Error;

	virtual_machine = wrenNewVM(&wren_config);
	std::string wren_script = App->file_system->GetFileContentAsString(path); // Manually loading test script
	std::string file_name = App->file_system->GetFileName(path);																									//start_signature = wrenMakeCallHandle(virtual_machine, "Start()");

	InterpretModule("console_test", wren_script.c_str());													// Interpreting Wren code

	update_signature = wrenMakeCallHandle(virtual_machine, "Update()");
	wren_class = SetClassHandle("console_test", file_name.c_str());

	GetMethodsFromClass(wren_class);
	LOG("[WREN] Loaded %s script", file_name.c_str());
}

bool M_Scripting::InterpretModule(const char* module, const char* code)
{
	return (wrenInterpret(virtual_machine, module, code) == WREN_RESULT_SUCCESS);
}

WrenHandle* M_Scripting::SetClassHandle(const char* module, const char* class_name) 
{
	wrenEnsureSlots(virtual_machine, 1);
	wrenGetVariable(virtual_machine, module, class_name, 0);

	return wrenGetSlotHandle(virtual_machine, 0);
}

std::vector<std::string> M_Scripting::GetMethodsFromClass(WrenHandle* wrenClass) {

	if (!IS_CLASS(wrenClass->value)) {
		LOG("[WARNING] Trying to get methods from a non class handler");
		return std::vector<std::string>(); // return empty string
	}

	std::vector<std::string> ret;
	ObjClass* wren_class = AS_CLASS(wrenClass->value);
	LOG("%.*s\n", wren_class->name->length, wren_class->name->value);
	for (int i = 0; i < wren_class->methods.count; ++i) {
		Method& method = wren_class->methods.data[i];
		if (method.type != METHOD_PRIMITIVE && method.type != METHOD_NONE) {
			ret.push_back(method.as.closure->fn->debug->name);
		}
	}

	return ret;

}
void M_Scripting::ReleaseHandle(WrenHandle* handle_to_release)
{
	wrenReleaseHandle(virtual_machine, handle_to_release);
}