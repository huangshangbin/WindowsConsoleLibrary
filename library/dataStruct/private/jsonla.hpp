#ifndef GGICCI_JSONLA_H_
#define GGICCI_JSONLA_H_

#define NDEBUG
#ifdef _DEBUG
#define TRACK(DESC) do {std::cout << "--> " << DESC << std::endl;} while (0);
#endif
#ifdef NDEBUG
#define TRACK(DESC)
#endif

#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>


namespace ggicci
{
/**
 * \brief A Json Parser and Manipulator.
 * \details This class is used to parse a json structural string. After parsing,
 * 			you can easily access the data in simple statements through the plain
 * 			C++ object. Feel free to contact me to help my developing this library.
 * 			And you can fetch the source code from Github
 * 			<a href="https://github.com/ggicci/ggicci--json">here</a>.
 *
 * \author Ggicci <ggicci@163.com>
 * \date 2013.8.26
 * \bug Maybe there exists petential memory leaks. Please be sure to let me know
 * 		if you find some bugs. And I'll be very appreciated :) After all, this is
 * 		my \b first repo on Github. And please give me some suggestions on optimizing
 * 		this small library.
 * \warning Some of the functions may throw exceptions if there's a parse error or
 * 			convert error.
 * \copyright GPLv3
 */
	class Json
	{
	public:
		/**
		 * \brief Enumerate all kinds of the <a href="http://www.json.org/">json structure</a>.
		 */
		enum Kind
		{
			kNumber,	///< a number, eg. 2013, 12.45, 3.3e+12, -12E-5
			kString,	///< a string, eg. "hello world"
			kBool,		///< a boolean, true or false
			kNull,		///< null
			kObject,	///< an object, eg. {"id": 1931, "name": "Ggicci"}
			kArray		///< an array, eg. [ 1, 2, null, {"love": true} ]
		};

		/**
		 * \brief Parse a json structural string to get a Json object.
		 *
		 * \note It may throw an exception when parsing failed. The std::exception::what() message
		 * tells you the failed reason.
		 * @param  json_string json structural string
		 * @return             a Json instance
		 *
		 * \code{.cpp}
		 * try
		 * {
		 * 	Json json = Json::Parse("{ \"year\": 2013, month: 8 }"); // cause exception
		 * }
		 * catch (exception& e)
		 * {
		 * 	cout << e.what() << endl;
		 * 	// output:
		 * 	// SyntaxError: Unexpected token m at pos 16
		 * }
		 * \endcode
		 */
		static Json Parse(const char* json_string);

		/**
		 * \brief Construct a Json object represents null.
		 * 
		 * \code{.cpp}
		 * Json json;
		 * Json another = Json::Parse("null");
		 * json.IsNull(); // true
		 * another.IsNull(); // true
		 * \endcode
		 */
		Json();

		/**
		 * \brief Construct a Json object from \b int.
		 */
		explicit Json(int num);

		/**
		 * \brief Construct a Json object from \b double.
		 */
		explicit Json(double num);

		/**
		 * \brief Construct a Json object from \b string.
		 * \note the string \em str will not be parsed.
		 */
		explicit Json(const std::string& str);

		/**
		 * \brief Construct a Json object from \b string.
		 * \note the string \em str will not be parsed.
		 */
		explicit Json(const char* str);

		/**
		 * \brief Construct a Json object from \b bool.
		 */
		explicit Json(bool boo);

		/**
		 * \brief Deep copy constructor.
		 * @param rhs another Json object.
		 */
		Json(const Json& rhs);

		/**
		 * \brief Assignment, DO deep copy.
		 *
		 * Copy everything from \em json to this Json object. And the old memory will be
		 * changed or just deallocated. The current Json object will hold a copy of the
		 * values of \em json.
		 */
		Json& operator = (const Json& rhs);

		/**
		 * \brief Destructor to delete a Json object.
		 */
		virtual ~Json();

		/**
		 * \brief Get the enum value of this Json object, which indicates
		 * the type of the data it holds.
		 * @return an enum value
		 * 
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, 3, 4]");
		 * json.IsArray(); // true
		 * json.DataKind() == Json::kArray; // true
		 * \endcode
		 */
		Kind DataKind() const { return kind_; }

		/**
		 * \brief Test whether this Json object represents a number.
		 * @return true means it's a number
		 */
		bool IsNumber() const { return kind_ == kNumber; }

		/**
		 * \brief Test whether this Json object represents a string.
		 * @return true means it's a string
		 */
		bool IsString() const { return kind_ == kString; }

		/**
		 * \brief Test whether this Json object represents a bool.
		 * @return true means it's a bool (its value may be true or false)
		 */
		bool IsBool() const { return kind_ == kBool; }

		/**
		 * \brief Test whether this Json object represents null.
		 * \note Distinguish with IsEmpty()
		 * \see Json()
		 * @return true means it's null
		 */
		bool IsNull() const { return !data_; }

