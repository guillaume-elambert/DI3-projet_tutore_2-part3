/**
 * \file json_tree.c
 * \brief implemente la mise en memoire d'un arbre json
 * \author NM
 * \version 0.2
 * \date 20.12.2018
 *
 */

#include <stdio.h> /* pour asprintf() */
#include <stdlib.h> /* pour reallocf()/realloc() */
/* rmq: The reallocf() function is identical to the realloc() function, except
 *    that it will free the passed pointer when the requested memory cannot be
 *    allocated.  This is a FreeBSD specific API designed to ease the problems
 *    with traditional coding styles for realloc causing memory leaks in
 *    libraries.
*/
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/param.h> /* pour que BSD soit definie si necessaire*/
#endif
#if !defined(BSD)
/* si reallocf n'existe pas, on l'ajoute
on pourrait aussi rediriger vers realloc():
#define reallocf(X,Y) realloc(X,Y)
*/
void * reallocf(void *ptr, size_t size) 
{
  void *nptr;
  nptr = realloc(ptr, size);
  if (!nptr && ptr && size != 0)
    free(ptr);
  return (nptr);
}
#endif
#include <string.h> /* pour strcmp() */
#include <assert.h> /* pour assert() */

#include "json_tree.h"

#define STRING_SIZE 128

/**
 * \fn CreateJsonArray
 * \brief creation en memoire d'un JsonArray
 * \return JsonArray*
 */
JsonArray * CreateJsonArray() {
    JsonArray * newArray = NULL;
    newArray = (JsonArray *)malloc(sizeof(JsonArray));
    newArray->elements = NULL;
    newArray->size = 0;
    return newArray;
}

/**
 * \fn InsertJsonArray
 * \brief Insere un JsonValueContainer a une position donnee
 * \param[in/out] _array : tableu json a modifier
 * \param[in] _valueContainer : element a inserer
 * \param[in] _position : position d'insertion dans le tableau (0 = premiere position, si _position>=size alors ajout a la fin)
 * \return <=0 : erreur, sinon la nouvelle taille de _array
 */
int InsertJsonArray(JsonArray * _array, JsonValueContainer * _valueContainer, const unsigned int _position) {
    int i=0;
    JsonValueContainer ** tabValueContainer = (JsonValueContainer**)reallocf(_array->elements,(_array->size + 1)*sizeof(JsonValueContainer*));
    if ( tabValueContainer == NULL ) {
        return 0;
    } else {
        _array->elements = tabValueContainer;
    }
    if ( _position < _array->size ) {
        i = _array->size;
        while ( i > _position ) {
            _array->elements[i] = _array->elements[i - 1];
            i--;
        }
    } else {
        i = _array->size;
    }
    _array->elements[i] = _valueContainer;
    _array->size++;
    return _array->size;
}

/**
 * \fn DeleteJsonArray
 * \brief libere la memoire d'un JsonArray
 * \param[in/out] JsonArray ** _array : JsonArray a effacer, vaudra NULL
 * \return <=0 : erreur, sinon 1
 */
int DeleteJsonArray(JsonArray ** _array) {
    int resultat = 1;
    int erreur = 0;
    while ( (*_array)->size ) {
        (*_array)->size--;
        erreur = DeleteJsonValueContainer(&((*_array)->elements[(*_array)->size]));
        if (erreur<=0) {
            resultat = erreur; /* il y a eu des erreurs */
        }
    }
    free((*_array)->elements);
    free(*_array);
    _array = NULL;
    return resultat;
}


/**
 * \fn PrintJsonArray
 * \brief construit une chaine de caracteres imprimable du contenu d'un JsonArray
 * \param[in] JsonArray * _array
 * \return char * : chaine représentant le JsonArray (a liberer ulterieurement)
 */
char * PrintJsonArray(const JsonArray * _array) {
    int i = 0;
    char * chaine = NULL;
    char * resultat = NULL;
    int nlle_taille = 0;
    resultat = strdup("[");
    
    while ( i < _array->size ) {
        chaine = PrintJsonValueContainer(_array->elements[i]);
        nlle_taille = strlen(resultat)+strlen(chaine)+1;
        resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
        strncat(resultat,chaine,nlle_taille);
        free(chaine);
        i++;
        if ( i < _array->size ) {
			nlle_taille = strlen(resultat)+2;
			resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
            strcat(resultat,",");
        }
    }
    nlle_taille = strlen(resultat)+2;
	resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
    strcat(resultat,"]");
    return resultat;
}

