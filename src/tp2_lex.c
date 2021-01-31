/**
 * \file tp2_lex.c
 * \brief analyseur lexical pour le langage JSON
 * \author NM
 * \version 0.1
 * \date 25/11/2015
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <regex.h>
#include "../include/tp2_lex.h"

char *separators = "{}[],;\n \t";

/**
 * \fn int isSep(char _symb)
 * \brief fonction qui teste si un symbole fait partie des separateurs
 *
 * \param[in] _symb symbole a analyser
 * \return 1 (vrai) si _symb est un separateur, 0 (faux) sinon
 */

int isSep(const char _symb)
{

    if (_symb == '{' || _symb == '[' || _symb == '}' || _symb == ']' || _symb == ',' || _symb == ':' || _symb == ' ' || _symb == '\t' || _symb == '\n' || _symb == '\r')
    {
        return 1;
    }
    return 0;
}

/**
 * \fn TLex * initLexData(char * _data)
 * \brief fonction qui reserve la memoire et initialise les
 * donnees pour l'analyseur lexical
 *
 * \param[in] _data chaine a analyser
 * \return pointeur sur la structure de donnees creee
 */
TLex *initLexData(char *_data)
{
    TLex *lexData = malloc(sizeof(TLex));
    lexData->data = _data;
    lexData->startPos = _data;
    lexData->nbLignes = 1;
    lexData->tableSymboles = NULL;
    lexData->nbSymboles = 0;
    lexData->tailleTableSymboles = 0;

    return lexData;
}

/**
 * \fn void deleteLexData(TLex ** _lexData)
 * \brief fonction qui supprime de la memoire les donnees pour
 * l'analyseur lexical
 *
 * \param[inout] _lexData donnees de l'analyseur lexical
 * \return neant
 */
void deleteLexData(TLex **_lexData)
{
    //free((*_lexData)->data);
    //free((*_lexData)->tableSymboles);
    if ((*_lexData) && (*_lexData)->tableSymboles)
    {
        for (int i = 0; i < (*_lexData)->nbSymboles; ++i)
        {
            if ((*_lexData)->tableSymboles[i].type == JSON_STRING)
            {
                free((*_lexData)->tableSymboles[i].val.chaine);
            }
        }
        free((*_lexData)->tableSymboles);
        free(*_lexData); 
    }
}

/**
 * \fn void printLexData(TLex * _lexData)
 * \brief fonction qui affiche les donnees pour
 * l'analyseur lexical
 *
 * \param[in] _lexData données de l'analyseur lexical
 * \return neant
 */
void printLexData(TLex *_lexData)
{
    printf("LexData\ndata: \"%s\"\nstartPos: \"%s\"\nnbLignes: %d\nnbSymboles: %d\ntableSymboles :\n", _lexData->data, _lexData->startPos, _lexData->nbLignes, _lexData->nbSymboles);
    for (int i = 0; i < _lexData->nbSymboles; i++)
    {

        switch (_lexData->tableSymboles[i].type)
        {
        case JSON_STRING:
            printf("\tChaine\t: %s\n", _lexData->tableSymboles[i].val.chaine);
            break;

        case JSON_INT_NUMBER:
            printf("\tEntier\t: %d\n", _lexData->tableSymboles[i].val.entier);
            break;

        case JSON_REAL_NUMBER:
            printf("\tReel\t: %f\n", _lexData->tableSymboles[i].val.reel);
            break; // oh bosse la paul
        }
    }
}

/**
 * \fn int changeTableSymboleSize(TLex *_lexData)
 * \brief Fonction qui incrémente le nombre de symbole de la table des symbole et augmente sa taille
 * 
 * \param[inout] _lexData donnees de l'analyseur lexical
 * \return reallocation reussie
 */
int changeTableSymboleSize(TLex *_lexData)
{
    _lexData->tailleTableSymboles += sizeof(TSymbole);
    _lexData->tableSymboles = realloc(_lexData->tableSymboles, _lexData->tailleTableSymboles * 2);

    return _lexData->tableSymboles ? 0 : 1;
}

/**
 * \fn void addIntSymbolToLexData(TLex * _lexData, const int _val)
 * \brief fonction qui ajoute un symbole entier a la table des symboles
 *
 * \param[inout] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur entiere e ajouter
 * \return neant
 */
void addIntSymbolToLexData(TLex *_lexData, const int _val)
{

    if (!changeTableSymboleSize(_lexData))
    {
        _lexData->tableSymboles[_lexData->nbSymboles].type = JSON_INT_NUMBER;
        _lexData->tableSymboles[_lexData->nbSymboles].val.entier = _val;
        ++_lexData->nbSymboles;
    }
}