		/**
		 * \brief Test whether this Json object represents an array.
		 * @return true means it's an array
		 */
		bool IsArray() const { return kind_ == kArray; }

		/**
		 * \brief Test whether this Json object represents an object.
		 * @return true means it's an object
		 */
		bool IsObject() const { return kind_ == kObject; }

		/**
		 * \brief Test whether an object or array holds values.
		 * @return true if no values in an object or array
		 * 
		 * \code{.cpp}
		 * Json empty_object = Json::Parse("{}");
		 * Json empty_array = Json::Parse("[]");
		 * empty_object.IsEmpty(); // true
		 * empty_array.IsEmpty(); // true
		 * empty_object.IsNull; // false
		 * \endcode
		 */
		bool IsEmpty() const;

		/**
		 * \brief Test if an object Json object has a KVP(key-value pair) named \em key.
		 * 
		 * An object Json object means this Json object represents an object "{...}".
		 * \note if this Json object does not represent an object (IsObject() == false),
		 * 		 it always returns false.
		 * 
		 * @param  key the key(name) of a KVP
		 * @return \b true if IsObject() == true and it contains that KVP, otherwise \b false
		 * 
		 * \code {.cpp}
		 * Json object = Json::Parse("{\"name\": \"Ggicci\", \"age\": 21}");
		 * object.Contains("name"); // true
		 * object.Contains("sex"); // false
		 * object = 3; // will delete the data of object and 
		 *             // allocate new memory for object to 
		 *             // hold value (3). and now, object is
		 *             // a number
		 * object.Contains("hello"); // false
		 * \endcode
		 */
		bool Contains(const char* key) const;

		/**
		 * Indicate the size of the array.
		 * @return size of the array, or 1 if it is not an array.
		 * 
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, 3, 4]");
		 * json.Size(); // 4
		 * json.Push(5).Push(6).Size(); // 6
		 * json = 0; // 1
		 * (json = "Ggicci").Size(); // 1
		 * \endcode
		 */
		int Size() const;

		/**
		 * \brief Get all the keys of the KVPs the object Json object holds.
		 *
		 * If this Json object represents an object (parsed from "{...}"), it
		 * will have some KVPs (or maybe none), then use this function you can
		 * retrieve all the keys (names) in a std::vector.
		 * \note An empty vector will be returned if it's not an object Json object.
		 * @return a std::vector contains all the keys
		 * 
		 * <b>Example 1:</b>
		 * \code{.cpp}
		 * Json json = Json::Parse("{\"id\":1234, \"name\": \"Ggicci\", \"birthday\": [1991, 11, 10]}");
		 * vector<string> keys = json.Keys();
		 * for (vector<string>::const_iterator cit = keys.begin(); cit != keys.end(); ++cit)
		 * {
		 * 	cout << *cit << ": " << json[cit->c_str()].ToString() << endl;
		 * }
		 * \endcode
		 * \b Output:
		 * \code{.txt}
		 * birthday: [ 1991, 11, 10 ]
		 * id: 1234
		 * name: "Ggicci"
		 * \endcode
		 */
		std::vector<std::string> Keys() const;

		/**
		 * \brief Push a Json object to the current Json object (finally an array).
		 *
		 * If current Json object is a number, string, bool, null or object, it will be
		 * converted to an array first and then push back \em rhs to this array.
		 * If current Json object is an array, just simply push back \em rhs.
		 * Otherwise, an exception will be thrown.
		 * \note An exception may be thrown to indicate that it's a bad operation.
		 * @param  rhs the Json object to push
		 * @return     The Json object finally got
		 * 
		 * <b>Example 1:</b>
		 * \code{.cpp}
		 * Json json(10);
		 * cout << json.ToString() << endl; // 10
		 * json.Push("fuck").Push("{Here will not be parsed}").Push(true);
		 * cout << json.ToString() << endl;
		 * // [ 10, "fuck", "{Here will not be parsed}", true ]
		 * \endcode
		 */
		Json& Push(const Json& rhs);

		/**
		 * \brief AddProperty a Json object to the current Json object (finally an object).
		 *
		 * If this Json object doesn't represent an object (IsObject() == false), an exception
		 * will be thrown. Otherwise, a KVP(key-value pair) will be insert into this object
		 * Json object, which consists of \em key and \em val. The function returns the 
		 * referece to this object, so you can call AddProperty function in a cascade way.
		 * \note An exception may be thrown to indicate that it's a bad operation.
		 * @param  key the key(name) of the KVP
		 * @param  val the value of the KVP
		 * @return      the Json object finally got
		 *
		 * <b>Example 1:</b>
		 * \code{.cpp}
		 * Json json = Json::Parse("{}");
		 * json.AddProperty("name", Json("Ggicci"));
		 * json.AddProperty("characteristics", Json::Parse("[\"optimitic\", \"sympathetic\"]"));
		 * cout << json.ToString() << endl;
		 * // output:
		 * // { "characteristics": [ "optimitic", "sympathetic" ], "name": "Ggicci" }
		 * \endcode
		 * 
		 */
		Json& AddProperty(const std::string& key, const Json& value);

