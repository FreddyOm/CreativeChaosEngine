#pragma once
#include "../../Core.h"
#include "../../String/String.h"
#include "D:\Repositories\json\single_include\nlohmann\json.hpp"

namespace CCE
{
	/// <summary>
	/// An interface class for (de-) serializing class data.
	/// </summary>
	/// <typeparam name="T">The class to be (de-) serialized.</typeparam>
	template<typename T>
	struct CCE_API ISerializable
	{
	public:

		/// <summary>
		/// Serializes the object into a string format.
		/// </summary>
		/// <param name="prettyPrint">Wether or not to indent the strings with newlines.</param>
		/// <returns>The serialized string</returns>
		virtual String SerializeString(bool prettyPrint = false) = 0;

		/// <summary>
		/// Serializes the object into a binary format.
		/// </summary>
		virtual char* SerializeBinary() = 0;

		/// <summary>
		/// Deserializes a binary or string object into the respective object.
		/// </summary>
		/// <param name="serializeString">The string that should be deserialized into the object.</param>
		/// <returns>The deserialized object</returns>
		virtual T Deserialize(String serializeString) = 0;
	};
}
