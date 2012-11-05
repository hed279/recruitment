#ifndef BOOK_H
#define BOOK_H
#include "common.h"
class Book;
struct result;
class Entry     
    //address entry
{
    friend class Book;  
private:    
    string name;    
    string tel;    
    string addr;      
public:    Entry* next;    
           void Print(){        
               cout<<endl<<"name: "<<name<<endl<<"mobile: "<<tel<<endl<<"address: "<<addr<<endl;    
           }    
           Entry(string n, string t, string a){ 
               name=n;        tel=t;        addr=a;        next=NULL;    
           }    
           Entry(){next=NULL;}   
           bool Validator();  //a validator ensuring the attribute satisfy our format    
           STATUS store(ofstream& logfile); //save entry data to file      
           void ResAddEntry(struct result& res);
           void Encode(char* buf);
};

struct result{
    char cmdcode;
    int num;
    Entry * head;
};


class Book  //book with entry list
{  
private:    
    Entry *head;    
    int size;    
    struct result SearchName(string str);    
    struct result SearchTel(string str);    
    struct result SearchAddr(string str);    
    struct result RemoveName(string str);    
    struct result RemoveTel(string str);    
    struct result RemoveAddr(string str);    
    STATUS Add(Entry* node);    
    STATUS Remove(Entry* prenode);  
    //  STATUS RemoveHead();   
public:    
    bool IsMatch(string s,string matcher);  //matching two string    
    struct result Search(string str,int type);     //search for entry with area and string    
    struct result Add(string n,string t,string a); //add entry with given name, mobil and address    
    struct result Remove(string str,int type);     //search for entry with area and string then remove it        
    int GetSize(){return size;}  //return size of the book    
    STATUS BookRestore(ifstream& logfile);  //restore book from file    
    STATUS BookStore(ofstream& logfile);    //save book to file    
    Book(ifstream& logfile);   //construction with file input    
    Book(void);       //default construction    
    ~Book();
};

#endif