		/**
		 * \brief Remove a specefied KVP(key-value pair) from an object Json object by key(name).
		 *
		 * Find the KVP named \em key and remove it from this current Json object. Simultaneously,
		 * the memory will be deallocated. And nothing will happen if not found.
		 * \note An exception will be thrown when you try to operate on non-object Json object.
		 * @param  key the key(name) of the KVP
		 * @return     the Json object after removing the specified KVP
		 *
		 * \code{.cpp}
		 * Json json = Json::Parse("{\"color\": \"green\", \"weight\": 12.4}");
		 * cout << "json = " << json.ToString() << endl;
		 * // output:
		 * // json = { "color": "green", "weight": 12.4 }
		 * cout << "json(after) = " << json.Remove("color").Remove("not found").ToString() << endl;
		 * // output:
		 * // json(after) = { "weight": 12.4 }
		 * \endcode
		 */
		Json& Remove(const std::string& key);

		/**
		 * \brief Remove an item from an array by the specified index
		 *
		 * If the index out of range [0, Size()-1], the specified item of this array
		 * will be removed and deleted in memory.
		 * \note This function will not return the Json object.
		 * @param index the index of the array
		 *
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, true, \"Hello World\"]");
		 * json.Remove(1);
		 * cout << json.ToString() << endl;
		 * // output:
		 * // [ 1, true, "Hello World" ]
		 * \endcode
		 */
		void Remove(int index);

		/**
		 * \brief Extract the data from number Json object and return it as \b int.
		 * \note Exception when Json object is not a number.
		 */
		int AsInt() const;

		/**
		 * \brief Extract the data from number Json object and return it as \b double.
		 * \note Exception when Json object is not a number.
		 */
		double AsDouble() const;

		/**
		 * \brief Extract the data from bool Json object and return it as \b bool.
		 * \note Exception when Json object is not a bool.
		 */
		bool AsBool() const;

		/**
		 * \brief Extract the data from string Json object and return it as \b std::string.
		 * \note Exception when Json object is not a string.
		 */
		std::string AsString() const;

		/**
		 * \brief Extract the item data from an array. Return the reference.
		 * \note Exception when Json object is not an array.
		 */
		const Json& operator[] (int index) const;

		/**
		 * \brief Extract the item data from an array. Return the reference.
		 * \note Exception when Json object is not an array.
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, 3 ,4]");
		 * json[1] = "hello"; // [1, "hello", 3, 4]
		 * \endcode
		 */
		Json& operator[] (int index);

		/**
		 * \brief Extract the item data from an object by specified a key(name).
		 * Return the reference.
		 * \note Exception when Json object is not an object.
		 */
		const Json& operator[] (const char* key) const;

		/**
		 * \brief Extract the item data from an object by specified a key(name).
		 * Return the reference.
		 * \note Exception when Json object is not an object.
		 * \code{.cpp}
		 * Json json = Json::Parse("{\"author\": \"Ggicci\", \"sex\": 0}");
		 * json["sex"] = 1; // { "author": "Ggicci", "sex": 1 }
		 * \endcode
		 */
		Json& operator[] (const char* key);

		/**
		 * \brief Assignment from \b int, finally become a number.
		 */
		Json& operator = (int num);

		/**
		 * \brief Assignment from \b double, finally become a number.
		 */
		Json& operator = (double num);

		/**
		 * \brief Assignment from \b std::string, finally become a string.
		 * \note \en str will not be parsed.
		 */
		Json& operator = (const std::string& str);

		/**
		 * \brief Assignment from \b const char*, finally become a string.
		 * \note \em str will not be parsed.
		 */
		Json& operator = (const char* str);

		/**
		 * \brief Assignment from \b bool, finally become a bool.
		 */
		Json& operator = (bool boo);

		/**
		 * \brief Get the json structural string of this Json object.
		 *
		 * For example, if this Json object represents an object, and it has
		 * a KVP named "id" and its value is 194024, then you will get a json
		 * string like this: <b>{ "id": 194024 }</b>.
		 * @return the json structural string
		 */
		std::string ToString() const;

	private:
		#define CAST_JSON_OBJ(DATA) (static_cast<ObjectData*>(DATA))
		#define CAST_JSON_ARR(DATA) (static_cast<ArrayData*>(DATA))

		typedef std::vector<Json*> ArrayData;
		typedef std::map<std::string, Json*> ObjectData;
		typedef std::pair<std::string, Json*> Pair;

