///Dobre Adriana-Lia, Guțu Ștefania-Alexandra, Pirlogea Luciana-Elena  - grupa 141
#include <iostream>
#include <fstream>

using namespace std;

ifstream fin1("test1.in");
ofstream fout1("test1.out");
ifstream fin2("test2.in");
ofstream fout2("test2.out");
ifstream fin3("test3.in");
ofstream fout3("test3.out");
ifstream fin4("test4.in");
ofstream fout4("test4.out");
ifstream fin5("test5.in");
ofstream fout5("test5.out");
ifstream fin6("test6.in");
ofstream fout6("test6.out");


class Node{
public:
  long long int info;
  bool color; // 1 = rosu, 0 = negru
  Node *left, *right, *parent;

  Node(int newInfo){
    info = newInfo;
    left = right = parent = NULL;
    color = 1; // 1 = rosu (initial nodul introdus este rosu)
  }

   //verifica daca nodul este pe partea stanga a parintelui
  bool LeftChild(){
    return this == parent->left;}

  //returneaza pointerul catre unchi
  Node *uncle(){
    // daca nu exista parinti sau bunici => nu exista nici unchi
    if (parent == NULL or parent->parent == NULL)
        return NULL;

    if (parent->LeftChild())
        return parent->parent->right; //unchi in dreapta
    else
        return parent->parent->left;  //unchi in stanga

  }

  //returneaza pointerul catre frate
  Node *sibling(){
    //null daca nu avem parinte
    if(parent == NULL)
      return NULL;

    if(LeftChild())
      return parent->right;

    return parent->left;
  }

  //interschimba nodul dat cu parintele
  void moveDownNode(Node *p){
    if(parent!=NULL){
      if(LeftChild())
        parent->left = p;
      else
        parent->right = p;
    }
    p->parent = parent;
    parent = p;
  }

  //verifica daca macar unul dintre copii este rosu
  bool RedChild() {
    return (left != NULL and left->color) or
           (right != NULL and right->color);
  }
};

class Red_Black_Tree
{public:
  Node *root;
  void rotateLeft(Node*);
  void rotateRight(Node*);
  void swapColors(Node*, Node*);
  void swapValues(Node*,Node*);
  void fixRedRed(Node*);
  void fixDoubleBlack(Node*);
  void insertNode(int);
  void deleteNode(int);
  int minNode();
  int maxNode();
  int predecessor(int);
  int successor(int);
  Node* k_Node(Node*, int&);
  int k_element(int);
  int cardinal();
  bool search(int);
  void PrintInOrder();
  void PrintInOrderFiles(ofstream & out);
  void inorder(Node *);
  void inorderFiles(Node *, ofstream & out);
  Node* searchNode(int);
  Red_Black_Tree(){
    root=NULL;
    contor=0;
  }
  int contor;
};

int Red_Black_Tree::cardinal(){
  return contor;
}
//roteste la stanga nodul dat
void Red_Black_Tree::rotateLeft(Node* n)
{
Node* newParent=n->right; //noul parinte va fi copilul drept al nodului dat ca parametru
if(n==root)
  root=newParent; //daca nodul curent e radacina, actualizam radacina
n->moveDownNode(newParent);
n->right=newParent->left;//facem legatura dintre n si copilul stang al noului parinte

if (newParent->left != NULL)
      newParent->left->parent = n; //facem legatura dintre copilul stang al noului parinte si n, in cazul in care nu este null
newParent->left=n; //facem legatura dintre noul parinte si n
}

//roteste la dreapta nodul dat
void Red_Black_Tree::rotateRight(Node* n)
{
Node* newParent=n->left; //noul parinte va fi copilul stang al nodului dat ca parametru
if(n==root)
  root=newParent; //daca nodul curent e radacina, actualizam radacina
n->moveDownNode(newParent);
n->left=newParent->right;//facem legatura dintre n si copilul drept al noului parinte

if (newParent->right != NULL)
      newParent->right->parent = n; //facem legatura dintre copilul drept al noului parinte si n, in cazul in care nu este null
newParent->right=n; //facem legatura dintre noul parinte si n
}

