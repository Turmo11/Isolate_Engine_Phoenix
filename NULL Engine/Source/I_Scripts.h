#include "Component.h"

#include <vector>
#include <string>

struct WrenHandle;			// Wraps a reference to an object

union Value
{
	bool b_value = false;
	float n_value;
	const char* s_value;
};

enum class WrenDataType 
{ 
	UNKNOWN, 
	BOOL, 
	NUMBER, 
	STRING
};

struct ImportedVariable
{
public:

	ImportedVariable(WrenDataType type, const char* name, void* value, WrenHandle* getter);

	Value value;										// Variable value
	std::string var_name;									// Variable name
	WrenDataType data_type = WrenDataType::UNKNOWN;		// Variable type (bool/number/string)

	bool is_public;										// True if the variable accessibility is set to public

	WrenHandle* var_handle = nullptr;						// Reference to the getter object
	
	bool			IsPublic() { return is_public; }
};


struct ImportedMethod
{

public:

	ImportedMethod(std::string name, WrenDataType ret_type, std::vector<ImportedVariable> args, WrenHandle* handle);  // for methods
	ImportedMethod(std::string name, WrenHandle* handle) : method_name(name), method_handle(handle) {};  // for setters

	//ret_type name(arguments[])

	WrenDataType method_type;					// Return type of a function

	std::string method_name;							// Name of the function
	std::vector<ImportedVariable> arguments;	// List of the different arguments

	WrenHandle* method_handle = nullptr;				// Reference to the method handle

};

enum class SCRIPT_STATUS
{
	INIT,
	UPDATE,
	PAUSE,
	STOP,
	CLEANUP
};

struct ImportedScript
{
public:

	~ImportedScript();

private:

	std::string class_name;
	SCRIPT_STATUS script_state;

	WrenHandle* class_handle;

	std::vector<ImportedVariable> variables;
	std::vector<ImportedMethod> methods;

public:

	SCRIPT_STATUS GetState() { return script_state; }
};

