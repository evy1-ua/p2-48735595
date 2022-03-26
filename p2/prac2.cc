// Programación 2 - Práctica 2
// DNI: 48735595K
// Nombre: Eric Vivancos Yagües

#include <iostream>
#include <vector>
#include <cctype>
#include <string.h>
#include <fstream>

using namespace std;

//DECLARACIÓN DE CONSTANTES
const int KMAXSTRING = 50;  //Máximo de carácteres en los archivos binarios

//Enumeración que nos indica el tipo de errores
enum Error {
  ERR_OPTION,
  ERR_BOOK_TITLE,
  ERR_BOOK_AUTHORS,
  ERR_BOOK_DATE,
  ERR_BOOK_PRICE,
  ERR_ID,
  ERR_FILE,
  ERR_ARGS
};

//Enumeración que nos indica el tipo de peticiones del usuario
enum Request {
  BOOK_TITLE_REQUEST,
  BOOK_AUTHORS_REQUEST,
  BOOK_YEAR_REQUEST,
  BOOK_PRICE_REQUEST,
  BOOK_ID_REQUEST,
  FILENAME_REQUEST,
  ERASE_REQUEST
};
//EStructura básica de un libro
struct Book {
  unsigned int id;  //id del libro
  string title; //titulo del libro
  string authors; //autor/es del libro
  int year; //año de publicación del libro
  string slug;  //slug del libro(implemetación web)
  float price;  //precio del libro
};
//Estructura Binaria de un libro
struct BinBook {
  unsigned int id;  //id binario del libro
  char title[KMAXSTRING]; //titulo en biario del libro
  char authors[KMAXSTRING]; //autor/es en binario del libro
  int year; //año en binario
  char slug[KMAXSTRING];  //slug en binario
  float price;  //precio en binario
};
//Estructura básica de una tienda de libros
struct BookStore {
  string name;  //Nombre de la tienda
  vector<Book> books; //vector de libros que contiene la tienda
  unsigned int nextId;  //id del siguiente libro de la tienda
};
//Estructura binaria de una tienda de libros
struct BinBookStore {
  char name[KMAXSTRING];  //nombre en binario de la tienda
  unsigned int nextId;  //id del siguiente libro en la tienda
};

/*Función que imprime y pide una petición al usuario
  *PARÁMETROS: enum con el tipo de petición
  *RETORNO: la respuesta de la petición
*/
string userRequest(Request r) {
  string request;
  switch(r) {
    case BOOK_TITLE_REQUEST:
      cout<<"Enter book title: ";
      getline(cin,request);
      break;
    case BOOK_AUTHORS_REQUEST:
      cout<<"Enter author(s): ";
      getline(cin,request);
      break;
    case BOOK_YEAR_REQUEST:
      cout<<"Enter publication year: ";
      getline(cin,request);
      break;
    case BOOK_PRICE_REQUEST:
      cout<<"Enter price: ";
      getline(cin,request);
      break;
    case BOOK_ID_REQUEST:
      cout<<"Enter book id: ";
      getline(cin,request);
      break;
    case FILENAME_REQUEST:
      cout<<"Enter filename: ";
      getline(cin,request);
      break;
    case ERASE_REQUEST:
      cout<<"All data will be erased, do you want to continue (Y/N)?: ";
      getline(cin,request);
      break;
  }
  return request;
}

/*Función imprime por pantalla un error
  *PARÁMETROS: enum con el tipo de error
  *RETORNO: imprime por pantalla el error
*/
void error(Error e) {
  switch (e) {
    case ERR_OPTION:
      cout << "ERROR: wrong menu option" << endl;
      break;
    case ERR_BOOK_TITLE:
      cout << "ERROR: wrong book title" << endl;
      break;
    case ERR_BOOK_AUTHORS:
      cout << "ERROR: wrong author(s)" << endl;
      break;
    case ERR_BOOK_DATE:
      cout << "ERROR: wrong date" << endl;
      break;
    case ERR_BOOK_PRICE:
      cout << "ERROR: wrong price" << endl;
      break;
    case ERR_ID:
      cout << "ERROR: wrong book id" << endl;
      break;
    case ERR_FILE:
      cout << "ERROR: cannot open file" << endl;
      break;
    case ERR_ARGS:
      cout << "ERROR: wrong arguments" << endl;
      break;
  }
}

