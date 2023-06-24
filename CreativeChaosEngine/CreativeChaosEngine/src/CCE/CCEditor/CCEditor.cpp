#include "CCEditor.h"

namespace CCE
{

	void CCEditor::PushEditorIntValue(const String key, const int value) noexcept
	{
		if (!editorIntValues.empty() && 
			editorIntValues.find(key.sId) != editorIntValues.end())
		{
			editorIntValues.at(key.sId) = value;
		}
		else
		{
			editorIntValues.insert({ key.sId, value });
		}
	}

	int CCEditor::GetEditorIntValue(const String key) noexcept
	{
		if (!editorIntValues.empty() &&
			editorIntValues.find(key.sId) != editorIntValues.end())
		{
			return editorIntValues.at(key.sId);
		}
		else
		{
			//DERROR("Failed trying to get editor value with key %s", key.Value());
			return 0;
		}
	}

	void CCEditor::PushEditorLongValue(const String key, const long value) noexcept
	{
		if (!editorLongValues.empty() && 
			editorLongValues.find(key.sId) != editorLongValues.end())
		{
			editorLongValues.at(key.sId) = value;
		}
		else
		{
			editorLongValues.insert({ key.sId, value });
		}
	}

	long CCEditor::GetEditorLongValue(const String key) noexcept
	{
		if (!editorLongValues.empty() && 
			editorLongValues.find(key.sId) != editorLongValues.end())
		{
			return editorLongValues.at(key.sId);
		}
		else
		{
			//DERROR("Failed trying to get editor value with key %s", key.Value());
			return 0;
		}
		return 0;
	}

	void CCEditor::PushEditorBoolValue(const String key, const bool value) noexcept
	{
		if (!editorBoolValues.empty() && 
			editorBoolValues.find(key.sId) != editorBoolValues.end())
		{
			editorBoolValues.at(key.sId) = value;
		}
		else
		{
			editorBoolValues.insert({ key.sId, value });
		}
	}

	bool CCEditor::GetEditorBoolValue(const String key) noexcept
	{
		if (!editorBoolValues.empty() && 
			editorBoolValues.find(key.sId) != editorBoolValues.end())
		{
			return editorBoolValues.at(key.sId);
		}
		else
		{
			//DERROR("Failed trying to get editor value with key %s", key.Value());
			return false;
		}
	}

	void CCEditor::PushEditorFloatValue(const String key, const float value) noexcept
	{
		if (!editorFloatValues.empty() && 
			editorFloatValues.find(key.sId) != editorFloatValues.end())
		{
			editorFloatValues.at(key.sId) = value;
		}
		else
		{
			editorFloatValues.insert({ key.sId, value });
		}
	}

	float CCEditor::GetEditorFloatValue(const String key) noexcept
	{
		if (!editorFloatValues.empty() && 
			editorFloatValues.find(key.sId) != editorFloatValues.end())
		{
			return editorFloatValues.at(key.sId);
		}
		else
		{
			//DERROR("Failed trying to get editor value with key %s", key.Value());
			return 0;
		}
	}

	void CCEditor::PushEditorStringValue(const String key, const String value) noexcept
	{
		if (!editorStringValues.empty() && 
			editorStringValues.find(key.sId) != editorStringValues.end())
		{
			editorStringValues.at(key.sId) = value;
		}
		else
		{
			editorStringValues.insert({ key.sId, value });
		}
	}

	String CCEditor::GetEditorStringValue(const String key) noexcept
	{
		if (!editorStringValues.empty() && 
			editorStringValues.find(key.sId) != editorStringValues.end())
		{
			return editorStringValues.at(key.sId);
		}
		else
		{
			//DERROR("Failed trying to get editor value with key %s", key.Value());
			return String();
		}
	}

	std::unordered_map<unsigned long long, int> CCEditor::editorIntValues = std::unordered_map<unsigned long long, int>();
	std::unordered_map<unsigned long long, long> CCEditor::editorLongValues = std::unordered_map<unsigned long long, long>();
	std::unordered_map<unsigned long long, float> CCEditor::editorFloatValues = std::unordered_map<unsigned long long, float>();
	std::unordered_map<unsigned long long, bool> CCEditor::editorBoolValues = std::unordered_map<unsigned long long, bool>();
	std::unordered_map<unsigned long long, String> CCEditor::editorStringValues = std::unordered_map<unsigned long long, String>();
}
