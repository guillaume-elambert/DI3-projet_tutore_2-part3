    
/**
 * \file json_tree.h
 * \brief implemente la mise en memoire d'un arbre json
 * \author NM
 * \version 0.1
 * \date 06.12.2014
 *
 */
 
#ifndef JSON_TREE_H
#define JSON_TREE_H

#define CSTE_JSON_NULL 2
#define CSTE_JSON_TRUE 1
#define CSTE_JSON_FALSE 0

/**
 * \struct JsonPair
 * \brief pour stocker une "pair" string : value
 */
typedef struct _json_pair {
	char * string; /**< name */
	struct _json_value_container * value; /**< value */
} JsonPair;

/**
 * \enum ValueType
 * \brief constantes pour représenter le type de données stockee dans "value"
 */
typedef enum _value_type {
	string,
	integer,
	real,
	object,
	array,
	constant /**< pour true, false et null */
} ValueType;
	
/**
 * \union JsonValue
 * \brief pour stocker "value"
 */
typedef union {
	char * string;
	int integer;
	float real;
	struct _json_object * object;
	struct _json_array * array;
	int constant;
} JsonValue;
	
/**
 * \struct JsonValueContainer
 * \brief pour stocker "value" et son type
 */
typedef struct _json_value_container {
	ValueType type; /**< type de valeur  */
	JsonValue value;
} JsonValueContainer;

/**
 * \struct JsonObject
 * \brief pour stoker un objet JSON
 */
typedef struct _json_object {
	JsonPair ** members; /**< liste des membres */
	int size; /**< nombre de membre */
} JsonObject;

/**
 * \struct JsonArray
 * \brief pour les tableau "array"
 */
typedef struct _json_array {
	JsonValueContainer ** elements; /**< liste des elements */
	int size; /**< nombre d'elements */
} JsonArray;

/**************************/
/*prototype des fonctions */

JsonArray * CreateJsonArray();
int InsertJsonArray(JsonArray * _array, JsonValueContainer * _valueContainer, const unsigned int _position);
int DeleteJsonArray(JsonArray ** _array);
char * PrintJsonArray(const JsonArray * _array);
char * PrintDotJsonArray(const JsonArray * _array);

JsonObject * CreateJsonObject();
int InsertJsonObject(JsonObject * _object, JsonPair * _pair);
JsonValueContainer * GetJsonValueContainer(const JsonObject * _object, const char * _string);
int DeleteJsonObject(JsonObject ** _object);
char * PrintJsonObject(const JsonObject * _object);
char * PrintDotJsonObject(const JsonObject * _object, int _id, int _idParent);

JsonValueContainer * CreateJsonValueContainer();
void UpdateJsonValueContainer(JsonValueContainer * _valueContainer, ValueType _type, JsonValue _value);
int DeleteJsonValueContainer(JsonValueContainer ** _valueContainer);
char * PrintJsonValueContainer(const JsonValueContainer * _valueContainer);

JsonPair * CreateJsonPair();
void UpdateJsonPair(JsonPair * _pair, char * _string, JsonValueContainer * _value);
int DeleteJsonPair(JsonPair ** _pair);
char * PrintJsonPair(const JsonPair *_pair);




#endif