/*Función qu muestra el menú principal
  *PARÁMETROS: Ninguno
  *RETORNO: Imprime por pantalla el menú
*/
void showMainMenu() {
  cout << "[Options]" << endl
       << "1- Show catalog" << endl
       << "2- Show extended catalog" << endl
       << "3- Add book" << endl
       << "4- Delete book" << endl
       << "5- Import/export" << endl
       << "q- Quit" << endl
       << "Option: ";
}
/*Función qu muestra el menú de importar/exportar
  *PARÁMETROS: Ninguno
  *RETORNO: Imprime por pantalla el menú
*/
void showImportMenu() {
  cout << "[Import/export options]" << endl
       << "1- Import from CSV" << endl
       << "2- Export to CSV" << endl
       << "3- Load data" << endl
       << "4- Save data" << endl
       << "b- Back to main menu" << endl
       << "Option: ";
}
/*Función que muestra el catálogo con datos reducidos
  *PARÁMETROS: la estructura de la tienda para acceder a sus libros
  *RETORNO: Impresión por pantalla de los libros
*/
void showCatalog(const BookStore &bookStore) {
    for(unsigned int i=0;i<bookStore.books.size();i++){
      cout<<bookStore.books[i].id<<". "<<bookStore.books[i].title
      <<" ("<<bookStore.books[i].year<<"), "<<bookStore.books[i].price<<endl;
    }
}
/*Función que muestra el catálogo con datos extendidos
  *PARÁMETROS: la estructura de la tienda para acceder a sus libros
  *RETORNO: Impresión por pantalla de los libros
*/
void showExtendedCatalog(const BookStore &bookStore) {
  for(unsigned int i=0;i<bookStore.books.size();i++){
    cout<<"\""<<bookStore.books[i].title<<"\",\""
        <<bookStore.books[i].authors<<"\","
        <<bookStore.books[i].year<<",\""
        <<bookStore.books[i].slug<<"\","
        <<bookStore.books[i].price<<endl;
  }
}
/*Función que comprueba que son correctos tanto los titulos cómo los autores
  *PARÁMETROS: el string a comprobar y un booleano de si es correcto
  *RETORNO: el booleano de si es correcto;
  *ANOTACIÓN: Hemos separado esta función y correct Title y correct authors ya que al importar un archivo también debemos comprobarlo
*/
bool checkCorrect(string title,bool correct){
  if(title.length()==0){
        error(ERR_BOOK_TITLE);
        correct=false;
        return correct;
    }else{
      for(unsigned int i=0;i<title.length();i++){
       char letra=title[i];
       if(isalnum(letra)==0 && isspace(letra)==0 && letra!=':' && letra!=',' && letra!='-'){
         error(ERR_BOOK_TITLE);
         correct=false;
         return correct;
       }
    }
    }return correct;

}
/*Función intermediaria que nos devuelve el titulo correcto que hemos comprobado en checkCorrect
  *PARÁMETROS: Ninguno;
  *RETORNO: titulo correcto
  *ANOTACIÓN: como debemos comprobar tambien al importar en CSV (otra opción) he creado esta función para que en esta sea pedida por el usuario y al importar sea automaticamente lo leido por el archivo
*/
string correctTitle(){
  string title;
  bool correct=true;
  do{
    correct=true;
    title="";
    title=userRequest(BOOK_TITLE_REQUEST);
    correct=checkCorrect(title,correct);
  }while(correct==false);
   return title;


}
/*Función intermediaria que nos devuelve los autores correctos que hemos comprobado en checkCorrect
  *PARÁMETROS: Ninguno;
  *RETORNO: titulo correcto
  *ANOTACIÓN: como debemos comprobar tambien al importar en CSV (otra opción) he creado esta función para que en esta sea pedida por el usuario y al importar sea automaticamente lo leido por el archivo
*/
string checkAuthors(){
  string authors;
  bool correct=true;
  do{
    correct=true;
    authors="";
    authors=userRequest(BOOK_AUTHORS_REQUEST);
    correct=checkCorrect(authors,correct);
  }while(correct==false);
  return authors;
}
/*Función que comprueba si el año de lanzamiento del libro es correcto
  *PARÁMETROS:Ninguno
  *RETORNO: año de lanzamiento correcto
*/
string checkYear(){
  string year;
  bool correct=true;
  do{
    correct=true;
    year="";
    year=userRequest(BOOK_YEAR_REQUEST);
    if(year.size()==0){
      error(ERR_BOOK_DATE);
      correct=false;
    }
    else if(stoi(year)<1440 || stoi(year)>2022 || year==""){
      error(ERR_BOOK_DATE);
      correct=false;
    }
  }while(correct==false);
  return year;
}
/*Función que crea un slug a partir del titulo
  *PARÁMETROS: el titulo del autor
  *RETORNO:el slug correcto
*/
string createSlug(string title){
  string slug;
  bool Hyphen=true;
  for(unsigned int i=0;i<title.length();i++){
    if(isalnum(title[i])!=0){
      slug.push_back(tolower(title[i]));
      Hyphen=false;
    }
    else if(isspace(title[i]) && !Hyphen){
      slug.push_back('-');
      Hyphen=true;
    }
  }
  if(Hyphen && title.size()>0){ //Evitamos los guiones finales
    slug.pop_back();
  }
  return slug;
}
/*Función que comprueba que  el precio es correcto
  *PARÁMETROS: Ninguno
  *RETORNO: el precio correcto
  *ANOTACIÓN: Como todas las peticiones de usuario nos devuelven strings, para poder comprobar tanto el precio y el año tendremos que usar stof y stoi
*/
string checkprice(){
  string price;
  bool correcto=true;
  do{
    correcto=true;
    price="";
    price=userRequest(BOOK_PRICE_REQUEST);
    if(price.length()==0 || stof(price)<=0){
        error(ERR_BOOK_PRICE);
        correcto=false;
    }else{
      for(unsigned int i=0;i<price.length();i++){
        char letra=price[i];
        if(isdigit(letra)==0 && letra!='.'){
          error(ERR_BOOK_PRICE);
          correcto=false;
        }
      }
    }
  }while(correcto==false);
  return price;
}
/*Función intermediaria que llama a todas las funciones para pedir los datos al usuario sobre un libro y coleccionarlo en una estructura de tipo Book
  *PARÁMETROS: La tienda de libros que contiene los libros y será mas tarde modificada(paso por referencia)
  *RETORNO: Estructura de tipo Book con los datos que el usuario ha introducido correctamente
*/
Book demandBook(BookStore &bookStore){
  Book book;
  string price;
  string title;
  book.id=bookStore.nextId;
  book.title=correctTitle();
  title=book.title;
  book.slug=createSlug(title);
  book.authors=checkAuthors();
  book.year=stoi(checkYear());
  book.price=stof(checkprice());
  bookStore.nextId++;
  return book;

}
/*Función que añade al final del vector books de la tienda el libro introducido por el usuario(añade dinámicamente)
  *PARÁMETROS: La tienda de libros en el que modificaremos su vector de libros(paso por referencia)
  *RETORNO: Añade al final el libro introducido
*/
void addBook(BookStore &bookStore) {
bookStore.books.push_back(demandBook(bookStore));
}
/*Función que borra un libro que el usuario quiera introduciendo su correpondiente id
  *PARÁMETROS: Tienda de libros que modificaremos su vector de libros
  *RETORNO: Borra el libro qu el usuario ha pedido
*/
void deleteBook(BookStore &bookStore) {
  string id;
  id=userRequest(BOOK_ID_REQUEST);
  bool find=false;
  for(unsigned int i=0;i<bookStore.books.size();i++){
    if(bookStore.books[i].id==(unsigned)stoi(id) && id.size()>0){
      bookStore.books.erase(bookStore.books.begin()+(stoi(id)-1));
      find=true;
    }
  }
  if(find==false){//Si el id del libro no existe...
    error(ERR_ID);
  }
}
/*Función que lee y traspasa los datos de un fichero a nuestra tienda
  *PARÁMETROS: El nombre del fichero, un booleano que comprueba si tanto el titulo como los autores son correctos
  *RETORNO: Añade al vector lo libros leidos por un archivo de texto
  *ANOTACIÓN: Utilizamos un string "vacío" que nos quitará todos aquellos carácteres que no nos interesa para nuestro libro
*/
void load_CSV(string namefile,bool correct,BookStore &bookStore){
fstream fichero(namefile,ios::in);
Book book;
  if(fichero.is_open()){
    string vacio,titulo,autor,year,slug,price;
    while(fichero.good()){
        correct=true;
        getline(fichero,vacio,'\"');
        getline(fichero,titulo,'\"');
        book.title=titulo;
        getline(fichero,vacio,'\"');
        getline(fichero,autor,'\"');
        book.authors=autor;
        getline(fichero,vacio,',');
        getline(fichero,year,',');
        book.year=stoi(year);
        getline(fichero,slug,',');
        slug=slug.substr(1,slug.size()-2);
        book.slug=slug;
        getline(fichero,price,'\n');
        book.price=stof(price);
        book.id=bookStore.nextId;
        if(!fichero.eof() && checkCorrect(titulo,correct)==true && checkCorrect(autor,correct)==true){  //Mientras no ea final de fichero y sean correctos tanto titulo y autores...
          bookStore.books.push_back(book);
          bookStore.nextId++; //Al añadir un libro tendremos que aumentar el id del siguiente
        }
    }
     fichero.close();
    }
  else{
    error(ERR_FILE);
  }
}
/*Función que importa libros desde un archivo de texto pero desde el menu principal
  PARÁMETROS: La tienda  que tendrá sus libros que añadiremos por fichero
*/
void importFromCsv(BookStore &bookStore){
  string namefile;
  bool correct=true;
  namefile=userRequest(FILENAME_REQUEST);
  load_CSV(namefile,correct,bookStore);
}
/*Función que exporta lo libros desde el vector de la tienda a un fichero
  *PARÁMETRO: La tienda que necesitaremos sus datos para exportarlos
*/
void exportToCsv(const BookStore &bookStore){
  string namefile;
  namefile=userRequest(FILENAME_REQUEST);
  fstream fichero(namefile,ios::out);
  if(fichero.is_open()){

    for(unsigned int i=0;i<bookStore.books.size();i++){
      fichero<<"\""<<bookStore.books[i].title<<"\",\""
      <<bookStore.books[i].authors<<"\","
      <<bookStore.books[i].year<<",\""
      <<bookStore.books[i].slug<<"\","
      <<bookStore.books[i].price<<endl;
    }
    fichero.close();
  }
  else{
    error(ERR_FILE);
  }
}
/*Función que convierte de binario a libro
  PARÁMETROS: Estructura de libro binaria
  RETORNO: EStructura de libro
*/
Book binarytoBook(const BinBook &bookbin){
  Book book;
  book.id=bookbin.id;
  book.title=bookbin.title;
  book.authors=bookbin.authors;
  book.year=bookbin.year;
  book.slug=bookbin.slug;
  book.price=bookbin.price;
  return book;
}
/*Función que lee un archivo binario
  PARÁMETROS: El nombre del archivo, y la libreria
  RETORNO: Añade los libros al vector
*/
void loadBin(string namefile,BookStore &bookStore){
  ifstream fichero;
  BinBookStore binbookstore;
  BinBook bookbin;
  fichero.open(namefile,ios::in | ios::binary);
    if(fichero.is_open()){
      fichero.read((char *) &binbookstore,sizeof(binbookstore));
      bookStore.name=binbookstore.name;
      bookStore.nextId=binbookstore.nextId;
      fichero.read((char *) &bookbin, sizeof(bookbin));
      while(!fichero.eof()){
        bookStore.books.push_back(binarytoBook(bookbin));
         fichero.read((char *) &bookbin, sizeof(bookbin));
      }

    }
    else{
      error(ERR_FILE);
      return;
    }
}
/*Función intermedia para importar desde un archivo binario que el usuario ha elegido en el menú
  PARÁMETROS: La librería que más tarde añadiremos los libros
*/
void loadData(BookStore &bookStore){
  bool repeat;  //booleano que indica si la petición ha sido correcta y hay que repetir la instrucción
  do{
  repeat=false;
  string erase_request=userRequest(ERASE_REQUEST);  //Antes de añadir nada primero borraremos todos los datos que tenemos en la tienda
  if(erase_request.size()==1 && (erase_request=="Y" || erase_request=="y")){
    string namefile=userRequest(FILENAME_REQUEST);
    bookStore.books.clear();
    bookStore.nextId=1;
    loadBin(namefile,bookStore);
  }
  else if(erase_request=="N" || erase_request=="n"){
    return;
  }
  else{
    repeat=true;
  }
  }while(repeat==true);
}
/*Función que convierte de libro a binario
  PARÁMETROS: Libro 
  RETORNO: Libro en estructura binaria
*/
BinBook booktoBinary(const Book &book){
  BinBook binbook;
  binbook.id=book.id;
  strcpy(binbook.title,book.title.c_str());
  strcpy(binbook.authors,book.authors.c_str());
  binbook.year=book.year;
  strcpy(binbook.slug,book.slug.c_str());
  binbook.price=book.price;
  return binbook;
}
/*Función que escribe en un fichero los datos de la tienda
  *PARÁMETROS: Librería, el fichero que queremos escribir
*/
void writeBookStore(const BookStore &bookStore, ofstream &fichero){
  if(fichero.is_open()){
    BinBookStore binBookStore;
    strcpy(binBookStore.name,bookStore.name.c_str());
    binBookStore.nextId=bookStore.nextId;
    fichero.write((const char *) &binBookStore ,sizeof(BinBookStore));
  }

}
/*Función que escribe en un fichero binario los datos de un libro
  *PARÁMETROS: Librería, el fichero binario que queremos escribir
*/
void writeBook(const BookStore &bookStore,ofstream &fichero){
  BinBook binbook;
  if(fichero.is_open()){
    for(unsigned int i=0;i<bookStore.books.size();i++){
      binbook=booktoBinary(bookStore.books[i]);
      fichero.write((const char *) &binbook, sizeof(BinBook));

    }
    fichero.close();
  }
}
/*Función intermedia que escribe los datos en un fichero binario
  PARÁMETROS: Librería
*/
void saveData(const BookStore &bookStore){
  ofstream fichero;
  string namefile=userRequest(FILENAME_REQUEST);
  fichero.open(namefile,ios::out|ios::binary);
  if(fichero.is_open()){
    writeBookStore(bookStore,fichero);
    writeBook(bookStore,fichero);
    fichero.close();
  }
  else{
    error(ERR_FILE);
  }
}
/*Función que gestiona el menú de importar/exportar y las opciones que el usuario elige
  PARÁMETROS: Librería
*/
void importExportMenu(BookStore &bookStore) {
  char option;
   do {
    showImportMenu();
    cin >> option;
    cin.get();

    switch (option) {
      case '1':
        importFromCsv(bookStore);
        break;
      case '2':
        exportToCsv(bookStore);
        break;
      case '3':
        loadData(bookStore);
        break;
      case '4':
        saveData(bookStore);
        break;
      case '5':
        importExportMenu(bookStore);
        break;
      case 'b':
        return;
      default:
        error(ERR_OPTION);
    }
  } while (option != 'b');
}
/*Función que comprueba los argumentos (si los hay)para saber el orden y el tipo de importación de datos 
  PARÁMETROS: Número de argumentos (contando el programa), los argumentos y la librería
  RETORNO: Importacíon por fichero de texto o por fichero binario o solamente ejecución del programa
*/
void checkArguments(int argc,char *argv[],BookStore &bookStore){
  bool correct=true;
  Book book;
  if(argc % 2 ==0 || argc>5){//Si algun agumento del tipo "-i" o "-l" no tiene fichero correspondiente
    error(ERR_ARGS);
    exit(0);
  }else if(argc==3){
    if(strcmp(argv[1],"-i")==0){
      load_CSV(argv[2],correct,bookStore);
    }
    else if(strcmp(argv[1],"-l")==0){
      loadBin(argv[2],bookStore);
    }
    else{
      error(ERR_ARGS);
      exit(0);
    }
  }
  else if(argc==5){
    if(strcmp(argv[1],"-i")==0 && strcmp(argv[3],"-l")==0){
        loadBin(argv[4],bookStore);
        load_CSV(argv[2],correct,bookStore);
      }
      else if(strcmp(argv[1],"-l")==0 && strcmp(argv[3],"-i")==0){
      loadBin(argv[2],bookStore);
    }
      else {
        error(ERR_ARGS);
        exit(0);
      }
  }
}
int main(int argc, char *argv[]) {
  BookStore bookStore;
  bookStore.name = "My Book Store";
  bookStore.nextId = 1;
  checkArguments(argc,argv,bookStore);
  char option;
  do {
    showMainMenu();
    cin >> option;
    cin.get();

    switch (option) {
      case '1':
        showCatalog(bookStore);
        break;
      case '2':
        showExtendedCatalog(bookStore);
        break;
      case '3':
        addBook(bookStore);
        break;
      case '4':
        deleteBook(bookStore);
        break;
      case '5':
        importExportMenu(bookStore);
        break;
      case 'q':
        break;
      default:
        error(ERR_OPTION);
    }
  } while (option != 'q');

  return 0;
}