/**
 * \fn PrintDotJsonArray
 * \brief construit une chaine de caracteres imprimable du contenu d'un JsonArray
 * \param[in] JsonArray * _array
 * \return char * : chaine représentant le JsonArray (a liberer ulterieurement)
 */
char * PrintDotJsonArray(const JsonArray * _array) {
    int i = 0;
    char * chaine = NULL;
    char * resultat = NULL;
    int nlle_taille = 0;
    resultat = strdup("array [.");
    while (i<_array->size) {
        /*chaine = PrintQtreeJsonValueContainer(_array->elements[i]);*/
        chaine = strdup("print dot array note implemented yet !");
        nlle_taille = strlen(resultat)+strlen(chaine)+1;
        resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
        strncat(resultat,chaine,nlle_taille);
        free(chaine);
        i++;
        if (i<_array->size) {
			nlle_taille = strlen(resultat)+2;
			resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
            strcat(resultat," ");
        }
    }
    nlle_taille = strlen(resultat)+2;
	resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
    strcat(resultat,"]");
    return resultat;
}

/**
 * \fn CreateJsonObject
 * \brief construit un JsonObject vide
 * \return JsonObject *
 */
JsonObject * CreateJsonObject() {
    JsonObject * object = NULL;
    object = (JsonObject *)malloc(sizeof(JsonObject));
    object->members = NULL;
    object->size = 0;
    return object;
}

/**
 * \fn InsertJsonObject
 * \brief insere un nouveau JsonPair
 * \param[in/out] JsonObject * _object
 * \param[in] JsonPair * _pair
 * \return <=0 : erreur, sinon la nouvelle taille de _object
 */
int InsertJsonObject(JsonObject * _object, JsonPair * _pair) {
    int i=0;
    JsonPair** tabPair = (JsonPair**)reallocf(_object->members,(_object->size+1)*sizeof(JsonPair*));
    if ( tabPair == NULL ) {
        return 0;
    } else {
        _object->members = tabPair;
    }
    _object->members[_object->size] = _pair;
    _object->size++;
    return _object->size;
}

/**
 * \fn GetJsonValueContainer
 * \brief renvoie la valeur correspondant a une chaine
 * \param[in] const JsonObject * _object
 * \param[in] const char * _string
 * \return
 */
JsonValueContainer * GetJsonValueContainer(const JsonObject * _object, const char * _string) {
    int i = 0;
    JsonValueContainer * resultat = NULL;
    while ( i < _object->size ) {
        if ( !strcmp(_string,_object->members[i]->string) ) {
            resultat = _object->members[i]->value;
            i = _object->size;
        } else {
            i++;
        }
    }
    return resultat;
}

/**
 * \fn DeleteJsonObject
 * \brief libere la memoire occupee par un JsonObject
 * \param[in/out] JsonObject ** _object
 * \return <=0 : erreur, sinon 1
 */
int DeleteJsonObject(JsonObject ** _object) {
    int resultat = 1;
    int erreur = 0;
    while ( (*_object)->size ) {
        (*_object)->size--;
        erreur = DeleteJsonPair(&((*_object)->members[(*_object)->size]));
        if (erreur <= 0) {
            resultat = erreur;
        }
    }
    if ((*_object)->members!=NULL) {
		free((*_object)->members);
	}
    free(*_object);
    _object = NULL;
    return resultat;
}

/**
 * \fn PrintJsonObject
 * \brief construit une chaine de caracteres imprimable du contenu d'un JsonObject
 * \param[in] JsonObject * _object
 * \return char * : chaine représentant le JsonObject (a liberer ulterieurement)
 */
char * PrintJsonObject(const JsonObject * _object) {
    int i = 0;
    char * chaine = NULL;
    char * resultat = NULL;
    int nlle_taille;

    resultat = strdup("{");
    while ( i < _object->size ) {
        chaine = PrintJsonPair(_object->members[i]);
        nlle_taille = strlen(resultat)+strlen(chaine)+1;
        resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
        strncat(resultat, chaine,strlen(chaine));
        free(chaine);
        i++;
        if ( i < _object->size ) {

			nlle_taille = strlen(resultat)+2;
			resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
			strcat(resultat,",");
        }
    }
	nlle_taille = strlen(resultat)+2;
	resultat = (char*)realloc(resultat,nlle_taille*sizeof(char));
    strcat(resultat,"}");
    return resultat;
}

