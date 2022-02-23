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

struct Core{//EStadísticas
  int attack; //ataque
  int defense;  //defensa
  int hp; //Puntos de vida
};

enum Breed{//Enumeración de enemigos
  AXOLOTL,
  TROLL,
  ORC,
  HELLHOUND,
  DRAGON
};

enum Error{//Enumeramos los errores
  ERR_NAME,
  ERR_DISTRIBUTION,
  ERR_OPTION,
  ERR_SPECIAL,
  ERR_RUNAWAY,
};

struct Enemy{ //Estrucura de enemigo
  Breed name;  //Nombre del enemigo
  Core features;  //Estadísticas del enemigo
};

struct Hero{  //Estructura de héroe
  char name[KNAME]; //Nombre del héroe
  Core features;  //Estadísticas del héroe
  bool special; //Habilidad especial del héroe
  int runaways; //Número de veces que puede escapar
  int exp;  //Puntos de experiencia
  int kills[KENEMIES];  //Numero de kills por enemigo
};
//DECLARACIÓN DE FUNCIONES
void menu(Hero&,Enemy&,bool&,bool&);
void report(const Hero& );
void error(Error);
int rollDice();
void Heroname(Hero&);
void Heropoints(Hero&);
Hero createHero();
void imprimirEnemigo();
void elegirenemigo(int,Enemy&);
Enemy createEnemy();
void Hero_turn(Hero&,Enemy&,bool&);
void Enemy_turn(Hero&,Enemy&);
void add_exp(Hero&,Enemy);
void fight(Hero&,Enemy&,bool&,bool&,bool&);
void run_away(Hero&,Enemy&,bool&);
void specialFight(Hero&,Enemy&,bool&,bool&);
void showMenu();

