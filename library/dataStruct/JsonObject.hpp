#pragma once

#include<library/dataStruct/private/jsonla.h>
using namespace ggicci;

#include<string>
#include<iostream>

using namespace std;

enum JsonObjectType{
    ARRAY,
    OBJECT,
    UNDEFINE
};


class JsonObject
{
private:
	Json m_json;
	JsonObjectType m_type;

public:
    JsonObject()
    {
        m_type = JsonObjectType::UNDEFINE;
    }

    JsonObject(const JsonObject& jsonObject)
    {
        *this = jsonObject;
    }

    JsonObject(string jsonStr)
    {
		if (jsonStr == "")
		{
			jsonStr = "{}";
		}

        m_json = Json::Parse(jsonStr.c_str());

        if(jsonStr.length() == 0)
        {
            m_type = JsonObjectType::UNDEFINE;
            return;
        }

        if(jsonStr[0] == '{')
        {
            m_type = JsonObjectType::OBJECT;
        }else if (jsonStr[0] == '[')
        {
            m_type = JsonObjectType::ARRAY;
        }else
        {
            m_type = JsonObjectType::UNDEFINE;
        }
    }




//数组类型的json
public:
    void addObject(const JsonObject& JsonObject)
    {
        if(m_type == JsonObjectType::OBJECT)
        {
            return;
        }

        if(m_json.IsNull())
        {
            m_json = Json::Parse("[]");
        }

        m_json.Push(JsonObject.m_json);
        m_type = JsonObjectType::ARRAY;
    }

	template<class T>
	void addValue(T data)
	{
		if (m_type == JsonObjectType::OBJECT)
		{
			return;
		}

		if (m_json.IsNull())
		{
			m_json = Json::Parse("[]");
		}

		Json valueJson(data);
		m_json.Push(valueJson);
		m_type = JsonObjectType::ARRAY;
	}

    JsonObject getObject(int index)
    {
        if((index < 0) || (index >= m_json.Size()) || m_type != JsonObjectType::ARRAY)
        {
            JsonObject nullObject;
            return nullObject;
        }

        JsonObject jsonObject(m_json[index].ToString());
        return std::move(jsonObject);
    }

	int getIntValue(int index)
	{
		if ((index < 0) || (index >= m_json.Size()) || m_type != JsonObjectType::ARRAY)
		{
			return 0;
		}

		return m_json[index].AsInt();
	}

	double getDoubleValue(int index)
	{
		if ((index < 0) || (index >= m_json.Size()) || m_type != JsonObjectType::ARRAY)
		{
			return 0;
		}

		return m_json[index].AsDouble();
	}

	string getStringValue(int index)
	{
		if ((index < 0) || (index >= m_json.Size()) || m_type != JsonObjectType::ARRAY)
		{
			return 0;
		}

		return m_json[index].AsString();
	}


//对象类型的json
public:
    void setValue(string key, int value)
    {
        if(m_type == JsonObjectType::ARRAY)
        {
            return;
        }

        if(m_json.IsNull())
        {
            m_json = Json::Parse("{}");
        }

        m_json[key.c_str()] = value;
        m_type = JsonObjectType::OBJECT;
    }

    void setValue(string key, double value)
    {
        if(m_type == JsonObjectType::ARRAY)
        {
            return;
        }

        if(m_json.IsNull())
        {
            m_json = Json::Parse("{}");
        }

        m_json[key.c_str()] = value;
        m_type = JsonObjectType::OBJECT;
    }

    void setValue(string key, string value)
    {
        if(m_type == JsonObjectType::ARRAY)
        {
            return;
        }

        if(m_json.IsNull())
        {
            m_json = Json::Parse("{}");
        }

        m_json[key.c_str()] = value;
        m_type = JsonObjectType::OBJECT;
    }

    void setValue(string key, const JsonObject& jsonObject)
    {
        if(m_type == JsonObjectType::ARRAY)
        {
            return;
        }

        if(m_json.IsNull())
        {
            m_json = Json::Parse("{}");
        }

        m_json[key.c_str()] = jsonObject.m_json;
        m_type = JsonObjectType::OBJECT;
    }
    
    int getIntValue(string key)
    {
        if(m_type != JsonObjectType::OBJECT)
        {
            return 0;
        }

		if (isExistKey(key) == false)
		{
			return 0;
		}

        return m_json[key.c_str()].AsInt();
    }

    double getDoubleValue(string key)
    {
        if(m_type != JsonObjectType::OBJECT)
        {
            return 0;
        }

		if (isExistKey(key) == false)
		{
			return 0;
		}

        return m_json[key.c_str()].AsDouble();
    }

    string getStringValue(string key)
    {
        if(m_type != JsonObjectType::OBJECT)
        {
            return "";
        }

		if (isExistKey(key) == false)
		{
			return "";
		}

        return m_json[key.c_str()].AsString();
    }

    JsonObject getObjectValue(string key)
    {
        if(m_type != JsonObjectType::OBJECT)
        {
            JsonObject nullObject;
            return std::move(nullObject);
        }

        JsonObject jsonObject(m_json[key.c_str()].AsString());
        return std::move(jsonObject);
    }


//通用操作
public:
    bool isExistKey(string key)
    {
        return m_json.Contains(key.c_str()); 
    }

    vector<string> getKeyList()
    {
        return m_json.Keys();
    }

    int size()
    {
        return m_json.Size();
    }

    string toString()
    {
        return m_json.ToString();
    }


//操作符重载
public:
    void operator = (const JsonObject& jsonObject)
    {
        m_json = jsonObject.m_json;
        m_type = jsonObject.m_type;
    }

};