/**
 * \fn PrintDotJsonObject
 * \brief construit une chaine de caracteres imprimable du contenu d'un JsonObject
 * \param[in] JsonObject * _object
 * \param[in] _id id of the calling object
 * \return char * : chaine représentant le JsonObject (a liberer ulterieurement)
 */

char * PrintDotJsonObject(const JsonObject * _object, int _id, int _idParent) {
    int i = 0;
    char * chaine = NULL;
    char * resultat = NULL;
    static int idObject = 0;
    static int idPair = 0;
    char * chaine_id;
    int currentId;
    
    /* 2 cas : 1)l'objet est dans une paire et 2) le noeud a deja ete cree */
    if ( _id >= 0 ) {
        /* le noeud a deja ete cree */
        currentId = _id;
        resultat = strdup(" ");
        if ( resultat == NULL ) goto ERREUR_ALLOCATION;
    } else {
        /* le noeud n'a pas ete cree */
        int nbCar;
        currentId = idObject++;
        resultat = (char*)malloc(STRING_SIZE*sizeof(char));
        if ( resultat == NULL ) goto ERREUR_ALLOCATION;
        nbCar = sprintf(resultat,"object_%d [label=\"object\"];\n",currentId);
        if ( nbCar >= STRING_SIZE ) goto ERREUR_MEMOIRE;
        /* si il y a un parent on cree un arc */
        if ( _idParent >= 0 ) {
            chaine = (char*)malloc(STRING_SIZE*sizeof(char));
            if ( chaine == NULL ) goto ERREUR_ALLOCATION;
            nbCar = sprintf(chaine,"object_%d -- object_%d;\n",_idParent,currentId);
            if ( nbCar >= STRING_SIZE ) goto ERREUR_MEMOIRE;
            resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
            if ( resultat == NULL ) goto ERREUR_ALLOCATION;
            strcat(resultat,chaine);
        }
        free(chaine);
    }
    /* pour chaque membre de l'objet, on complete la chaine resultat*/
    while ( i < _object->size ) {
        int nbCar;
        int tailleChaine;
        switch( _object->members[i]->value->type ) {
            case string:
                /* creation du noeud pair_XX */
                idPair++;
                tailleChaine = STRING_SIZE+strlen(_object->members[i]->string)+strlen(_object->members[i]->value->value.string);
                chaine = (char*)malloc(tailleChaine*sizeof(char));
                if ( chaine == NULL ) goto ERREUR_ALLOCATION;
                nbCar = sprintf(chaine,"pair_%d [shape=box,label=\"%s=%s\"];\n",idPair,_object->members[i]->string,_object->members[i]->value->value.string);
                if ( nbCar >= tailleChaine ) goto ERREUR_MEMOIRE;
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                /* creation de l'arc */
                sprintf(chaine,"object_%d -- pair_%d;\n",currentId,idPair);
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                free(chaine);
                break;
            case integer:
                /* creation du noeud pair_XX */
                idPair++;
                tailleChaine = STRING_SIZE+strlen(_object->members[i]->string)+32;
                chaine = (char*)malloc(tailleChaine*sizeof(char));
                if ( chaine == NULL ) goto ERREUR_ALLOCATION;
                nbCar = sprintf(chaine,"pair_%d [shape=box,label=\"%s=%d\"];\n",idPair,_object->members[i]->string,_object->members[i]->value->value.integer);
                if ( nbCar >= tailleChaine ) goto ERREUR_MEMOIRE;
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                /* creation de l'arc */
                sprintf(chaine,"object_%d -- pair_%d;\n",currentId,idPair);
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                free(chaine);
                break;
            case real:
                /* creation du noeud pair_XX */
                idPair++;
                tailleChaine = STRING_SIZE+strlen(_object->members[i]->string)+32;
                chaine = (char*)malloc(tailleChaine*sizeof(char));
                if ( chaine == NULL ) goto ERREUR_ALLOCATION;
                nbCar = sprintf(chaine,"pair_%d [shape=box,label=\"%s=%f\"];\n",idPair,_object->members[i]->string,_object->members[i]->value->value.real);
                if ( nbCar >= tailleChaine ) goto ERREUR_MEMOIRE;
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                /* creation de l'arc */
                sprintf(chaine,"object_%d -- pair_%d;\n",currentId,idPair);
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                free(chaine);
                break;
            case object:
                /* creation du noeud object_XX */
                idObject++;
                tailleChaine = STRING_SIZE+strlen(_object->members[i]->string);
                chaine = (char*)malloc(tailleChaine*sizeof(char));
                if ( chaine == NULL ) goto ERREUR_ALLOCATION;
                nbCar = sprintf(chaine,"object_%d [label=\"object\\n%s\"];\n",idObject,_object->members[i]->string);
                if ( nbCar >= tailleChaine ) goto ERREUR_MEMOIRE;
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                /* creation de l'arc */
                sprintf(chaine,"object_%d -- object_%d;\n",currentId,idObject);
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                if ( resultat == NULL ) goto ERREUR_ALLOCATION;
                strcat(resultat,chaine);
                free(chaine);
                /* creation des sous-noeuds */
                chaine = PrintDotJsonObject(_object->members[i]->value->value.object,idObject,currentId);
                if ( chaine == NULL ) goto ERREUR_CHAINE_VIDE;
                resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+2)*sizeof(char));
                strcat(resultat,chaine);
                free(chaine);
                break;
            case array:
                break;
            case constant:
            break;
        }
        i++;
    }
    return resultat;
