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
    for(int i=0;i<bookStore.books.size();i++){
      cout<<bookStore.books[i].id<<". "<<bookStore.books[i].title
      <<" ("<<bookStore.books[i].year<<"), "<<bookStore.books[i].price<<endl;
    }
}
void showExtendedCatalog(const BookStore &bookStore) {
  for(int i=0;i<bookStore.books.size();i++){
    cout<<"\""<<bookStore.books[i].title<<"\",\""
        <<bookStore.books[i].authors<<"\","
        <<bookStore.books[i].year<<",\""
        <<bookStore.books[i].slug<<"\","
        <<bookStore.books[i].price<<endl;
  }
}
string checkTitle(){
  string title;
  bool correcto=true;
  do{
    correcto=true;
    title="";
    cout<<"Enter book title: ";
    getline(cin,title);
    if(title.length()==0){
        error(ERR_BOOK_TITLE);
        correcto=false;
    }else{
    for(int i=0;i<title.length();i++){
      char letra=title[i];
      int s=isalpha(letra);
      int j=isspace(letra);
      if(isalnum(letra)==0 && isspace(letra)==0 && letra!=':' && letra!=',' && letra!='-'){
        error(ERR_BOOK_TITLE);
        correcto=false;
      }
    }
    }

  }while(correcto==false);
   return title;


}
string checkAuthors(){
  string authors;
  bool correcto=true;
  do{
    correcto=true;
    authors="";
    cout<<"Enter author(s): ";
    getline(cin,authors);
    if(authors.length()==0){
      error(ERR_BOOK_AUTHORS);
      correcto=false;
    }else{
    for(int i=0;i<authors.length();i++){
      char letra=authors[i];
      if(authors.length()==0 && isalnum(letra)==0 && isspace(letra)==0 && letra!=':' && letra!=',' && letra!='-'){
        error(ERR_BOOK_TITLE);
        correcto=false;
      }
    }
    }
  }while(correcto==false);
  return authors;
}
int checkYear(){
  int year;
  bool correcto=true;
  do{
    correcto=true;
    year=0;
    cout<<"Enter publication year: ";
    scanf("%d",&year);
    if(year<1440 || year>2022){
      error(ERR_BOOK_DATE);
      correcto=false;
    }
  }while(correcto==false);
  cin.ignore();
  return year;
}
string createSlug(string title){
  string slug;
  char letra;
  char letra2;
  for(int i=0;i<title.length();i++){

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
    cout<<"Enter price: ";
    getline(cin,price);
    if(price.length()==0){
        error(ERR_BOOK_PRICE);
        correcto=false;
    }else{
      for(int i=0;i<price.length();i++){
        char letra=price[i];
        int s = isalpha(price[i]);
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
  book.title=checkTitle();
  title=book.title;
  book.slug=createSlug(title);
  book.authors=checkAuthors();
  book.year=checkYear();
  price=checkprice();
  book.price=stof(price);
  bookStore.nextId++;
  return book;

}
void addBook(BookStore &bookStore) {
bookStore.books.push_back(demandBook(bookStore));
}

void deleteBook(BookStore &bookStore) {
  int id;
  cout<<"Enter book id: ";
  cin>>id;
  for(int i=0;i<bookStore.books.size();i++){
    if(bookStore.books[i].id==id){
      bookStore.books.erase(bookStore.books.begin()+(id-1));
    }
  }
  cin.ignore();
}
BinBook bookToBinary(const BookStore &bookStore){
  BinBook binbook;
  binbook.id=bookStore.books[0].id;
  strncpy(binbook.title,bookStore.books[0].title.c_str(),KMAXSTRING);
  strncpy(binbook.authors,bookStore.books[0].authors.c_str(),KMAXSTRING);
  binbook.year=bookStore.books[0].year;
  strncpy(binbook.slug,bookStore.books[0].slug.c_str(),KMAXSTRING);
  binbook.price=bookStore.books[0].price;
  return binbook;
}
void importExportMenu(BookStore &bookStore) {
  BinBook binbook;
  binbook=bookToBinary(bookStore);
}

void importFromCsv(BookStore &bookStore){
}

void exportToCsv(const BookStore &bookStore){
}

void loadData(BookStore &bookStore){
}

void saveData(const BookStore &bookStore){
}

int main(int argc, char *argv[]) {
  BookStore bookStore;
  bookStore.name = "My Book Store";
  bookStore.nextId = 1;

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