		/**
		 * \brief A nested struct who does the real parsing job.
		 * 
		 * Firstly, I want to use static functions for doing
		 * the same job. However, it is annoying to pass arguments
		 * between different functions in order to keep the library
		 * \b thread-safe.
		 */
		struct Parser
		{
			const char* source;		///< the json structural string need to be parsed
			int			pos;		///< current position(index) of the character in \em source
			unsigned char character;///< current character scanned at
			std::string	token;		///< appear as a word, finally it will be parsed to correspoding
									///< data structure
									///< ~~~
									///< +---------+---+----------+---+---------+---+
									///< | source  | [ | 112.4e+3 | , | "hello" | ] |
									///< +---------+---+----------+---+---------+---+
									///< | tokens  |   |  token   |   |  token  |   |
									///< +---------+---+----------+---+---------+---+
									///< ~~~

			/**
			 * Constructor
			 */
			Parser(const char* json_string);

			/**
			 * \brief Parse a \b Value.
			 *
			 * Value is a general term. A value maybe a string, number, object, array, bool, null.
			 * ![value](value.gif "value")
			 * \note All the consume functions will throw an exception when syntax error occurs
			 * 		 in the \en source string.
			 * @param  section whether global, it will check the end of the input when global
			 * @return         the Json object parsed from \em source (maybe from substring)
			 */
			Json* ConsumeValue(bool section = true);

			/**
			 * \brief Parse a \b number.
			 * 
			 * ![number](number.gif "number")
			 * @return the number Json object parsed
			 */
			Json* ConsumeNumber();

			/**
			 * \brief Parse a \b string.
			 * 
			 * ![string](string.gif "string")
			 * @return the string Json object parsed
			 */
			Json* ConsumeString();

			/**
			 * \brief Parse a \b bool(true or false).
			 * @return the bool Json object parsed
			 */
			Json* ConsumeBool();

			/**
			 * \brief Parse a \b null.
			 * @return the null Json object parsed
			 */
			Json* ConsumeNull();

			/**
			 * \brief Parse an \b object.
			 * 
			 * ![object](object.gif "object")
			 * @return the object Json object parsed
			 */
			Json* ConsumeObject();

			/**
			 * \brief Parse an \b array.
			 * 
			 * ![array](array.gif "array")
			 * @return the array Json object parsed
			 */
			Json* ConsumeArray();

			/**
			 * \brief Parse a \b pair of an \b object.
			 * 
			 * Object of a Json consists of zero or more pairs. Ref :
			 * <a href="www.json.org">json.org</a>
			 * ![object](object.gif "object")
			 * @return the pair parsed
			 */
			Json::Pair ConsumePair();

			/**
			 * \brief Consume a specified string in the \em source.
			 * \note If the string not found in \em source, it will throw an exception
			 * 		 to indicate a parse error.
			 * @param str the spcified string to be cosumed in \em source
			 */
			void ConsumeSpecific(const char* str);

			/**
			 * \brief Detect the whole \em source string represents what, a string or a number or something else.
			 * @return the Kind enum value indicates the data type
			 */
			Json::Kind KindDetect();

			/**
			 * \brief Concat the current \em character to \em token.
			 */
			void Concat() { token += character; }

			/**
			 * \brief Scan backward a step(one character distance).
			 */
			void Retract() { pos--; character = ' '; }

			/**
			 * \brief Whether it is the end-of-line.
			 * @return true if EOL.
			 */
			bool EOL() const { return '\0' == character; }

			/**
			 * \brief Scan forward a step(one character distance).
			 * @return the character after current \em position, i.e. the character after steping forward
			 */
			char NextCharacter()
			{
				if (pos >= 0 && '\0' == *(source + pos)) { UnexpectedToken(); }
				character = *(source + (++pos));
				return character;
			}

			/**
			 * \brief Skip all the white spcaes(' ', \t, \r, \n) from \em position.
			 */
			void SkipWhitespaces();

			/**
			 * \brief Throw UnexpectedTokenException to indicate an unexpected token in \em source.
			 */
			void UnexpectedToken();
		};

		/**
		 * \brief Exception indicates syntax error of \em source.
		 */
		struct UnexpectedTokenException : std::exception
		{
		public:
			UnexpectedTokenException(char ch, int pos);
			virtual ~UnexpectedTokenException() throw();
			const char* what() const throw();
		private:
			char 		ch_; 	///< which character cause syntax error
			int  		pos_;	///< where this character locates
			std::string msg_;	///< error message
		};

		/**
		 * \brief Exception indicates there is a bad conversion.
		 *
		 * \code{.cpp}
		 * Json a(12.3);
		 * a.AsString(); 	// bad conversion
		 * a.AsBool();   	// bad conversion
		 * a.AsInt();		// fine
		 * a.AsDouble();	// fine
		 * Json b("hello");
		 * b.AsString(); 	// fine
		 * b.AsInt();		// bad conversion
		 * b.AsBool(); 		// bad conversion
		 * \endcode
		 */
		struct BadConversionException : std::exception
		{
		public:
			const char* what() const throw() { return "a bad conversion"; }
		};

		/**
		 * \brief Delete Json object in an array (vector)
		 */
		static void DestroyArrayData(ArrayData& arr);

		/**
		 * \brief Delete Json object in an object (map)
		 */
		static void DestroyObjectData(ObjectData& obj);

