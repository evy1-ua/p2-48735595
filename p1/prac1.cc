// Programación 2 - Práctica 1
// DNI:48735595K
// Nombre:Eric Vivancos Yagues

#include <iostream>
#include <cstdlib> // Para rand(), srand() y atoi()
#include <cctype> // Comprobar si es numero o digito


using namespace std;

const int KNAME=32;
const int KENEMIES=5;
const int KPOINTS=200;
const int KDICE=20; // Número de caras del dado

struct Core{
  int attack;
  int defense;
  int hp;
};

enum Breed{
  AXOLOTL,
  TROLL,
  ORC,
  HELLHOUND,
  DRAGON
};

struct Enemy{
  Breed name;
  Core features;
};

struct Hero{
  char name[KNAME];
  Core features;
  bool special;
  int runaways;
  int exp;
  int kills[KENEMIES];
};
enum Error{
  ERR_NAME,
  ERR_DISTRIBUTION,
  ERR_OPTION,
  ERR_SPECIAL,
};

void error(Error e){
  switch(e){
    case ERR_NAME:
      cout << "ERROR: wrong name" << endl;
      break;
    case ERR_DISTRIBUTION:
      cout << "ERROR: wrong distribution" << endl;
      break;
    case ERR_OPTION:
      cout << "ERROR: wrong option" << endl;
      break;
    case ERR_SPECIAL:
      cout << "ERROR: cannot run away" << endl;
      break;
  }
}

int rollDice(){
  return rand()%KDICE+1;
}
void DemandHero(){
Hero hero;
int i=0;
do{
  cout<<"Enter hero name: \n";
  fgets(hero.name,KNAME,stdin);
  if(isalpha(hero.name[0])==0){
    error(ERR_NAME); 
  }
  while(hero.name[i]){
    if(ispunct(hero.name[i])){
      error(ERR_NAME);
      break; 
    }
    else{
      i++;
    } 
  }
}while(isalpha(hero.name[0])==0 || ispunct(hero.name[i])!=0) ;

}

void fight(Hero &hero,Enemy &enemy){
}

void report(const Hero &hero){
}

void showMenu(){
  cout << "[Options]" << endl
       << "1- Fight" << endl
       << "2- Run away" << endl
       << "3- Special" << endl 
       << "4- Report" << endl
       << "q- Quit" << endl
       << "Option: ";
}

int main(int argc,char *argv[]){
  if(argc!=2){ // Si los parámetros no son correctos, el programa termina inmediatamente
    cout << "Usage: " << argv[0] << " <seed>" << endl;
  }
  else{
    srand(atoi(argv[1])); // Introducimos la semilla para generar números aleatorios
    DemandHero();
    // Aquí vendrá todo tu código del "main"...
    
  }
}
