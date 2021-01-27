/**
 * \file pile.h
 * \brief gestion d'une pile
 * \author NM
 * \version 0.1
 * \date 11/12/2015
 *
 */

#ifndef __PILE_H__
#define __PILE_H__


/** pile contenant des entiers */
/**
 * \struct TIntPile
 * \brief structure contenant une pile d'entiers
 */
typedef struct {
    int * data; /**< tableau d'entiers representant la pile */
    int indexSommet; /**< indice du sommet */
    int size; /**< taille en mémoire de la pile */
} TIntPile;


TIntPile * initIntPile();
void deleteIntPile(TIntPile ** _pile);
void printIntPile(TIntPile * _pile);
void empilerInt(TIntPile * _pile, int _val);
int depilerInt(TIntPile * _pile);
int sommetInt(TIntPile * _pile);

/** pile contenant des pointeurs sur des objets */

/**
 * \struct TVoidPile
 * \brief structure contenant une pile de pointeur void *
 */
typedef struct {
    void ** data; /**< tableau de pointeur void * */
    int indexSommet; /**< indice du sommet */
    int size;/**< taille en memoire de la pile */
} TVoidPile;


TVoidPile * initVoidPile();
void deleteVoidPile(TVoidPile ** _pile);
void printVoidPile(TVoidPile * _pile);
void empilerVoid(TVoidPile * _pile, void * _val);
void * depilerVoid(TVoidPile * _pile);
void * sommetVoid(TVoidPile * _pile);

#endif