/*Función imprime por pantalla un error
  *PARÁMETROS: enum con el tipo de error
  *RETORNO: imprime por pantalla el error
*/
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
      cout << "ERROR: special not available" << endl;
      break;
    case ERR_RUNAWAY:
      cout<<"ERROR: cannot run away"<<endl;
  }
}
//Función que genera aleatoriamente un numero
int rollDice(){
  return rand()%KDICE+1;
}
/*Función que obtiene el nombre del héroe y comprueba si es correcto
  *PARÁMETROS: Un héroe en el que modificaremos su nombre (paso por referencia)
  *RETORNO: Nombre del héroe correcto
*/
void Heroname(Hero& hero){
int i=0;
do{
  cout<<"Enter hero name: ";
  fgets(hero.name,KNAME,stdin); //Cuando leemos el nombre su tamaño será de 32 rellenando las posiciones resultantes
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
/*Función que obtiene los puntos del héroe que añadiremos en un string
  *PARÁMETROS: UN Héroe en el que modificaremos un features (paso por referencia)
  *RETORNO: Distribución correcta de los puntos de combate así como la vida del héroe
*/
void Heropoints(Hero& hero){
unsigned int total,pos=0;
float attack_distr,defense_distr;
do{
  string aux="",distribution="";
  unsigned int i=0; //Inicializamos todos los valores del héroe por defecto
  attack_distr=0;
  defense_distr=0;
  hero.features.attack=0;
  hero.features.defense=0;
  hero.features.hp=0;
  hero.special=true;
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
      if(pos>=0 && pos<KNAME){
        for( i=0;i<pos;i++){
        aux+=distribution[i];
      }
        attack_distr=(atoi(aux.c_str()));
        hero.features.attack=KPOINTS*(attack_distr/100);
        aux="";//LImpiamos el auxiliar ya que lo vamos a volver a usar
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
}while( attack_distr<0 || defense_distr<0 || total!=100 );
}
/*Función que crea un héroe con todos los valores que ha añadido el usuario correctamente
  *PARÁMETROS: Ninguno
  *RETORNO: Héroe con datos correctos
*/
Hero createHero(){
Hero hero;
Heroname(hero);
Heropoints(hero);
return hero;
}
/*Función que imprime los datos de un enemigo
  *PARÁMETROS: Enemigo actual que queremos imprimir por pantalla
  *RETORNO: Datos del enemigo
*/
void imprimirEnemigo(Enemy& enemy){
  string enemy_name="";
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
/*Función que elige un enemigo según el número de un dado
  *PARÁMETROS: un número de dado y el propio enemigo que será modificado por el anterior (paso por referecia)
  *RETORNO: Un nuevo enemigo
*/
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
/*Función que crea un enemigo con todos los valores obtenidos por un dado aleatoriamente
  *PARÁMETROS: Ninguno
  *RETORNO: Enemigo
*/
Enemy createEnemy(){
Enemy enemy;
int dado=rollDice();
elegirenemigo(dado,enemy);
return enemy;
}
/*Función que simula el primer turno donde el héroe ataca primero
  *PARÁMETROS: Héroe actual,Enemigo actual, booleano que indica si hemos activado el ataque especial
  *RETORNO: Ataque de nuestro héroe para matar al enemigo
*/
void Hero_turn(Hero &hero, Enemy& enemy,bool& special){
  int hero_turn=0,enemy_turn=0;
  if(special==true){
     hero_turn=rollDice()*5*3;
     enemy_turn=rollDice()*5;
  }else{
     hero_turn=rollDice()*5;
     enemy_turn=rollDice()*5;
  }
  int attack=hero.features.attack+hero_turn;
  int defense=enemy.features.defense+enemy_turn;
  int hit_points=attack-defense;
  if(hit_points<=0){
    hit_points=0;
  }
  int enemy_health= enemy.features.hp-hit_points;
  if(enemy_health<0){
      enemy_health=0;
    }
    enemy.features.hp=enemy_health;
cout<<"[Hero -> Enemy]" <<endl
    <<"Attack: "<<hero.features.attack<<" + "<<hero_turn<<endl
    <<"Defense: "<<enemy.features.defense<<" + "<<enemy_turn<<endl
    <<"Hit points: "<<hit_points<<endl
    <<"Enemy health points: "<<enemy_health<<endl;
    
}
/*Función que simula el segundo turno donde el enemigo ataca al héroe
  *PARÁMETROS: Héroe actual,Enemigo actual
  *RETORNO: Ataque de el enemigo para matar al héroe
*/
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
    if(hero_health<0){
      hero_health=0;    }
      hero.features.hp=hero_health;
cout<<"[Enemy -> Hero]" <<endl
    <<"Attack: "<<enemy.features.attack<<" + "<<enemy_turn<<endl
    <<"Defense: "<<hero.features.defense<<" + "<<hero_turn<<endl
    <<"Hit points: "<<hit_points<<endl
    <<"Hero health points: "<<hero_health<<endl;
}
/*Función que añade experiencia al héroe dependiendo del enemigo que ha matado
  *PARÁMETROS: El héroe que modificaremos su experiencia(paso por referencia) y el enemigo que necesitaremos su nombre(paso por valor) para saber cuantos puntos añadiremos
  *RETORNO: Puntos de experiencia correspondientes para el héroe
*/
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
/*Función que simula la batalla entre el héroe y enemigo
  *PARÁMETROS: El héroe , el enemigo actual, un booleano para indicar si podemos huir y otro si podemos usar el ataque especial
  *RETORNO: Imprime por pantalla la batalla
*/
void fight(Hero &hero,Enemy &enemy,bool& runaway,bool& special){
    Hero_turn(hero,enemy,special);
    if(enemy.features.hp==0){
      cout<<"Enemy killed\n";
      add_exp(hero,enemy);
      enemy=createEnemy();
    }
    else{
      Enemy_turn(hero,enemy);
      if(hero.features.hp==0){
        cout<<"You are dead";
        report(hero);
        exit(0);
      }
    }
    runaway=true;
    special=false;
}
/*Función que nos permite huir del enemigo y cambiarlo
  *PARÁMETRO: El héroe, nuestro enemigo que queremos huir,y booleano que nos indica si podemos huir
  *RETORNO: Imprime por pantalla otro nuevo enemigo modificandolo
*/
void run_away(Hero &hero, Enemy &enemy,bool& runaway){
  if(hero.runaways>0 && runaway==true){
    hero.runaways--;
    cout<<"You run away\n";
    enemy=createEnemy();
    runaway=false;
  }
  else{
    error(ERR_RUNAWAY);
  }
}
/*Función que nos permite usar el ataque especial de nuestro héroe
  *PARÁMETRO: El héroe, nuestro enemigo que queremos huir,y booleano que nos indica si hemos huido anteriormente y otro si ya hemos utilizado el ataque especial
  *RETORNO: Imprime por pantalla la batalla contra un enemigo pero usando el ataque especial
*/
void specialFight(Hero& hero,Enemy& enemy,bool& runaway,bool& special){
  if(hero.special==true){
    special=true;
    fight(hero,enemy,runaway,special);
    hero.special=false;
  }
  else{
    error(ERR_SPECIAL);
  }
}
/*Función que imprime por pantalla nuestros ddatos y estadísticas
  *PARÁMETROS: El héroe
  *RETORNO: Datos y estadísticas del jugador
*/
void report(const Hero &hero){
  string special="";
  if(hero.special==true){
    special="yes";
  }
  else{
    special="no";
  }
  int total_enemies=0;
  for(int i=0;i<=4;i++){
    total_enemies+=hero.kills[i];
  }
  cout<<"[Report]"<<endl
      <<"Name: "<<hero.name
      <<"Attack: "<<hero.features.attack<<endl
      <<"Defense: "<<hero.features.defense<<endl
      <<"Health points: "<<hero.features.hp<<endl
      <<"Special: "<<special<<endl
      <<"Runaways: "<<hero.runaways<<endl
      <<"Exp: "<<hero.exp<<endl
      <<"Enemies killed:"<<endl
      <<"- Axolotl: "<<hero.kills[0]<<endl
      <<"- Troll: "<<hero.kills[1]<<endl
      <<"- Orc: "<<hero.kills[2]<<endl
      <<"- Hellhound: "<<hero.kills[3]<<endl
      <<"- Dragon: "<<hero.kills[4]<<endl
      <<"- Total: "<<total_enemies<<endl;
}
/*Función qu muestra el menú
  *PARÁMETROS: Ninguno
  *RETORNO: Imprime por pantalla el menú
*/
void showMenu(){
  cout << "[Options]" << endl
       << "1- Fight" << endl
       << "2- Run away" << endl
       << "3- Special" << endl
       << "4- Report" << endl
       << "q- Quit" << endl
       << "Option: ";
}
/*Función que comprueba que el menú está siendo utilizado correctamente
  *PARÁMETROS: El héroe, el enemigo, un booleano para conocer si podemos huir y otro si podemos usar el ataque especial
  *RETORNO: Uso correcto de nuestro menú
*/
void menu(Hero& hero, Enemy& enemy,bool& runaway, bool& special){
char opc='0';
    do{
      showMenu();
      cin>>opc;
      switch (opc)
      {
      case '1':
        fight(hero,enemy,runaway,special);
        break;
      case '2':
        run_away(hero,enemy,runaway);
        break;
      case '3':
        specialFight(hero,enemy,runaway,special);
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
    bool  runaway=true,special=false;
    srand(atoi(argv[1])); // Introducimos la semilla para generar números aleatorios
    hero=createHero();
    enemy=createEnemy();
    menu(hero,enemy,runaway,special);
  }
}