ERREUR_MEMOIRE:
    fprintf(stderr,"Erreur: chaine trop grande\n");
    exit(1);
ERREUR_CHAINE_VIDE:
    fprintf(stderr,"Erreur: chaine vide\n");
    exit(1);
ERREUR_ALLOCATION:
    fprintf(stderr,"Erreur: allocation impossible\n");
    exit(1);
}

/**
 * \fn CreateJsonValueContainer
 * \brief construit un JsonValueContainer vide
 * \param[in/out]
 * \return JsonValueContainer *
 */

JsonValueContainer * CreateJsonValueContainer() {
    JsonValueContainer * valueContainer;
    valueContainer = (JsonValueContainer*)malloc(sizeof(JsonValueContainer));
    valueContainer->type = integer;
    valueContainer->value.integer = 0;
    return valueContainer;
}

/**
 * \fn UpdateJsonValueContainer
 * \brief
 * \param[in/out] JsonValueContainer * _valueContainer
 * \param[in] ValueType _type
 * \param[in] JsonValue _value
 * \return
 */
void UpdateJsonValueContainer(JsonValueContainer * _valueContainer, ValueType _type, JsonValue _value) {
    _valueContainer->value = _value;
    _valueContainer->type = _type;
}

/**
 * \fn DeleteJsonValueContainer
 * \brief
 * \param[in/out]
 * \return <=0 : erreur, sinon 1
 */
int DeleteJsonValueContainer(JsonValueContainer ** _valueContainer) {
	int resultat = 1;
	if ( *_valueContainer!=NULL ) {
		switch ((*_valueContainer)->type ) {
			case string:
				free((*_valueContainer)->value.string);
				break;
			case integer:
				break;
			case real:
				break;
			case object:
				resultat = DeleteJsonObject(&((*_valueContainer)->value.object));
				break;
			case array:
				resultat = DeleteJsonArray(&((*_valueContainer)->value.array));
				break;
			case constant:
				break;
			default:
				break;
			}
	
		free(*_valueContainer);
		*_valueContainer = NULL;
    }
    return resultat;
}

/**
 * \fn PrintJsonValueContainer
 * \brief
 * \param[in/out]
 * \return
 */
