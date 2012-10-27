#ifndef BOOK_H
#define BOOK_H

#include "common.h"

class Book;
class Entry     //address entry
{
friend class Book;
  private:
    string name;
    string tel;
    string addr;
    
  public:
    Entry* next;
    void Print(){
        cout<<endl<<"name: "<<name<<endl<<"mobile: "<<tel<<endl<<"address: "<<addr<<endl;
    }
    Entry(string n, string t, string a){
        name=n;
        tel=t;
        addr=a;
        next=NULL;
    }
    Entry(){next=NULL;}
    bool Validator();  //a validator ensuring the attribute satisfy our format
    STATUS store(ofstream& logfile); //save entry data to file
      
};

class Book  //book with entry list
{
  private:
    Entry *head;
    int size;

    STATUS SearchName(string str);
    STATUS SearchTel(string str);
    STATUS SearchAddr(string str);
    STATUS RemoveName(string str);
    STATUS RemoveTel(string str);
    STATUS RemoveAddr(string str);
    STATUS Add(Entry* node);
    STATUS Remove(Entry* prenode);
  //  STATUS RemoveHead();
 
  public:
    bool IsMatch(string s,string matcher);  //matching two string
    STATUS Search(string str,int type);     //search for entry with area and string
    STATUS Add(string n,string t,string a); //add entry with given name, mobil and address
    STATUS Remove(string str,int type);     //search for entry with area and string then remove it
    
    int GetSize(){return size;}  //return size of the book
    STATUS BookRestore(ifstream& logfile);  //restore book from file
    STATUS BookStore(ofstream& logfile);    //save book to file

    Book(ifstream& logfile);   //construction with file input
    Book(void);       //default construction
    ~Book();
};



#endif
