/****************************************
Autor: J. Corredor
Correctores: Alejandro Caicedo y Harry Sánchez
Fecha: 28 de febrero 2024
Materia: Estructura de Datos
Tema: Implementación AVL 
****************************************/

/*Implementación de arbol de tipo AVL con funcionalidad de inserción, eliminación de nodos y autobalanceo. Además, impresion en InOrder, PreOrder y PostOrder*/

#include <iostream>
using namespace std;

//Definción de la clase nodo
class Nodo{
  public:
    int dato; //Valor del dato almacenado
    Nodo *left, *right;//Apuntadores a los nodos hijos
    int profundidad;//Nivel de profundiddad del nodo
};	

//Método constructor de la clase Nodo
Nodo *nuevoNodo(int dato){
  Nodo *nodo = new Nodo();
  nodo->dato = dato;
  nodo->left = NULL;
  nodo->right= NULL;
  nodo->profundidad = 1;
  return nodo;
}

//Método getter de la profundidad del nodo
int profundidad(Nodo *p){
  if(p==NULL)
    return 0;
  return p->profundidad;
}

//Función para encontrar menor nodo hijo a partir de un nodo dado
Nodo *minimoNodo(Nodo *nodo){
  Nodo *actual = nodo;
  while(actual->left != NULL)
    actual = actual->left;

  return actual;
}

/*Función para obtener el maximo valor entre 2 datos*/
int maxDato(int datoA, int datoB){
  return (datoA > datoB)? datoA : datoB;
}

//Función para encontrar el factor de balance de un nodo
int factorBalance(Nodo *nodo){
  if(nodo == NULL)
    return 0;
  return profundidad(nodo->left) - profundidad(nodo->right);
}
//Función que genera una right rotation a partir de un nodo dado
Nodo *rightRotacion(Nodo *yR){
  Nodo *xL = yR->left;//Se asigna el valor del nodo izquierdo a un nuevo nodo 
  Nodo *Temp = xL->right;//Se crea un nodo Temporal y se le asigna el valor del nodo derecho del anterior
  xL->right = yR;//Se intercambia el valor del nodo izquierdo por el del nodo original
  yR->left = Temp;//Se asigna el valor del nodo temporal al nodo izqueirdo
  yR->profundidad = maxDato(profundidad(yR->left), profundidad(yR->right)) + 1;//Se le asigna un valor de profundidad al nodo 
  xL->profundidad = maxDato(profundidad(xL->left), profundidad(xL->right)) + 1; //Se le asigna un valor de profundidad al nodo
  return xL;
}

//Función que genera una left rotation a paratir de un nodo dado (Funcionamiento similar a la anterior)
Nodo *leftRotacion(Nodo *xL){
  Nodo *yR = xL->right;
  Nodo *Temp = yR->left;
  yR->left = xL;
  xL->right = Temp;
  xL->profundidad = maxDato(profundidad(xL->left), profundidad(xL->right)) + 1; 
  yR->profundidad = maxDato(profundidad(yR->left), profundidad(yR->right)) + 1; 
  return yR;
}

//Inserta un nuevo nodo en el arbol dado un dato y un nodo (Generalmente la raiz) y balancea el arbol de acuerdo a su nuevo nodo 
Nodo *insertarNodo(Nodo *nodo, int dato){
  if(nodo == NULL)
    return nuevoNodo(dato);//Si el nodo (gerneralmente raíz) es nulo crea un o nuevo nodo con el dato dado
  if (dato < nodo->dato)
    nodo->left = insertarNodo(nodo->left, dato);//Si el dato dado es menor que el del nodo dado, lo posiciona a la izquierda del nodo 
  else if (dato > nodo->dato)//Similar a la anterior
    nodo->right = insertarNodo(nodo->right, dato);
  else 
    return nodo;//Retorna el nodo en caso de que el dato estÃ© repetido y no lo inserta en el arbol
  nodo->profundidad = 1 + maxDato(profundidad(nodo->left), profundidad(nodo->right));//Le asigna una profundidad al nodo
  int balance = factorBalance(nodo);//Se mide el factor de balance del arbol

  //Se hacen las rotaciones necesarias hasta corregir el balance
  if (balance > 1 && dato < nodo->left->dato)
    return rightRotacion(nodo);

  if (balance < -1 && dato > nodo->right->dato)
    return leftRotacion(nodo);

  // left-right
  if (balance > 1 && dato > nodo->left->dato) {
    nodo->left = leftRotacion(nodo->left);
    return rightRotacion(nodo);
  }
  //right-left

  if (balance < -1 && dato < nodo->right->dato) {
    nodo->right = rightRotacion(nodo->right);
    return leftRotacion(nodo);
  }


  return nodo;
}

