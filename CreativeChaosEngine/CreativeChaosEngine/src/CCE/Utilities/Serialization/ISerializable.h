#pragma once
#include "../../Core.h"
#include "../../String/String.h"
#include "json.hpp"
#include <vector>
#include <string>

namespace CCE
{
#define SERIALIZE_CLASS_MEMBER(varName) data[typeid(*this).name()][#varName] = varName
#define DESERIALIZE_CLASS_MEMBER(varName) varName = data[typeid(*this).name()][#varName]

	/// <summary>
	/// An interface class for (de-) serializing class data.
	/// </summary>
	/// <typeparam name="T">The class to be (de-) serialized.</typeparam>
	template<typename T>
	struct CCE_API ISerializable
	{
	public:

#define JSON nlohmann::json

		//TODO: Change std::string to CCE::String -> Fix problem with parsed string to CCE::String

		/// <summary>
		/// Serializes the object into a string format.
		/// </summary>
		/// <param name="prettyPrint">Wether or not to indent the strings with newlines.</param>
		/// <returns>The serialized string</returns>
		virtual std::string SerializeToString(bool prettyPrint = false) = 0;

		/// <summary>
		/// Serializes the object into a binary format.
		/// </summary>
		virtual std::vector<uint8_t> SerializeToBinary() = 0;

		/// <summary>
		/// Deserializes a string object into the respective object.
		/// </summary>
		/// <param name="serializeString">The string that should be deserialized into the object.</param>
		virtual void DeserializeFromString(std::string serializeString) = 0;

		/// <summary>
		/// Deserializes a binary object into the respective object.
		/// </summary>
		/// <param name="serializeData">The binary that should be deserialized into the object.</param>
		virtual void DeserializeFromBinary(std::vector<uint8_t> serializeData) = 0;

	};
}
