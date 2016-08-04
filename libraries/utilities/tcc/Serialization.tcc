////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     Serialization.tcc (utilities)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

namespace utilities
{
    //
    // Serializer class
    //

    // TODO: split out scalar vs. array here (or is it fundamental vs. ISerializable?)
    template <typename ValueType>
    void Serializer::Serialize(ValueType&& value)
    {
        Serialize("", value);
    }

    template <typename ValueType, IsNotVector<ValueType> concept>
    void Serializer::Serialize(const char* name, ValueType&& value)
    {
        SerializeValue(name, value);
    }

    // Pointers
    template <typename ValueType>
    void Serializer::Serialize(const char* name, ValueType* value)
    {
        Serialize(name, *value);
    }

    // Vector of fundamental types
    template <typename ValueType, IsFundamental<ValueType> concept>
    void Serializer::Serialize(const char* name, const std::vector<ValueType>& array)
    {
        SerializeArrayValue(name, array);
    }

    // Vector of serializable objects
    template <typename ValueType, IsSerializable<ValueType> concept>
    void Serializer::Serialize(const char* name, const std::vector<ValueType>& array)
    {
        std::vector<const utilities::ISerializable*> tmpArray;
        for (const auto& item : array)
        {
            tmpArray.push_back(&item);
        }
        SerializeArrayValue(name, tmpArray);
    }

    // Vector of serializable pointers
    template <typename ValueType, IsSerializable<ValueType> concept>
    void Serializer::Serialize(const char* name, const std::vector<const ValueType*>& array)
    {
        std::vector<const utilities::ISerializable*> tmpArray;
        for (const auto& item : array)
        {
            tmpArray.push_back(item);
        }
        SerializeArrayValue(name, tmpArray);
    }

    //
    // Deserialization
    //
    template <typename ValueType>
    void Deserializer::Deserialize(ValueType&& value, SerializationContext& context)
    {
        Deserialize("", value, context);
    }

    template <typename ValueType, IsNotVector<ValueType> concept>
    void Deserializer::Deserialize(const char* name, ValueType&& value, SerializationContext& context)
    {
        DeserializeValue(name, value, context);
    }

    // pointer to non-serializable type
    template <typename ValueType, IsNotSerializable<ValueType> concept>
    void Deserializer::Deserialize(const char* name, std::unique_ptr<ValueType>& value, SerializationContext& context)
    {
        std::cout << "Deserializing non-serializable pointer" << std::endl;
        auto ptr = std::make_unique<ValueType>();
        DeserializeValue(name, *ptr, context);
        value = std::move(ptr);
    }

    // pointer to serializable type
    template <typename ValueType, IsSerializable<ValueType> concept>
    void Deserializer::Deserialize(const char* name, std::unique_ptr<ValueType>& value, SerializationContext& context)
    {
        std::cout << "Deserializing serializable pointer" << std::endl;
        ValueType dummy; // TODO: find a way to get rid of this

        auto typeName = BeginDeserializeObject(name, dummy, context);

        // TODO: create new typeName thing
        auto newPtr = std::make_unique<ValueType>(); // ####
        // Somehow we need to get a TypeFactory<ValueType>
        // auto newPtr = factory.Construct(typeName);

        DeserializeObject(name, *newPtr, context);
        EndDeserializeObject(name, *newPtr, context);
        value = std::move(newPtr);
    }

    template <typename ValueType, IsFundamental<ValueType> concept>
    void Deserializer::Deserialize(const char* name, std::vector<ValueType>& array, SerializationContext& context)
    {
        DeserializeArray(name, array, context);
    }

    // Vector of serializable objects
    template <typename ValueType, IsSerializable<ValueType> concept>
    void Deserializer::Deserialize(const char* name, std::vector<ValueType>& array, SerializationContext& context)
    {
        DeserializeArray(name, array, context);
    }

    // Vector of serializable pointers
    template <typename ValueType, IsSerializable<ValueType> concept>
    void Deserializer::Deserialize(const char* name, std::vector<const ValueType*>& array, SerializationContext& context)
    {
        std::cout << "Deserializing array of pointers";
        throw LogicException(LogicExceptionErrors::notImplemented, "Deserialize vector<ValueType*> not implemented."); 
        std::vector<const utilities::ISerializable*> tmpArray;
//        DeserializeArray(name, tmpArray, context);
        // TODO: copy
        for(const auto& item: tmpArray)
        {

        }
    }}