/**
 * \fn void addRealSymbolToLexData(TLex * _lexData, const float _val)
 * \brief fonction qui ajoute un symbole reel a la table des symboles
 *
 * \param[inout] _lexData donnees de l'analyseur lexical
 * \param[in] _val valeur reelle a ajouter
 */
void addRealSymbolToLexData(TLex *_lexData, const float _val)
{
    if (!changeTableSymboleSize(_lexData))
    {
        _lexData->tableSymboles[_lexData->nbSymboles].type = JSON_REAL_NUMBER;
        _lexData->tableSymboles[_lexData->nbSymboles].val.reel = _val;
        ++_lexData->nbSymboles;
    }
}

/**
 * \fn void addStringSymbolToLexData(TLex * _lexData, char * _val)
 * \brief fonction qui ajoute une chaine de caracteres a la table des symboles
 *
 * \param[inout] _lexData donnees de l'analyseur lexical
 * \param[in] _val chaine a ajouter
 */
void addStringSymbolToLexData(TLex *_lexData, char *_val)
{
    if (!changeTableSymboleSize(_lexData))
    {
        _lexData->tableSymboles[_lexData->nbSymboles].type = JSON_STRING;
        _lexData->tableSymboles[_lexData->nbSymboles].val.chaine = _val;
        ++_lexData->nbSymboles;
    }
}

/**
 * \fn int lex(Lex * _lexData)
 * \brief fonction qui effectue l'analyse lexicale (contient le code l'automate fini)
 *
 * \param[inout] _lexData donnees de suivi de l'analyse lexicale
 * \return code d'identification de l'entite lexicale trouvee
 */
