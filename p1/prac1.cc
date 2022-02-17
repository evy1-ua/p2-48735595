// Programación 2 - Práctica 1
// DNI:48735595K
// Nombre:Eric Vivancos Yagues

#include <iostream>
#include <cstdlib> // Para rand(), srand() y atoi()
#include <cctype> // Comprobar si es numero o digito
#include <string.h>

using namespace std;

//DECLARACIÓN DE CONSTANTES
const int KNAME=32; //Máximo numero de caracteres del nombre del héroe
const int KENEMIES=5; //Número de razas de enemigos diferentes
const int KPOINTS=200; //Número de puntos de habilidad a repartir
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
//Enumeramos los errores
enum Error{
  ERR_NAME,
  ERR_DISTRIBUTION,
  ERR_OPTION,
  ERR_SPECIAL,
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
void menu(Hero&,Enemy&);
void report(const Hero& );
//Función que recibe un enum y imprimirá por pantalla el error
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
//Modulo de introducir el nombre del héroe, recibirá un parámetro de heroe
void Heroname(Hero& hero){
int i=0;
do{
  cout<<"Enter hero name: ";
  fgets(hero.name,KNAME,stdin);
  while(hero.name[i]){
    if(isalpha(hero.name[0])==0 || ispunct(hero.name[i])){
      error(ERR_NAME);
      break; 
    }
    else{
      i++;
    }
  }
}while(isalpha(hero.name[0])==0 || ispunct(hero.name[i])!=0) ;
}
void Heropoints(Hero& hero){
unsigned int total,pos=0;
float attack_distr,defense_distr;
do{
  string aux="",distribution="";
  unsigned int i=0;
  attack_distr=0;
  defense_distr=0;
  hero.features.attack=0;
  hero.features.defense=0;
  hero.features.hp=0;
  hero.special=false;
  hero.exp=0;
  hero.runaways=3;
  for (int i = 0; i < KENEMIES; i++)
  {
    hero.kills[i]=0;
  }
  
  hero.kills[KENEMIES]={0};

    cout<<"Enter attack/defense: ";
    getline(cin,distribution);
      pos=distribution.find("/");
      if(pos>=0){
        for( i=0;i<pos;i++){
        aux+=distribution[i];
      }
        attack_distr=(atoi(aux.c_str()));
        hero.features.attack=KPOINTS*(attack_distr/100);
        aux="";
       for( i=pos+1;i<distribution.length();i++){
          aux+=distribution[i];
       }
        defense_distr=(atof(aux.c_str()));
        hero.features.defense=KPOINTS*(defense_distr/100);
        hero.features.hp=2*hero.features.defense;
        total=attack_distr+defense_distr;
        hero.special=true;
        if(attack_distr<0 || defense_distr<0 || total!=100){
          error(ERR_DISTRIBUTION);
        }
      }
      else{
        error(ERR_DISTRIBUTION);
      }
    }while(pos<0 || attack_distr<0 || defense_distr<0 || total!=100 );
}
Hero createHero(){
Hero hero;
Heroname(hero);
Heropoints(hero);
return hero;
}
void imprimirEnemigo(Enemy& enemy){
  string enemy_name;
  switch (enemy.name)
  {
  case  0 :
    enemy_name="Axolotl";
    break;
  case 1 :
    enemy_name="Troll";
    break;
  case 2 :
    enemy_name="Orc";
    break;
  case 3 :
    enemy_name="Hellhound";
    break;
  default:
    enemy_name="Dragon";
    break;
  }
  cout<<"[Enemy]\n";
  cout<<"Breed: "<<enemy_name<<endl;
  cout<<"Attack: "<<enemy.features.attack<<endl;
  cout<<"Defense: "<<enemy.features.defense<<endl;
  cout<<"Health points: "<<enemy.features.hp<<endl;
}

void elegirenemigo(int dado,Enemy& enemy){
  switch (dado)
  {
  case 1: case 2: case 3: case 4: case 5: case 6:
    enemy.name=AXOLOTL;
    enemy.features.attack=40;
    enemy.features.defense=40;
    enemy.features.hp=2*enemy.features.defense;
    imprimirEnemigo(enemy);
    break;
  case 7: case 8: case 9: case 10: case 11:
    enemy.name=TROLL;
    enemy.features.attack=60;
    enemy.features.defense=80;
    enemy.features.hp=2*enemy.features.defense;
    imprimirEnemigo(enemy);
    break;
  case 12: case 13: case 14: case 15:
    enemy.name=ORC;
    enemy.features.attack=80;
    enemy.features.defense=120;
    enemy.features.hp=2*enemy.features.defense;
    imprimirEnemigo(enemy);
    break;
  case 16: case 17: case 18:
    enemy.name=HELLHOUND;
    enemy.features.attack=120;
    enemy.features.defense=100;
    enemy.features.hp=2*enemy.features.defense;
    imprimirEnemigo(enemy);
    break;
  default:
    enemy.name=DRAGON;
    enemy.features.attack=160;
    enemy.features.defense=140;
    enemy.features.hp=2*enemy.features.defense;
    imprimirEnemigo(enemy);
    break;
  }
  
}
Enemy createEnemy(){
Enemy enemy;
int dado=rollDice();
elegirenemigo(dado,enemy);
return enemy;
}
void Hero_turn(Hero &hero, Enemy& enemy){
  int hero_turn=rollDice()*5;
  int enemy_turn=rollDice()*5;
  int attack=hero.features.attack+hero_turn;
  int defense=enemy.features.defense+enemy_turn;
  int hit_points=attack-defense;
  if(hit_points<=0){
    hit_points=0;
  }
  int enemy_health= enemy.features.hp-hit_points;
cout<<"[Hero -> Enemy]" <<endl
    <<"Attack: "<<hero.features.attack<<" + "<<hero_turn<<endl
    <<"Defense: "<<enemy.features.defense<<" + "<<enemy_turn<<endl
    <<"Hit points: "<<hit_points<<endl
    <<"Enemy health points: "<<enemy_health<<endl;
    enemy.features.hp=enemy_health;
    
}
void Enemy_turn(Hero &hero, Enemy& enemy){
  int enemy_turn=rollDice()*5;
  int hero_turn=rollDice()*5;
  
  int attack=enemy.features.attack+enemy_turn;
  int defense=hero.features.defense+hero_turn;
  int hit_points=attack-defense;
  if(hit_points<=0){
    hit_points=0;
  }
  int hero_health= hero.features.hp-hit_points;
cout<<"[Enemy -> Hero]" <<endl
    <<"Attack: "<<enemy.features.attack<<" + "<<enemy_turn<<endl
    <<"Defense: "<<hero.features.defense<<" + "<<hero_turn<<endl
    <<"Hit points: "<<hit_points<<endl
    <<"Hero health points: "<<hero_health<<endl;
    hero.features.hp=hero_health;
    
}
void add_exp(Hero& hero, Enemy enemy){
  switch (enemy.name)
  {
  case AXOLOTL:
    hero.exp=hero.exp+100;
    hero.kills[0]+=1;
    break;
  case TROLL:
    hero.exp=hero.exp+150;
    hero.kills[1]+=1;
    break;
  case ORC:
    hero.exp=hero.exp+200;
    hero.kills[2]+=1;
    break;
  case HELLHOUND:
    hero.exp=hero.exp+300;
    hero.kills[3]+=1;
  
  default:
    hero.exp=hero.exp+400;
    hero.kills[4]+=1;
  }
}
//TODO: error cuando nos matan
void fight(Hero &hero,Enemy &enemy){
    Hero_turn(hero,enemy);
    if(enemy.features.hp==0){
      cout<<"Enemy killed\n";
      add_exp(hero,enemy);
      enemy=createEnemy();
      menu(hero,enemy);
    }
    else{
      Enemy_turn(hero,enemy);
      if(hero.features.hp==0){
        cout<<"You are dead";
        report(hero);
      }
    }
  
}

void report(const Hero &hero){
  cout<<"[Report]"<<endl
      <<"Name: "<<hero.name
      <<"Attack: "<<hero.features.attack<<endl
      <<"Defense: "<<hero.features.defense<<endl
      <<"Health points: "<<hero.features.hp<<endl
      <<"Special: yes"<<endl
      <<"Runaways: "<<hero.runaways<<endl
      <<"Exp: "<<hero.exp<<endl
      <<"Enemies killed:"<<endl
      <<"- Axolotl: "<<hero.kills[0]<<endl
      <<"- Troll: "<<hero.kills[1]<<endl
      <<"- Orc: "<<hero.kills[2]<<endl
      <<"- Hellhound: "<<hero.kills[3]<<endl
      <<"- Dragon: "<<hero.kills[4]<<endl
      <<"- Total: 0 "<<endl;
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
void menu(Hero& hero, Enemy& enemy){
char opc;
    do{
      showMenu();
      cin>>opc;
      switch (opc)
      {
      case '1':
        fight(hero,enemy);
        break;
      case '2':
        cout<<"Run away";
        break;
      case '3':
        cout<<"Special";
        break;
      case '4':
        report(hero);
        break;
      case 'q':
        break;
      default:
      error(ERR_OPTION);
        break;
      }
    }while(opc!='q');
}
int main(int argc,char *argv[]){
  if(argc!=2){ // Si los parámetros no son correctos, el programa termina inmediatamente
    cout << "Usage: " << argv[0] << " <seed>" << endl;
  }
  else{
    Hero hero;
    Enemy enemy;
    srand(atoi(argv[1])); // Introducimos la semilla para generar números aleatorios
    hero=createHero();
    enemy=createEnemy();
    menu(hero,enemy);
    
    // Aquí vendrá todo tu código del "main"...
    
  }
}
