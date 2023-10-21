#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

int id = 0;
link novoNo (int item, link l, link r) {
  link t = malloc(sizeof(struct node));
  t->item = item;
  t->l = l;
  t->r = r;
  t->N = 1;
  t->id = id++;
  return t;
}
Tree createTree(){
  Tree t = malloc (sizeof (struct tree));
  t->z = malloc (sizeof(struct node));
  t->head = t->z;
  t->z->l = t->z;
  t->z->N = 0;
  t->z->r = t->z;
  return t;
}

link rotR (Tree t, link h) {
  link y = h->l; // y aponta para a subarvore direita da raiz
  link x = y->r; // x aponta para o filho esquerdo de y

  y->r = h; // o filho esquerdo de y passa a ser a raiz
  h->l = x; // o filho direito de h passa a ser x

  //recalcula a altura dos nós que foram movimentados
  h->N = maior(alturaNo(h->l, t), alturaNo(h->r, t)) + 1;
  y->N = maior(alturaNo(y->l, t), alturaNo(y->l, t)) +1;
  return y;
}

link rotL (Tree t, link h) {
  link y = h->r; // y aponta para a subarvore direita da raiz
  link x = y->l; // x aponta para o filho esquerdo de y
  y->l = h; // o filho esquerdo de y passa a ser a raiz
  h->r = x; // o filho direito de h passa a ser x

  //recalcula a altura dos nós que foram movimentados
  h->N = maior(alturaNo(h->l, t), alturaNo(h->r, t)) + 1;
  y->N = maior(alturaNo(y->l, t), alturaNo(y->l, t)) +1;
  return y;
}

link rotLeftRight(link h, Tree t){
  h->l = rotL(t, h->l);
  return rotR(t, h);
}
link rotRightLeft(link h, Tree t){
  h->r = rotR(t, h->r);
  return rotL(t, h);
}

link searchR(Tree t, link h, int query) {
  if (h == t->z) {
    return NULL;
  }
  if(h->item == query) 
    return h;
  if (h->item >= query) 
    return searchR(t, h->l, query);
  return searchR(t, h->r, query);
} 

link search (Tree t, int query){
   return searchR(t, t->head, query);
}

link insertR (Tree t, link h, link n) {
  if (h == t->z) {
    return  n;
  }

  if(h->item >= n->item)
    h->l = insertR(t, h->l, n);
  else 
    h->r = insertR(t, h->r, n);
  (h->N)++;

  return h;
}

link insert (Tree t, int item){
  if(t->head == t->z) {
    return (t->head =novoNo(item, t->z, t->z));
  }
  return AVLinsertR(t->head, novoNo(item, t->z, t->z), item);
}

void imprimeEmOrdem (Tree t, link h){
  if(h == t->z) return;
  imprimeEmOrdem (t, h->l);
  printf("<chave: %d N: %d>", h->item, h->N); 
  imprimeEmOrdem (t, h->r);
}

void imprimePosOrdem (Tree t, link h){
  if(h == t->z) return;
  imprimePosOrdem (t, h->l);
  imprimePosOrdem (t, h->r);
  printf("<chave: %d N: %d>", h->item, h->N); 
}
void imprimePreOrdem (Tree t, link h, int k) {
  if(h == t->z) return;
  for(int i = 0; i <= k; i++)
    printf (" . ");
  printf("<chave: %d N: %d>\n", h->item, h->N); 
  imprimePreOrdem (t, h->l, k + 1); 
  imprimePreOrdem (t, h->r, k + 1);
}

void printnode (char  *x, int h) {
    int i;
    for(i = 0; i < h; i++)
        printf("\t");
    printf("%s\n", x);
}
int c = 0;
void imprimeR(Tree a, link t){
    char s[255];
    if(t==a->z) { return;}
    imprimeR(a,t->r);
    printf ("%d [ label = \"<f0>| <f1> %d (%d) | <f2>\" ]\n", t->id, t->item, t->N);
    if(t->l != a->z){
        printf("%d:f0->%d:f1;\n", t->id, t->l->id);
    } else {
        printf("z%d [ shape=point ];\n", c);
        printf("%d:f0->z%d;\n", t->id, c);
        c++;
    }
    if(t->r != a->z){
        printf("%d:f2->%d:f1;\n", t->id, t->r->id);
    } else {
        printf("z%d [ shape=point ];\n", c);
        printf("%d:f2->z%d;\n", t->id, c );
        c++;
    }
    imprimeR(a,t->l);
}
void imprime(Tree a, char *s) {
    c = 0;
    printf("digraph %s {\n", s);
    printf ("node [shape=record,style=rounded, height=.1]\n");
    imprimeR(a, a->head);
    printf("}\n");
}
void imprimeFromNode(Tree a, link h, char *s) {
  c = 0;
  printf("digraph  {\n" );
  printf ("name [label = \"%s\",shape=plaintext]\n", s);
  printf ("node [shape=record,style=rounded, height=.1]\n");
  imprimeR(a, h);
  printf("}\n");
}

int maior(int a, int b){
    return(a>b)? a: b;
}

int alturaNo(link h, Tree a){
    if (h == a->z)
        return -1;
    else   
        return h -> N;
}

int fatorDeBalanceamento(link h, Tree a){
    if(h)
      return (alturaNo(h->l, a) - alturaNo(h->r, a));
    else
      return 0;
}

link balancear(link h, Tree a){
  int fb = fatorDeBalanceamento(h, a);

  //rotação à esquerda
  if(fb < -1 && fatorDeBalanceamento(h->r, a) <= 0)
    h = rotL(a, h);

   //rotação à direita
  else if(fb < 1 && fatorDeBalanceamento(h->l, a) >= 0)
    h = rotR(a, h);

   //rotação dupla à esquerda
  else if(fb < 1 && fatorDeBalanceamento(h->l, a) < 0)
    h = rotLeftRight(h, a);

   //rotação dupla à direita
  else if(fb < -1 && fatorDeBalanceamento(h->r, a) > 0)
    h = rotRightLeft(h, a);

  return h;

}

link AVLinsertR(Tree t, link h, int item){
  if (h == t->z) {
    return  novoNo(item, h->l, h->r);
  }
  else{
    if(item < h->item)
      h->l = AVLinsertR(t, h->l, item);
    else if(item > h->item)
      h->r = AVLinsertR(t, h->r, item);
  }

  h->N= maior(alturaNo(h->l, t), alturaNo(h->r, t))+ 1;

  h = balancear(h, t);
  return h;
}

/*
Armazenar outras informações no nó.
Armazenar a altura da subárvore enraizada naquele nó. Isso permitirá que você calcule o fator de balanceamento.
ROTR e ROTL
As funções de rotação devem ser corrigidas para que seja possível rastrear a quantidade de nós na árvore e a altura da árvore
*/