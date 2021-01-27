/**
 * \file tp2_lex.h
 * \brief analyseur lexical pour le langage JSON
 * \author NM
 * \version 0.1
 * \date 25/11/2015
 *
 */

#ifndef JSON_LEX_ERROR

#define JSON_LEX_ERROR -1   /**< code d'erreur lexicale */
#define JSON_TRUE 1         /**< entite lexicale true */
#define JSON_FALSE 2        /**< entite lexicale false */
#define JSON_NULL 3         /**< entite lexicale null */
#define JSON_LCB 4          /**< entite lexicale { */
#define JSON_RCB 5          /**< entite lexicale } */
#define JSON_LB 6           /**< entite lexicale [ */
#define JSON_RB 7           /**< entite lexicale ] */
#define JSON_COMMA 8        /**< entite lexicale , */
#define JSON_COLON 9        /**< entite lexicale : */
#define JSON_STRING 10      /**<entite lexicale chaine de caracteres */
#define JSON_INT_NUMBER 11  /**< entite lexicale nombre entier */
#define JSON_REAL_NUMBER 12 /**< entite lexicale nombre reel */

/**
 * \union TSymbole
 * \brief union permettant de manipuler un entier/reel/chaine pour la table
 * des symboles
 */
typedef struct
{
    int type; /**< l'un des 3 types suivants : JSON_STRING/JSON_INT_NUMBER/
JSON_REAL_NUMBER */
    union
    {
        int entier;
        float reel;
        char *chaine;
    } val; /**< valeur associer a un element de la table des symboles */
} TSymbole;

/**
 * \struct TLex
 * \brief structure contenant tous les parametres/donnees pour
 * l'analyse lexicale
 */
typedef struct
{
    char *data;              /**< chaine a parcourir */
    char *startPos;          /**< position de depart pour la prochaine analyse */
    int nbLignes;            /**< nb de lignes analysees */
    TSymbole *tableSymboles; /**< tableau des symboles : chaines/entier/reel */
    int nbSymboles;          /**< nb de symboles stockes dans tableSymboles */
    int tailleTableSymboles; /**< taille memoire du tableau tableSymboles */
} TLex;

int isSep(const char _symb);
TLex *initLexData(char *_data);
void deleteLexData(TLex **_lexData);
void printLexData(TLex *_lexData);
int changeTableSymboleSize(TLex *_lexData);
void addIntSymbolToLexData(TLex *_lexData, const int _val);
void addRealSymbolToLexData(TLex *_lexData, const float _val);
void addStringSymbolToLexData(TLex *_lexData, char *_val);
int lex(TLex *_lexData);

#endif