char * PrintJsonValueContainer(const JsonValueContainer * _valueContainer) {
    char * resultat = NULL;
    int erreur;
    if (_valueContainer==NULL) {
		resultat = strdup("<empty value container>");
	} else {
		switch (_valueContainer->type ) {
			case string:
				resultat = strdup(_valueContainer->value.string);
				break;
			case integer:
				erreur = asprintf(&resultat,"%d",_valueContainer->value.integer);
				break;
			case real:
				erreur = asprintf(&resultat,"%f",_valueContainer->value.real);
				break;
			case object:
				resultat = PrintJsonObject(_valueContainer->value.object);
				break;
			case array:
				resultat = PrintJsonArray(_valueContainer->value.array);
				break;
			case constant:
				erreur = asprintf(&resultat,"%d",_valueContainer->value.constant);
				break;
			default:
				break;
			}
    }
    return resultat;
}

/**
 * \fn CreateJsonPair
 * \brief
 * \param[in/out]
 * \return
 */
JsonPair * CreateJsonPair() {
    JsonPair * pair = (JsonPair*)malloc(sizeof(JsonPair));
    pair->string = NULL;
    pair->value = NULL;
    return pair;
}

/**
 * \fn UpdateJsonPair
 * \brief
 * \param[in/out]
 * \return
 */
void UpdateJsonPair(JsonPair * _pair, char * _string, JsonValueContainer * _value) {
    /* attention une copie de la chaine est faite, il faudra la libérer */
    _pair->string = strdup(_string);
    _pair->value = _value;
}

/**
 * \fn DeleteJsonPair
 * \brief
 * \param[in/out]
 * \return  <=0 : erreur, sinon 1
 */
int DeleteJsonPair(JsonPair ** _pair) {
    int erreur = 0;
    int resultat = 0;
    if ((*_pair)->string!=NULL) {
		free((*_pair)->string);
	}
	if ((*_pair)->value!=NULL) {
		erreur = DeleteJsonValueContainer(&((*_pair)->value));
		if ( erreur <= 0 ) {
			resultat = erreur; /* il y a eu des erreurs */
		}
	}
    free(*_pair);
    *_pair = NULL;
    return resultat;
}

/**
 * \fn PrintJsonPair
 * \brief
 * \param[in/out]
 * \return
 */
char * PrintJsonPair(const JsonPair * _pair) {
    char * chaine;
    char * resultat;
    if (_pair->string==NULL) {
		resultat = strdup("<null string>");
	} else {
		resultat = strdup(_pair->string);
	}
    chaine = PrintJsonValueContainer(_pair->value);
    resultat = (char*)realloc(resultat,(strlen(resultat)+strlen(chaine)+3)*sizeof(char));
    strcat(resultat,":");
    strcat(resultat,chaine);
    free(chaine);
    return resultat;
}


/** code pour test */
#ifdef TEST
int main(void) {
	char * chaine;
	JsonObject * object1 = CreateJsonObject();
	JsonObject * object2 = CreateJsonObject();
	JsonArray * array1 = CreateJsonArray();
	JsonPair * pair1 = CreateJsonPair();
	JsonPair * pair2 = CreateJsonPair();
	JsonValue value1  = {.integer = 12};
	JsonValue value2 = {.object = object2};
	JsonValue value3 = {.array = array1};
	JsonValue value4 = {.string = strdup("essai de chaine")};
	
	JsonValueContainer * container1 = CreateJsonValueContainer();
	JsonValueContainer * container2 = CreateJsonValueContainer();
	JsonValueContainer * container3 = CreateJsonValueContainer();
	JsonValueContainer * container4 = CreateJsonValueContainer();
	
	UpdateJsonValueContainer(container1,integer,value1);
	UpdateJsonValueContainer(container2,object,value2);
	UpdateJsonValueContainer(container3,array,value3);
	UpdateJsonValueContainer(container4,string,value4);
	UpdateJsonPair(pair1,"test_pair 1",container1);
/* attention, si on insere une même ValueContainer dans plusieurs pair/tableau/... 
 * cela pose des problèmes lors de la liberation mémoire */
//	InsertJsonArray(array1,container1,0);
	InsertJsonArray(array1,container2,0);
	InsertJsonArray(array1,container4,0);
	UpdateJsonPair(pair2,"table 1",container3);
	InsertJsonObject(object1,pair1);
	InsertJsonObject(object1,pair2);
	                    
	chaine = PrintJsonObject(object1);                     
	printf("%s\n",chaine);
//	printf("%s\n",PrintDotJsonObject(object1,0,0));

	printf("Liberation de la mémoire :\n");
	free(chaine);
	DeleteJsonObject(&object1);
}
#endif