		/**
		 * \brief Construct from a Json object pointed by /em rhs
		 */
		explicit Json(Json* rhs);

		/**
		 * \brief Consturct from a map
		 */
		explicit Json(ObjectData* obj);

		/**
		 * \brief Construct from a vector
		 */
		explicit Json(ArrayData* arr);

		/**
		 * \brief Deep copy work.
		 */
		void DoDeepCopy(const Json& rhs);

		/**
		 * \brief Release memory of a Json object.
		 */
		void Release();

		/**
		 * \brief Convert data to specified type. Return the reference.
		 */
		template <typename ToType>
		ToType& Data()
		{
			return const_cast<ToType&>(static_cast<const Json&>(*this).Data<ToType>());
		}

		/**
		 * \brief Convert data to specified type. Return the reference.
		 */
		template <typename ToType>
		const ToType& Data() const
		{
			bool okay;
			switch(kind_)
			{
			case kNumber: okay = (typeid(double) == typeid(ToType)); break;
			case kString: okay = (typeid(std::string) == typeid(ToType)); break;
			case kBool: okay = (typeid(bool) == typeid(ToType)); break;
			case kArray: okay = (typeid(ArrayData) == typeid(ToType)); break;
			case kObject: okay = (typeid(ObjectData) == typeid(ToType)); break;
			default: okay = false;
			}
			if (!okay) throw BadConversionException();
			return *static_cast<ToType*>(data_);
		}
		
		Kind kind_;		///< which kind of data this Json object represents
		void *data_;	///< the real data held by the Json object
	};

}