//Función que borra un nodo del arbol dado un dato y un nodo (generalmente la raíz), y lo rebalancea de acuerdo a su nueva estructura
Nodo *borrarNodo(Nodo *nodo, int dato){
  if (nodo == NULL)
    return nodo;//Si el nodo es nulo retorna el nodo y no elimina nada
  /*=Borrar BST=*/ //Busca el nodo en la dirección que corresponde
  if (dato < nodo->dato)
    nodo->left = borrarNodo(nodo->left, dato);//Se corrigio esta linea para que la función avance hacia la izquierda en la busqueda del nodo en vez de quedarse en un bucle infinito 
  else if (dato > nodo->dato)
    nodo->right = borrarNodo(nodo->right, dato);//Se corrigio de manera similar a la anterior condición
    else{
      /*=Nodo con un hijo o sin hijos=*/
      if((nodo->left == NULL) || (nodo->right == NULL)){
        Nodo *temp = nodo->left? nodo->left : nodo->right;
        if (temp == NULL){
          temp = nodo;
          nodo = NULL;
        } else 
          *nodo = *temp;
        delete temp;
      } else {
        Nodo *temp = minimoNodo(nodo->right);
        nodo->dato = temp->dato;
        nodo->right = borrarNodo(nodo->right, temp->dato);
      }
    }
    //Balancea el arbol de acuerdo a su nuevo factor de balance
    if(nodo == NULL)
      return nodo;
    nodo->profundidad = 1 + maxDato(profundidad(nodo->left), profundidad(nodo->right));
    int balance = factorBalance(nodo);
    if(balance > 1 && factorBalance(nodo->left) >= 0)
      return rightRotacion(nodo);
    if(balance > 1 && factorBalance(nodo->left) < 0){
      nodo->left = leftRotacion(nodo->left);
      return rightRotacion(nodo);
    }
    if(balance < -1 && factorBalance(nodo->right) <= 0)
      return leftRotacion(nodo);
    if(balance < -1 && factorBalance(nodo->right) > 0){
      nodo->right = rightRotacion(nodo->right);
      return leftRotacion(nodo);
    }
    return nodo;
}

/*=InOrder traversal=*/
void InOrdenT(Nodo *nodo){
  if(nodo!=NULL){
    InOrdenT(nodo->left);
    cout << nodo->dato << " ";
    InOrdenT(nodo->right);
  }
}

/*=PreOrder traversal=*/
void PreOrdenT(Nodo *nodo){//Implementación de Preorder
  if(nodo!=NULL){
    cout << nodo->dato << " ";
    PreOrdenT(nodo->left);
    PreOrdenT(nodo->right);
  }
}

/*=PostOrder traversal=*/
void PostOrdenT(Nodo *nodo){//Implementación de Postorder
  if(nodo!=NULL){
    PostOrdenT(nodo->left);
    PostOrdenT(nodo->right);
    cout << nodo->dato << " ";
  }
}



int main(){

  //10,14,12,2,4,16,18,20

  Nodo *arbolAVL = NULL;

  arbolAVL = insertarNodo(arbolAVL, 10);
  arbolAVL = insertarNodo(arbolAVL, 14);
  arbolAVL = insertarNodo(arbolAVL, 12);
  arbolAVL = insertarNodo(arbolAVL, 2);
  arbolAVL = insertarNodo(arbolAVL, 4);
  arbolAVL = insertarNodo(arbolAVL, 16);
  arbolAVL = insertarNodo(arbolAVL, 18);
  arbolAVL = insertarNodo(arbolAVL, 20);

  cout << "\n -InOrden ";
  InOrdenT(arbolAVL);
  cout << endl;

  cout << "\n -PreOrden ";
  PreOrdenT(arbolAVL);
  cout << endl;

  cout << "\n -PostOrden ";
  PostOrdenT(arbolAVL);
  cout << endl;

  
  cout << "\n  Se elimina el nodo(16): " << endl;
  arbolAVL = borrarNodo(arbolAVL, 16);
  
  cout << "\n -InOrden ";
  InOrdenT(arbolAVL);
  cout << endl;

  cout << "\n -PreOrden ";
  PreOrdenT(arbolAVL);
  cout << endl;

  cout << "\n -PostOrden ";
  PostOrdenT(arbolAVL);
  cout << endl;
  
  

  return 0;	
}
