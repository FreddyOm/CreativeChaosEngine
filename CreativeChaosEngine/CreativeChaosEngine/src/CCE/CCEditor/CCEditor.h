#pragma once
#include "../Core.h"
#include "../String/String.h"
#include <unordered_map>

#define PUSH_EDITOR_BOOL(key, value) CCE::CCEditor::PushEditorBoolValue(key, value)ff
#define PUSH_EDITOR_LONG(key, value) CCE::CCEditor::PushEditorLongValue(key, value)
#define PUSH_EDITOR_INT(key, value) CCE::CCEditor::PushEditorIntValue(key, value)
#define PUSH_EDITOR_FLOAT(key, value) CCE::CCEditor::PushEditorFloatValue(key, value)
#define PUSH_EDITOR_STRING(key, value) CCE::CCEditor::PushEditorStringValue(key, value)

#define GET_EDITOR_BOOL(key) CCE::CCEditor::GetEditorBoolValue(key)
#define GET_EDITOR_LONG(key) CCE::CCEditor::GetEditorLongValue(key)
#define GET_EDITOR_INT(key)	CCE::CCEditor::GetEditorIntValue(key)
#define GET_EDITOR_FLOAT(key) CCE::CCEditor::GetEditorFloatValue(key)
#define GET_EDITOR_STRING(key) CCE::CCEditor::GetEditorStringValue(key)

namespace CCE
{
	struct CCE_API CCEditor
	{
		static void PushEditorIntValue(const String key, const int value) noexcept;
		static int GetEditorIntValue(const String key)  noexcept;
		static void PushEditorLongValue(const String key, const long value) noexcept;
		static long GetEditorLongValue(const String key)  noexcept;
		static void PushEditorBoolValue(const String key, const bool value) noexcept;
		static bool GetEditorBoolValue(const String key)  noexcept;
		static void PushEditorFloatValue(const String key, const float value) noexcept;
		static float GetEditorFloatValue(const String key)  noexcept;
		static void PushEditorStringValue(const String key, const String value) noexcept;
		static String GetEditorStringValue(const String key) noexcept;

	private:
		static std::unordered_map<unsigned long long, int> editorIntValues;
		static std::unordered_map<unsigned long long, long> editorLongValues;
		static std::unordered_map<unsigned long long, float> editorFloatValues;
		static std::unordered_map<unsigned long long, bool> editorBoolValues;
		static std::unordered_map<unsigned long long, String> editorStringValues;
	};
}