namespace ggicci
{
using namespace std;

/* Json */
Json Json::Parse(const char* json_string)
{
	Parser parser(json_string);
	Json *json = parser.ConsumeValue(false);
	Json retval(json);
	delete json;
	return retval;
}

Json::Json() : kind_(kNull), data_(0) { }
Json::Json(int num) : kind_(kNumber), data_(new double(num)) { }
Json::Json(double num) : kind_(kNumber), data_(new double(num)) { }
Json::Json(const string& str) : kind_(kString), data_(new string(str)) { }
Json::Json(const char* str) : kind_(kString), data_(new string(str)) { }
Json::Json(bool boo) : kind_(kBool), data_(new bool(boo)) { }
Json::Json(const Json& rhs) : kind_(kNull), data_(0)
{
	TRACK("Json::Json(const Json& rhs)");
	DoDeepCopy(rhs);
}

Json& Json::operator = (const Json& rhs)
{
	TRACK("Json& Json::operator = (const Json& rhs)");
	if (this == &rhs) { return *this; }
	Release();
	DoDeepCopy(rhs);
	return *this;
}

Json::Json(Json* rhs) : kind_(rhs->kind_), data_(rhs->data_)
{
	TRACK("Json::Json(Json* rhs)");
	rhs->data_ = 0;
}

Json::Json(ObjectData* obj) : kind_(kObject), data_(obj) { }
Json::Json(ArrayData* arr) : kind_(kArray), data_(arr) { }

void Json::DoDeepCopy(const Json& rhs)
{
	TRACK("void Json::DoDeepCopy(const Json& rhs)");
	kind_ = rhs.kind_;
	switch (kind_)
	{
		case kNull: data_ = 0; break;
		case kNumber: data_ = new double(*static_cast<const double*>(rhs.data_)); break;
		case kString: data_ = new string(*static_cast<const string*>(rhs.data_)); break;
		case kBool: data_ = new bool(*static_cast<const bool*>(rhs.data_)); break;
		case kArray:
		{
			ArrayData *tmp = new ArrayData();
			const ArrayData& data = *CAST_JSON_ARR(rhs.data_);
			ArrayData::const_iterator cit = data.begin();
			for (; cit != data.end(); ++cit)
			{
				tmp->push_back(new Json(*(*cit)));
			}
			data_ = tmp;
			break;
		}
		case kObject:
		{
			ObjectData *tmp = new ObjectData();
			const ObjectData& data = *CAST_JSON_OBJ(rhs.data_);
			ObjectData::const_iterator cit = data.begin();
			for (; cit != data.end(); ++cit)
			{
				tmp->insert(make_pair(cit->first, new Json(*cit->second)));
			}
			data_ = tmp;
			break;
		}
		default: break;
	}
}

void Json::Release()
{
	TRACK("void Json::Release()");
	TRACK("----------------------------------- Delete[" << kind_ << "]: " << ToString());
	switch (kind_)
	{
		case kNumber: delete static_cast<double*>(data_); break;
		case kString: delete static_cast<string*>(data_); break;
		case kBool: delete static_cast<bool*>(data_); break;
		case kObject:
		{
			Json::DestroyObjectData(*CAST_JSON_OBJ(data_));
			delete CAST_JSON_OBJ(data_);
			break;
		}
		case kArray:
		{
			Json::DestroyArrayData(*CAST_JSON_ARR(data_));
			delete CAST_JSON_ARR(data_);
			break;
		}
		default: break;
	}
	data_ = 0;
	kind_ = kNull;
}

Json::~Json()
{
	TRACK("~Json");
	if (!data_) { return; }
	Release();
}

bool Json::IsEmpty() const
{
	if (IsObject()) { return CAST_JSON_OBJ(data_)->size() == 0; }
	if (IsArray()) { return CAST_JSON_ARR(data_)->size() == 0; }
	return false;
}

bool Json::Contains(const char* key) const
{
	if (!IsObject()) { return false; }
	const ObjectData &data = *CAST_JSON_OBJ(data_);
	return data.find(key) != data.end();
}

int Json::Size() const
{
	if (!IsArray()) { return 1; } 
	return CAST_JSON_ARR(data_)->size();
}

vector<std::string> Json::Keys() const
{
	vector<string> keys;
	if (IsObject())
	{
		ObjectData *data = CAST_JSON_OBJ(data_);
		ObjectData::const_iterator cit = data->begin();
		for (; cit != data->end(); ++cit)
		{
			keys.push_back(cit->first);
		}
	}
	return keys;
}

Json& Json::Push(const Json& rhs)
{
	TRACK("Json& Json::Push(const Json& rhs)");
	switch (kind_)
	{
		case kArray:
		{
			ArrayData *data = CAST_JSON_ARR(data_);
			data->push_back(new Json(rhs));
			break;
		}
		case kNumber: case kString: case kBool: case kNull: case kObject:
		{
			Json* old = new Json(this);
			kind_ = Json::kArray;
			ArrayData *tmp = new ArrayData();
			tmp->push_back(old);
			tmp->push_back(new Json(rhs));
			data_ = tmp;
			break;
		}
		default: break;
	}
	return *this;
}

Json& Json::AddProperty(const std::string& key, const Json& value)
{
	TRACK("Json& Json::AddProperty(const std::string& key, const Json& value)");
	(*this)[key.c_str()] = value;
	return *this;
}

Json& Json::Remove(const std::string& key)
{
	TRACK("Json& Json::Remove(const std::string& key)");
	ObjectData& data = Data<ObjectData>();
	ObjectData::iterator it = data.find(key);
	if (it != data.end())
	{
		delete it->second;
		data.erase(it);
	}
	return *this;
}

void Json::Remove(int index)
{
	TRACK("void Json::Remove(int index)");
	ArrayData& data = Data<ArrayData>();
	if (index >= 0 && index < Size())
	{
		ArrayData::iterator it = data.begin() + index;
		delete (*it);
		data.erase(it);
	}
}

int Json::AsInt() const
{
	return (int)Data<double>();
}

double Json::AsDouble() const
{
	return Data<double>();
}

bool Json::AsBool() const
{
	return Data<bool>();
}

string Json::AsString() const
{
	return Data<string>().c_str();
}

const Json& Json::operator [] (int index) const
{
	return *Data<ArrayData>()[index];
}

Json& Json::operator [] (int index)
{
	return *Data<ArrayData>()[index];
}

const Json& Json::operator[] (const char* key) const
{
	ObjectData& data = const_cast<ObjectData&>(Data<ObjectData>());
	if (data.find(key) == data.end()) { data[key] = new Json(); }
	return *(data[key]);
}

Json& Json::operator[] (const char* key)
{
	return const_cast<Json&>(static_cast<const Json&>(*this)[key]);
}

Json& Json::operator = (int num)
{
	return operator = ((double)num);
}

Json& Json::operator = (double num)
{
	Release();
	kind_ = kNumber;
	data_ = new double(num);
	return *this;
}

Json& Json::operator = (const string& str)
{
	return operator = (str.c_str());
}

Json& Json::operator = (const char* str)
{
	Release();
	kind_ = kString;
	data_ = new string(str);
	return *this;
}

Json& Json::operator = (bool boo)
{
	Release();
	kind_ = kBool;
	data_ = new bool(boo);
	return *this;
}

string Json::ToString() const
{
	ostringstream oss;
	switch (kind_)
	{
		case kNumber: oss << *static_cast<double*>(data_); break;
		case kString: oss << "\"" << *static_cast<string*>(data_) << "\""; break;
		case kBool: oss << boolalpha << *static_cast<bool*>(data_); break;
		case kNull: oss << "null"; break;
		case kObject:
		{
			ObjectData& data = *CAST_JSON_OBJ(data_);
			ObjectData::const_iterator cit = data.begin();
			oss << "{ ";
			bool unique = true;
			for (; cit != data.end(); ++cit)
			{
				oss << (unique ? "\"" : ", \"") << cit->first << "\": " << cit->second->ToString();
				unique = false;
			}
			oss << " }";
			break;
		}
		case kArray:
		{
			ArrayData& data = *CAST_JSON_ARR(data_);
			ArrayData::const_iterator cit = data.begin();
			oss << "[ ";
			bool unique = true;
			for (; cit != data.end(); ++cit)
			{
				oss << (unique ? "" : ", ") << (*cit)->ToString();
				unique = false;
			}
			oss << " ]";
			break;
		}
		default : break;
	}
	return oss.str();
}

/* Private Members */
void Json::DestroyArrayData(ArrayData& arr)
{
	TRACK("void Json::DestroyArrayData(ArrayData& arr)");
	for (ArrayData::iterator it = arr.begin(); it != arr.end(); ++it)
	{
		delete *it;
		*it = 0;
	}
}

void Json::DestroyObjectData(ObjectData& obj)
{
	TRACK("void Json::DestroyObjectData(ObjectData& obj)");
	for (ObjectData::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		delete it->second;
		it->second = 0;
	}
}

/* Json::Parser */
Json::Parser::Parser(const char* json_string)
{
	source = json_string;
	pos = -1;
	character = ' ';
	token = "";
}

Json* Json::Parser::ConsumeValue(bool section/* = true */)
{
	TRACK("Json Json::Parser::ConsumeValue()");
	SkipWhitespaces();
	Kind kind = KindDetect();
	Json* (Json::Parser::*consumer)(); // member function pointer
	switch (kind)
	{
		case kNumber: { consumer = &Json::Parser::ConsumeNumber; break; }
		case kString: { consumer = &Json::Parser::ConsumeString; break; }
		case kBool: { consumer = &Json::Parser::ConsumeBool; break; }
		case kNull: { consumer = &Json::Parser::ConsumeNull; break; }
		case kObject: { consumer = &Json::Parser::ConsumeObject; break; }
		case kArray: { consumer = &Json::Parser::ConsumeArray; break; }
		default: { throw; break; }
	}
	Json *json = (this->*consumer)();
	if (!section && !EOL())
	{
		SkipWhitespaces();
		NextCharacter();
		if (!EOL()) { UnexpectedToken(); }
	}
	if (section) { SkipWhitespaces(); }
	return json;
} // end fn:ConsumeValue

Json* Json::Parser::ConsumeNumber()
{
	TRACK("Json* Json::Parser::ConsumeNumber()");
	token = "";
	NextCharacter();
	// negative
	if ('-' == character) { Concat(); NextCharacter(); }
	bool loop = true;
	bool dot = false;
	if (!isdigit(character)) { UnexpectedToken(); }
	Concat(); // the first digit or after '-'
	if ('0' == character) {  loop = false; }
	while (loop) // * loop
	{
		if (!isdigit(NextCharacter())) { Retract(); break; }
		Concat();
	}
	if (isdigit(NextCharacter())) { UnexpectedToken(); } // fix 000.3
	if ('.' == character) { Concat(); dot = true; }
	if (dot) // met '.', at least need one digit
	{
		if (!isdigit(NextCharacter())) { UnexpectedToken(); }
		Concat();
	}
	while (dot) // * loop
	{
		NextCharacter();
		if (!isdigit(character)) { break; }
		Concat();
	}
	// confront with scientific notation
	if ('e' == character || 'E' == character)
	{
		Concat();
		NextCharacter();
		if ('+' == character || '-' == character) { Concat(); }
		else if (isdigit(character)) { Retract(); }
		else { UnexpectedToken(); }
		// at least need one digit after '+' or '-' or 'E' or 'e'
		if (!isdigit(NextCharacter())) { UnexpectedToken(); }
		Concat();
		while (true)
		{
			NextCharacter();
			// if (EOL() || isspace(character)) { break; }
			if (!isdigit(character)) { Retract(); break; }
			Concat();
		}
	}
	else
	{
		Retract();
	}
	// else if (EOL() || isspace(character)) { ; } 
	// else { UnexpectedToken(); } // fix -23.0s
	return new Json(atof(token.c_str()));
} // end fn:ConsumeNumber

Json* Json::Parser::ConsumeString()
{
	TRACK("Json* Json::Parser::ConsumeString()");
	SkipWhitespaces();
	// consume the open quote
	if ('\"' != NextCharacter()) { UnexpectedToken(); }
	token = "";
	while (true)
	{
		// meet the close quote, end loop
		if ('\"' == NextCharacter()) { break; }
		// the escape characters
		if ('\\' == character)
		{
			Concat();
			NextCharacter();
			switch (character)
			{
				case '\"': case '\\': case '/': case 'b':
				case 'f': case 'n': case 'r': case 't':
				{
					Concat();
					break;
				}
				case 'u':
				{
					Concat();
					for (int i = 0; i < 4; ++i)
					{
						if (!isxdigit(NextCharacter()))
						{
							UnexpectedToken();
						}
						Concat();
					}
					break;
				}
				default:
				{
					UnexpectedToken();
					break;
				}
			}
		}
		else // if not a escape character
		{
			if (iscntrl(character)) { UnexpectedToken(); }
			Concat();
		}
	}
	return new Json(token);
} // end fn:ConsumeString

Json* Json::Parser::ConsumeBool()
{
	TRACK("Json* Json::Parser::ConsumeBool()");
	char ch = NextCharacter();
	Retract();
	if ('t' == ch) { ConsumeSpecific("true"); return new Json(true); }
	else { ConsumeSpecific("false"); return new Json(false); }
} // end fn:ConsumeBool

Json* Json::Parser::ConsumeNull()
{
	TRACK("Json* Json::Parser::ConsumeNull()");
	ConsumeSpecific("null");
	return new Json();
} // end fn:ConsumeNull

Json* Json::Parser::ConsumeObject()
{
	TRACK("Json* Json::Parser::ConsumeObject()");
	ObjectData *obj = new ObjectData();
	if ('{' != NextCharacter()) { UnexpectedToken(); }
	SkipWhitespaces();
	if ('}' == NextCharacter()) { ; } // empty object
	else // at least need one pair
	{
		Retract();
		try { obj->insert(ConsumePair()); }
		catch (exception& e)
		{
			Json::DestroyObjectData(*obj);
			delete obj;
			UnexpectedToken();
		}
		while (true) // * loop
		{
			if ('}' == NextCharacter()) { break; }
			else if (',' == character)
			{ 
				try { obj->insert(ConsumePair()); }
				catch (exception& e)
				{
					Json::DestroyObjectData(*obj);
					delete obj;
					UnexpectedToken();
				}
			}
			else
			{
				Json::DestroyObjectData(*obj);
				delete obj;
				UnexpectedToken();
			}
		}
	}
	return new Json(obj);
} // end fn:ConsumeObject

Json* Json::Parser::ConsumeArray()
{
	TRACK("Json* Json::Parser::ConsumeArray()");
	ArrayData *arr = new ArrayData();
	if ('[' != NextCharacter()) { UnexpectedToken(); }
	SkipWhitespaces();
	if (']' == NextCharacter()) { ; } // empty array
	else // at least one value
	{
		Retract();
		try { arr->push_back(ConsumeValue()); }
		catch (exception& e)
		{
			Json::DestroyArrayData(*arr);
			delete arr;
			UnexpectedToken();
		}
		while (true) // * loop
		{
			if (']' == NextCharacter()) { break; }
			else if (',' == character) {
				try { arr->push_back(ConsumeValue()); }
				catch(exception& e)
				{
					Json::DestroyArrayData(*arr);
					delete arr;
					UnexpectedToken();
				}
			}
			else
			{
				Json::DestroyArrayData(*arr);
				delete arr;
				UnexpectedToken();
			}
		}
	}
	return new Json(arr);
} // end fn: Consume Array

Json::Pair Json::Parser::ConsumePair()
{
	TRACK("Json::Parser::Pair Json::Parser::ConsumePair()");
	Json* pkey = ConsumeString();
	string key(pkey->AsString());
	delete pkey;
	SkipWhitespaces();
	ConsumeSpecific(":");
	Json *value = ConsumeValue();
	return make_pair(key, value);
} // end fn:ConsumePair

void Json::Parser::ConsumeSpecific(const char* str)
{
	TRACK("void Json::Parser::ConsumeSpecific(const char* str)");
	int len = char_traits<char>::length(str);
	for (int i = 0; i < len; ++i)
	{
		if (str[i] != NextCharacter()) { UnexpectedToken(); }
	}
} // end fn:ConsumeSpeciffic

Json::Kind Json::Parser::KindDetect()
{
	Json::Kind kind = Json::kNumber;
	switch (NextCharacter())
	{
		case '\"': { kind = Json::kString; break; }
		case 't': case 'f': { kind = Json::kBool; break; }
		case 'n': { kind = Json::kNull; break; }
		case '{': { kind = Json::kObject; break; }
		case '[': { kind = Json::kArray; break; }
		default: break; // treat as number (will cause exception)
	}
	Retract();
	return kind;
}

void Json::Parser::SkipWhitespaces()
{
	char ch;
	while (true)
	{
		ch = NextCharacter();
		if (' ' == ch || '\t' == ch || '\n' == ch || '\r' == ch) { ; }
		else { pos--; break; }
	}
}

void Json::Parser::UnexpectedToken() { throw Json::UnexpectedTokenException(character, pos); }

/* Json::UnexpectedTokenException */
Json::UnexpectedTokenException::UnexpectedTokenException(char ch, int pos)
	:exception(), ch_(ch), pos_(pos)
{
	ostringstream oss;
	if ('\0' == ch_)
	{
		oss << "SyntaxError: Unexpected end of input";
	}
	else
	{
		oss << "SyntaxError: Unexpected token ";
		if (isgraph(ch_)) { oss << ch_; }
		else { oss << (int)ch_ << "(ASCII)"; }
		oss << " at pos " << pos_;
	}
	msg_ = oss.str();
}

Json::UnexpectedTokenException::~UnexpectedTokenException() throw() {}

const char* Json::UnexpectedTokenException::what() const throw()
{
	return msg_.c_str();
}

}

#endif // GGICCI_JSONLA_H_