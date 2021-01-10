
#include "Application.h"
#include "M_Scripting.h"

#include "I_Scripts.h"

ImportedVariable::ImportedVariable(WrenDataType type, const char* name, void* _value, WrenHandle* getter)
{
	var_name = name;
	data_type = type;
	var_handle = getter;

	// Variable value containers
	bool* var_bool = nullptr;
	float* var_float = nullptr;
	const char* var_string;

	switch (type)
	{
	case WrenDataType::BOOL:
		var_bool = (bool*)_value;
		value.b_value = *var_bool;
		break;

	case WrenDataType::NUMBER:
		var_float = (float*)_value;
		value.n_value = *var_float;
		break;

	case WrenDataType::STRING:
		value.s_value = (char*)_value;
		break;
	}
}


void ImportedVariable::SetValue(Value value, WrenDataType value_type)
{
	if (!IsTypeCasted() || value_type == data_type)
	{
		this->value = value;
	}
}

ImportedMethod::ImportedMethod(std::string name, WrenDataType ret_type, std::vector<ImportedVariable> args, WrenHandle* handle)
{
	method_name = name;
	method_type = ret_type;
	arguments = args;
	method_handle = handle;
}

// CleanUp
ImportedScript::~ImportedScript()
{
	class_name.clear();

	variables.clear();
	methods.clear();
	if (class_handle)
	{
		App->scripting->ReleaseHandle(class_handle);
	}
}