int lex(TLex *_lexData)
{
    //Caractère courant de la chaîne JSON
    char current;

    //Etat courant
    int state = 0;

    //Resultat de l'execution : erreur ou etatat terminal
    int ended = 0;

    //Variable qui définit si l'on doit passer au caractère suivant lors de l'execution
    //TRUE (1) par défaut, sera définie sur FALSE (0) si l'on tombe sur un séparateur
    int incrementStartPos = 1;

    //Chaîne de caratère qui contient l'objet JSON parcouru
    char *obj = NULL;

    //Taille de l'objet JSON parcouru
    int objSize = 0;

    //Parcours de la chaîne JSON tant qu'elle n'est pas vide
    //et qu'on est pas dans un état terminal
    while (ended == 0 && *_lexData->startPos != '\0')
    {
        current = *_lexData->startPos;

        switch (state)
        {
        case 0:
            switch (current)
            {
            case '\n':
                ++_lexData->nbLignes;
                break;

            case 't':
                state = 1;
                break;

            case 'f':
                state = 6;
                break;

            case 'n':
                state = 12;
                break;

            case '{':
                state = 17;
                ended = JSON_LCB;
                break;

            case '}':
                state = 18;
                ended = JSON_RCB;
                break;

            case '[':
                state = 19;
                ended = JSON_LB;
                break;

            case ']':
                state = 20;
                ended = JSON_RB;
                break;

            case ':':
                state = 21;
                ended = JSON_COLON;
                break;

            case ',':
                state = 22;
                ended = JSON_COMMA;
                break;

            case '"':
                state = 23;
                break;

            case '-':
                state = 27;
                break;

            case '0':
                state = 28;
                break;

            case '\t':
                break;

            default:
                if (current >= '1' && current <= '9')
                    state = 29;
                else if (isSep(current))
                {
                    break;
                }
                else
                    ended = JSON_LEX_ERROR;
                break;
            }

            break;

        case 1:
            if (current == 'r')
                state = 2;
            else
                ended = JSON_LEX_ERROR;
            break;
        case 2:
            if (current == 'u')
                state = 3;
            else
                ended = JSON_LEX_ERROR;
            break;

        case 3:
            if (current == 'e')
                state = 4;
            else
                ended = JSON_LEX_ERROR;
            break;

        case 4:
            //On change d'état si on à trouvé un séparateur
            if (isSep(current))
            {
                //On ne passe pas au caractère suivant car on veut pouvoir détecter si le séparateur
                //est une parenthèse, un crochet, un point virugle,...
                incrementStartPos = 0;

                state = 5;
                ended = JSON_TRUE;
            }
            else
                ended = JSON_LEX_ERROR;

            break;

        case 6:
            if (current == 'a')
            {
                state = 7;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 7:
            if (current == 'l')
            {
                state = 8;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 8:
            if (current == 's')
            {
                state = 9;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 9:
            if (current == 'e')
            {
                state = 10;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 10:

            //On change d'état si on à trouvé un séparateur
            if (isSep(current))
            {
                //On ne passe pas au caractère suivant car on veut pouvoir détecter si le séparateur
                //est une parenthèse, un crochet, un point virugle,...
                incrementStartPos = 0;

                state = 11;
                ended = JSON_FALSE;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 12:
            if (current == 'u')
            {
                state = 13;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 13:
            if (current == 'l')
            {
                state = 14;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 14:
            if (current == 'l')
            {
                state = 15;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 15:
            if (isSep(current))
            {
                //On ne passe pas au caractère suivant car on veut pouvoir détecter si le séparateur
                //est une parenthèse, un crochet, un point virugle,...
                incrementStartPos = 0;

                state = 16;
                ended = JSON_NULL;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 23:
            if (current == '"')
            {
                state = 26;
                ended = JSON_STRING;
            }
            else if (current == '\\')
            {
                state = 25;
            }

            break;

        case 25:
            state = 23;
            break;

        case 27:
            if (current == '0')
            {
                break;
            }
            else if (current >= '1' && current <= '9')
                state = 29;
            else
                ended = JSON_LEX_ERROR;
            break;

        case 28:
            if (current == '.')
            {
                state = 30;
            }
            else if (isSep(current))
            {
                //On ne passe pas au caractère suivant car on veut pouvoir détecter si le séparateur
                //est une parenthèse, un crochet, un point virugle,...
                incrementStartPos = 0;

                state = 31;
                ended = JSON_INT_NUMBER;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 29:
            if (current >= '0' && current <= '9')
            {
                break;
            }
            else if (current == '.')
            {
                state = 30;
            }
            else if (isSep(current))
            {
                //On ne passe pas au caractère suivant car on veut pouvoir détecter si le séparateur
                //est une parenthèse, un crochet, un point virugle,...
                incrementStartPos = 0;

                state = 31;
                ended = JSON_INT_NUMBER;
            }
            else if (toupper(current) == 'E')
            {
                state = 32;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 30:
            if (current >= '0' && current <= '9')
            {
                state = 33;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 31:
            if (current >= '0' && current <= '9')
            {
                state = 33;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 32:
            if (current == '+' || current == '-' || (current >= '0' && current <= '9'))
            {
                state = 35;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }

            break;

        case 33:
            if (current >= '0' && current <= '9')
            {
                break;
            }
            else if (toupper(current) == 'E')
            {
                state = 32;
            }
            else if (isSep(current))
            {
                //On ne passe pas au caractère suivant car on veut pouvoir détecter si le séparateur
                //est une parenthèse, un crochet, un point virugle,...
                incrementStartPos = 0;

                state = 34;
                ended = JSON_REAL_NUMBER;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }
            break;

        case 35:
            if (current >= '0' && current <= '9')
            {
                break;
            }
            else if (isSep(current))
            {
                //On ne passe pas au caractère suivant car on veut pouvoir détecter si le séparateur
                //est une parenthèse, un crochet, un point virugle,...
                incrementStartPos = 0;

                state = 36;
                ended = JSON_REAL_NUMBER;
            }
            else
            {
                ended = JSON_LEX_ERROR;
            }

            break;
        }

        //On ajoute le caractère courant à l'objet JSON
        if (state != 0)
        {
            obj = realloc(obj, sizeof(char) * (objSize + 1));
            obj[objSize] = current;
            ++objSize;
        }

        //On passe au caractère suivant si on est pas tombé sur un séparateur
        if (incrementStartPos)
            ++_lexData->startPos;
    }

    //Variable qui servira à copier l'objet JSON courant
    char *objCopy;

    //On ajoute le caractère courant à l'objet JSON
    obj = realloc(obj, sizeof(char) * (objSize + 1));
    obj[objSize] = '\0';
    ++objSize;

    switch (ended)
    {
    case JSON_STRING:
        //On insère une copie de la chaîne de caratère de l'objet JSON
        //(sinon ereur ligne 687 lors du free)
        //L'espace mémoire sera libéré lors de l'appelle à deleteLexData()
        objCopy = strdup(obj);
        addStringSymbolToLexData(_lexData, objCopy);
        break;

    case JSON_INT_NUMBER:
        addIntSymbolToLexData(_lexData, atoi(obj));
        break;

    case JSON_REAL_NUMBER:
        addRealSymbolToLexData(_lexData, atof(obj));
        break;
    }

    free(obj);

    return ended;
}