//interchimbare culori
void Red_Black_Tree::swapColors(Node* n1, Node* n2){
  bool aux;
  aux = n1->color;
  n1->color = n2->color;
  n2->color = aux;
  }

//interschimbare valori
void Red_Black_Tree::swapValues(Node* n1, Node* n2){
  int aux;
  aux = n1->info;
  n1->info = n2->info;
  n2->info = aux;
}
void Red_Black_Tree::fixRedRed(Node *n){
  if(n==root){
    n->color = 0; //0=negru
    return;
  }
  Node *parent = n->parent;
  Node *grandparent = parent->parent;
  Node *uncle = n->uncle();
  if(parent->color!=0){
    if(uncle!=NULL && uncle->color==1){
      // daca unchiul e rosu, recoloram si apelam recursiv functia pentru a rezolva problema red red pentru bunic
      parent->color = 0;
      uncle->color = 0;
      grandparent->color = 1;
      fixRedRed(grandparent);
    }
    else{
      //unchiul e negru sau e null
        if(parent->LeftChild()){ //parintele pe partea stanga
          //cazul in care parintele si fiul formeaza o linie
            if(n->LeftChild()){
              swapColors(parent, grandparent);
            }
          //cazul in care parintele si fiul formeaza un triunghi
            else{
              rotateLeft(parent);
              swapColors(n, grandparent);
            }
            rotateRight(grandparent);
        }
        else{ //parintele pe partea dreapta
         //cazul in care parintele si fiul formeaza un triunghi
          if(n->LeftChild()){
            rotateRight(parent);
            swapColors(n, grandparent);
          }
          //cazul in care parintele si fiul formeaza o linie
          else{
            swapColors(parent, grandparent);
          }
          rotateLeft(grandparent);
        }
    }

  }
}

//cautam valoarea n in arbore
//daca o gaseste, returneaza nodul
//daca nu o gaseste, returneaza ultimul nod din parcurgere
Node* Red_Black_Tree::searchNode(int n)
  {Node* aux = root;
    while (aux != NULL) {
      if (n < aux->info) {
        if (aux->left == NULL)
          break;
        else
          aux = aux->left;
      } else if (n == aux->info) {
        break;
      } else {
        if (aux->right == NULL)
          break;
        else
          aux = aux->right;
      }
    }
    return aux;
  }

//cautam valoarea n in arbore
//daca o gaseste returneaza 1, altfel returneaza 0
bool Red_Black_Tree::search(int n)
  {Node* aux=searchNode(n);
  if(aux->info==n)
    return 1;
  else
    return 0;
  }

//insereaza un nou nod cu valoarea n
void Red_Black_Tree::insertNode(int n)
{contor++; //incrementam nr total de elemente
  Node *newNode=new Node(n);
  //daca arborele este gol, noul nod devine radacina care este de culoare neagra
  if (root==NULL)
  {
    newNode->color=0;
    root=newNode;
  }
  else //daca nu este gol, se verifica daca se poate introduce noul nod
  {
    Node *aux= searchNode(n);
    //daca valoarea exista deja, nu se mai insereaza
    if (aux->info == n)
      return ;
    //valoarea nu exista, deci inseram noua valoare si facem legatura dintre noduri
    newNode->parent=aux;
    if (n<aux->info)
       aux->left=newNode;
    else
       aux->right=newNode;
    fixRedRed(newNode); //se corecteaza problema red red daca exista
  }
}

void Red_Black_Tree::inorder(Node *n){
    if (n == NULL)
      return;
    inorder(n->left);
    cout << n->info << " ";
    inorder(n->right);
}
//metoda pentru afisare in consola
void Red_Black_Tree::PrintInOrder(){
  cout << "Inorder: " << endl;
    if (root == NULL)
      cout << "Tree is empty" << endl;
    else
      inorder(root);
    cout << endl;
}

