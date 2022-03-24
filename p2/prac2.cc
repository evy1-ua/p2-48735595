// Programación 2 - Práctica 2
// DNI: 48735595K
// Nombre: Eric Vivancos Yagües

#include <iostream>
#include <vector>
#include <cctype>
#include <string.h>
#include <fstream>

using namespace std;

const int KMAXSTRING = 50;

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

enum Request {
  BOOK_TITLE_REQUEST,
  BOOK_AUTHORS_REQUEST,
  BOOK_YEAR_REQUEST,
  BOOK_PRICE_REQUEST,
  BOOK_ID_REQUEST,
  FILENAME_REQUEST,
  ERASE_REQUEST
};
struct Book {
  unsigned int id;
  string title;
  string authors;
  int year;
  string slug;
  float price;
};

struct BinBook {
  unsigned int id;
  char title[KMAXSTRING];
  char authors[KMAXSTRING];
  int year;
  char slug[KMAXSTRING];
  float price;
};

struct BookStore {
  string name;
  vector<Book> books;
  unsigned int nextId;
};

struct BinBookStore {
  char name[KMAXSTRING];
  unsigned int nextId;
};

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
void showImportMenu() {
  cout << "[Import/export options]" << endl
       << "1- Import from CSV" << endl
       << "2- Export to CSV" << endl
       << "3- Load data" << endl
       << "4- Save data" << endl
       << "b- Back to main menu" << endl
       << "Option: ";
}
void showCatalog(const BookStore &bookStore) {
    for(unsigned int i=0;i<bookStore.books.size();i++){
      cout<<bookStore.books[i].id<<". "<<bookStore.books[i].title
      <<" ("<<bookStore.books[i].year<<"), "<<bookStore.books[i].price<<endl;
    }
}
void showExtendedCatalog(const BookStore &bookStore) {
  for(unsigned int i=0;i<bookStore.books.size();i++){
    cout<<"\""<<bookStore.books[i].title<<"\",\""
        <<bookStore.books[i].authors<<"\","
        <<bookStore.books[i].year<<",\""
        <<bookStore.books[i].slug<<"\","
        <<bookStore.books[i].price<<endl;
  }
}
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
string checkYear(){
  string year;
  bool correcto=true;
  do{
    correcto=true;
    year="";
    year=userRequest(BOOK_YEAR_REQUEST);
    if(stoi(year)<1440 || stoi(year)>2022){
      error(ERR_BOOK_DATE);
      correcto=false;
    }
  }while(correcto==false);
  return year;
}
string createSlug(string title){
  string slug;
  for(unsigned int i=0;i<title.length();i++){

    if(slug.length()>0){
      if((isspace(title[i])!=0 || ispunct(title[i]!=0)) && slug[i-1]!='-' && title[i]!='-')
        slug.push_back('-');
        else if(isalnum(title[i])!=0){
          slug.push_back(tolower(title[i]));
        }
    }
    else if(isalnum(title[i])!=0)
    {
      slug.push_back(tolower(title[i]));
    }
    //  if (isspace(title[i]) || ispunct(title[i])){
    //    if(slug[i-1]!='-' && ispunct(title[i])==0 && slug[0]!='-'){
    //      slug.push_back('-');
    //    }
    // }else if(isalnum(title[i])!=0){
    //   slug.push_back(tolower(title[i]));
    // }
    // else{
    //   slug.push_back(title[i]);
    // }
  }
  return slug;
}
string checkprice(){
  string price;
  bool correcto=true;
  do{
    correcto=true;
    price="";
    price=userRequest(BOOK_PRICE_REQUEST);
    if(price.length()==0){
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
void addBook(BookStore &bookStore) {
bookStore.books.push_back(demandBook(bookStore));
}

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
  if(find==false){
    error(ERR_ID);
    
  }
  
}
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
        if(!fichero.eof() && checkCorrect(titulo,correct)==true && checkCorrect(titulo,correct)==true){
          bookStore.books.push_back(book);
          bookStore.nextId++;
        }
    }
     fichero.close();
    }
  else{
    error(ERR_FILE);
  }
}

void importFromCsv(BookStore &bookStore){
  string namefile;
  bool correct=true;
  namefile=userRequest(FILENAME_REQUEST);
  load_CSV(namefile,correct,bookStore);
  
}

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
void loadData(BookStore &bookStore){
  bool repeat;
  do{
  repeat=false;
  string erase_request=userRequest(ERASE_REQUEST);
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
void writeBookStore(const BookStore &bookStore, ofstream &fichero){
  if(fichero.is_open()){
    BinBookStore binBookStore;
    strcpy(binBookStore.name,bookStore.name.c_str());
    binBookStore.nextId=bookStore.nextId;
    fichero.write((const char *) &binBookStore ,sizeof(BinBookStore));
  }

}
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
void checkArguments(int argc,char *argv[],BookStore &bookStore){
  bool correct=true;
  Book book;
  if(argc % 2 ==0 || argc>5){
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