void Red_Black_Tree::inorderFiles(Node *n, ofstream &out){
    if (n == NULL)
      return;
    inorderFiles(n->left, out);
    out << n->info << " ";
    inorderFiles(n->right, out);
}
//metoda pentru afisare in fisiere
void Red_Black_Tree::PrintInOrderFiles(ofstream &out)
{   out << "Inorder: " << endl;
    if (root == NULL)
      out << "Tree is empty" << endl;
    else
      inorderFiles(root, out);
    out << endl;

}
//returneaza valoarea succesorului nodului dat ca parametru
int Red_Black_Tree::successor(int n)
{ Node* aux=searchNode(n);
if (aux->info==n)
  {if(aux->right!=NULL)
      aux = aux->right;
    while (aux->left != NULL)
      aux = aux->left;
    if(aux->info==n){
      while(aux!=root){
        aux = aux->parent;
        if(aux->info > n)
          return aux->info;
      }
      if(aux==root)
        return n; //daca nodul n nu are succesor este returnata valoarea lui n
    }
  }
return aux->info;
}

//returneaza valoarea predecesorului nodului dat ca parametru
int Red_Black_Tree::predecessor(int n)
{ Node* aux=searchNode(n);
if (aux->info==n)
  {if(aux->left!=NULL)
      aux = aux->left;
    while (aux->right != NULL)
      aux = aux->right;
     if(aux->info==n){
      while(aux!=root){
        aux = aux->parent;
        if(aux->info < n)
          return aux->info;
      }
      if(aux==root)
        return n;//daca nodul n nu are predecesor este returnata valoarea lui n
    }
  }
return aux->info;
}

void Red_Black_Tree::fixDoubleBlack(Node *n) {
    if (n == root)
      // Reached root
      return;

    Node *sibling = n->sibling();
    Node *parent = n->parent;
    if (sibling == NULL) {
      //daca nu avem frate, problema de double black trece la parinte
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == 1) {
        // fratele este rosu
        parent->color = 1;
        sibling->color = 0;
        if (sibling->LeftChild()) {
          // fratele e copilul stang
          rotateRight(parent);
        } else {
          // fratele e copilul drept
          rotateLeft(parent);
        }
        fixDoubleBlack(n);
      } else {
        // fratele e negru
        if (sibling->RedChild()) {
          // avem macar un copil rosu
          if (sibling->left != NULL and sibling->left->color == 1) {
            if (sibling->LeftChild()) {
              // fratele si copilul lui sunt ambii pe partea stanga (copilul rosu)
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rotateRight(parent);
            } else {
              // fratele pe partea dreapta, iar copilul pe stanga(copilul rosu)
              sibling->left->color = parent->color;
              rotateRight(sibling);
              rotateLeft(parent);
            }
          } else {
            if (sibling->LeftChild()) {
              // fratele pe partea stanga, copilul sau pe dreapta sau are culoarea neagra
              sibling->right->color = parent->color;
              rotateLeft(sibling);
              rotateRight(parent);
            } else {
              // fratele pe partea dreapta si copilul lui tot pe dreapta sau are culoare neagra
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              rotateLeft(parent);
            }
          }
          parent->color = 0;
        } else {
          // are 2 copii negri
          sibling->color = 1;
          if (parent->color == 0)
            fixDoubleBlack(parent);
          else
            parent->color = 0;
        }
      }
    }
  }
void Red_Black_Tree::deleteNode(int n)
{
  if (root == NULL)
      // arborele e gol
      return;

    Node *v = searchNode(n), *u;

    if (v->info != n) {
      cout << "Nu am gasit nodul cautat: " << n << endl;
      return;
    }

  contor--;//decrementam numarul total de elemente
 //cautam inlocuitorul nodului pe care vrem sa il stergem
  while(1){
    if (v->left != NULL and v->right != NULL){///nodul are 2 copii
      //cautam nodul care nu are copil stang
       u = v->right;

      while (u->left != NULL)
        u = u->left;
    }
    else{
      // nodul este frunza
      if (v->left == NULL and v->right == NULL)
        u= NULL;
      else
        // nodul este unicul copil
        if (v->left != NULL)
          u= v->left;
        else
          u= v->right;
    }

    Node *parent = v->parent;

    if (u == NULL) {
      // v e frunza
      if (v == root) {
        // v e radacina, rezulta ca stergem radacina
        root = NULL;
      } else {
        if ((u == NULL or u->color == 0) and (v->color == 0)) {
          // u si v sunt negri
          // v este frunza, deci rezolvam cazul double black
          fixDoubleBlack(v);
        } else {
          // ori u, ori v sunt rosii
          if (v->sibling() != NULL)
            // daca v are frate, facem fratele rosu
            v->sibling()->color = 1;
        }

        // stergem v din arbore
        if (v->LeftChild()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }

    if (v->left == NULL or v->right == NULL) {
      // v are un singur copil
      if (v == root) {
        // v este radacina, atribuim valoarea lui u catre v si stergem u
        v->info = u->info;
        v->left = v->right = NULL;
        delete u;
      } else {
        // stergem v din arbore si il mutam pe u in locui lui
        if (v->LeftChild()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if ((u == NULL or u->color == 0) and (v->color == 0)) {
          // u si v sunt de culoare neagra, deci rezolvam cazul double black
          fixDoubleBlack(u);
        } else {
          //ori u, ori v rosii, il facem pe u negru
          u->color = 0;
        }
      }
      return;
    }

    // daca v are doi copii, interschimbam valorile cu succesorul si refacem pasii de stergere pentru u
    swapValues(u, v);
    v = u;
  }
}

//returneaza elementul minim
int Red_Black_Tree::minNode()
{   Node* aux=root;
    while(aux->left)
        aux=aux->left;
    return aux->info;
}

//returneaza elementul maxim
int Red_Black_Tree::maxNode()
{   Node* aux=root;
    while(aux->right)
        aux=aux->right;
    return aux->info;
}

//returneaza al k-lea nod
Node* Red_Black_Tree::k_Node(Node* root, int& k)
{
    // cazul in care arborele e gol
    if (root == NULL)
        return NULL;

    // cautam in subarborele stang
    Node* left = k_Node(root->left, k);

    // daca al k-lea element este gasit in subarborele stang, il returnam
    if (left != NULL)
        return left;

    // daca elementul curent este al k-lea, il returnam
    k--;
    if (k == 0)
        return root;

    // altfel cautam in subarborele drept
    return k_Node(root->right, k);
}

//returneaza valoarea celui de-al k-lea element
int Red_Black_Tree::k_element(int k)
{
  return k_Node(root, k)->info;
}

int main() {
  Red_Black_Tree tree;

  ///testul 1 care se aplica pe arborele tree
  long long int x;
  while(fin1>>x)
    tree.insertNode(x);
  fout1<<"Cardinalul este "<<tree.cardinal()<<endl;
  tree.PrintInOrderFiles(fout1);

  ///testul 2 care se aplica pe arborele tree
  long long int y;
  while(fin2>>y)
    tree.deleteNode(y);
  fout2<<"Cardinalul este "<<tree.cardinal()<<endl;
  tree.PrintInOrderFiles(fout2);

  ///testul 3 care se aplica pe arborele tree2
  Red_Black_Tree tree2;
  long long int z;
  while(fin3>>z)
    tree2.insertNode(z);
  fout3<<"Cardinalul este "<<tree2.cardinal()<<endl;
  tree2.PrintInOrderFiles(fout3);
  fout3<<"Minimul este "<<tree2.minNode()<<endl;
  fout3<<"Maximul este "<<tree2.maxNode();

  ///testul 4 care se aplica pe arborele tree2
  //in cazul in care nu avem predecesor sau succesor pentru a, se afiseaza valoarea a
  long long int a;
  while(fin4>>a){
    fout4<<"Predecesorul nodului "<<a<<" este "<<tree2.predecessor(a)<<", iar succesorul este "<<tree2.successor(a)<<endl;
  }

  ///testul 5 care se aplica pe arborele tree2
  long long int b;
  while(fin5>>b){
    fout5<<"Al "<<b<<"-lea element este "<<tree2.k_element(b)<<endl;
  }

  ///testul 6 care se aplica pe arborele tree2
  long long int c;
  while(fin6>>c){
    if(tree2.search(c))
      fout6<<"Elementul "<<c<<" a fost gasit in multime"<<endl;
    else
      fout6<<"Elementul "<<c<<" nu a fost gasit in multime"<<endl;
  }

  return 0;
}
