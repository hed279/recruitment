#include "common.h"
#include "book.h"

void Entry::ResAddEntry(struct result& res){
    Entry* tmp;
    tmp=res.head;
    res.head=new Entry;
    res.head->next=tmp;
    res.head->name=this->name;
    res.head->tel=this->tel;
    res.head->addr=this->addr;

}
void Entry::Encode(char* buf){

    strcpy(&buf[4],name.c_str());
    strcpy(&buf[MAXNAME+5],this->tel.c_str());
    strcpy(&buf[MAXNAME+MAXMOBILE+6],this->addr.c_str());

}
Book::Book(void){
    head=new Entry;    
    size=0;
}
Book::Book(ifstream& logfile){
    head=new Entry;   
    size=0;  
    BookRestore(logfile);
}
Book::~Book(){  
    while (head->next!=NULL)    
        Remove(head);  
    delete head;
}
struct result Book::Search(string str,int type){  
    struct result res;  
    switch (type)  
    {   
    case 1:    
        res=SearchName(str); 
        break;   
    case 2:     
        res=SearchTel(str); 
        break;   
    case 3:     
        res=SearchAddr(str);  
        break;  
    default:   
        res.num=0; 
    }  
    return res;
}
struct result Book::SearchName(string str){ 
    struct result res;   
    res.head=NULL;
    if (size<1){
        res.num=0;
        return res;
    }
    Entry* ptr=head;  
    int cnt=0; 
    while(ptr->next!=NULL){  
        if (IsMatch(ptr->next->name,str)){  
            ptr->next->Print();
            ptr->next->ResAddEntry(res);
            ++cnt;    
        }     
        ptr=ptr->next;  
    }  
    std::cout<<cnt<<" entries are found!"<<endl;  
    res.num=cnt;
    return res;
}
struct result Book::SearchTel(string str){  
    struct result res;   
    res.head=NULL;
    if (size<1){
        res.num=0;
        return res;
    }
    Entry* ptr=head; 
    int cnt=0;  
    while(ptr->next!=NULL){  
        if (IsMatch(ptr->next->tel,str)){   
            ptr->next->Print();
            ptr->next->ResAddEntry(res);
            ++cnt;     
        }      
        ptr=ptr->next; 
    }  
    //std::cout<<cnt<<" entries are found!"<<endl; 
    res.num=cnt;
    return res;
}
struct result Book::SearchAddr(string str){  
    struct result res;   
    res.head=NULL;
    if (size<1){
        res.num=0;
        return res;
    } 
    Entry* ptr=head; 
    int cnt=0;  
    while(ptr->next!=NULL){   
        if (IsMatch(ptr->next->addr,str)){   
            ptr->next->Print();  
            ptr->next->ResAddEntry(res);
            ++cnt;     
        }      
        ptr=ptr->next;  
    }   
    //std::cout<<cnt<<" entries are found!"<<endl; 
    res.num=cnt;
    return res;
}
STATUS Book::Add(Entry * node){   
    STATUS status=0;   
    if (node==NULL)    
        return BOOK_ADD_ERROR;  
    if(!node->Validator())  
        return BOOK_ATTRIBUTE_ERROR; 
    node->next=head->next; 
    head->next=node;  
    ++size;  
    return status;     
}
struct result Book::Add(string n,string t,string a){ 
    struct result res;
    res.head=NULL;

    STATUS status=0;
    Entry* newentry=new Entry(n,t,a);  
    status=Add(newentry); 
    if (status==0)
        res.num=1;
    else
        res.num=0;
    return res;

}
STATUS Book::Remove(Entry * prenode){ 
    STATUS status=0;  
    if (prenode==NULL || prenode->next==NULL)  
        return BOOK_REMOVE_ERROR;  
    Entry *ptr=prenode->next;  
    prenode->next=ptr->next;  
    --size;  
    ptr->next=NULL;  
    delete ptr;   
    return status;
}
struct result Book::Remove(string str,int type){ 
    struct result res;
    res.head=NULL;
    switch (type)    {   
    case 1:    
        res=RemoveName(str);  
        break; 
    case 2:    
        res=RemoveTel(str);  
        break;
    case 3:   
        res=RemoveAddr(str);    
        break;   
    default:   
        res.num=0;  
    } 
    return res;
}
struct result Book::RemoveName(string str){  
    struct result res;   
    res.head=NULL;
    if (size<1){
        res.num=0;
        return res;
    } 
    STATUS status=0;
    int cnt=0;  
    /*if (IsMatch(head->name,str)){            status=RemoveHead();            ++cnt;        }*/   
    Entry* ptr=head;        
    while(ptr->next!=NULL){      
        if (IsMatch(ptr->next->name,str)){   
            status=Remove(ptr);      
            if(status==0) 
                ++cnt;         
        }      
        else        
            ptr=ptr->next;    
    }   
    std::cout<<cnt<<" entries deleted!"<<endl; 
    res.num=cnt;
    return res;
}
struct result Book::RemoveTel(string str){  
    STATUS status=0; 
    struct result res;   
    res.head=NULL;
    if (size<1){
        res.num=0;
        return res;
    }   
    int cnt=0;  
    Entry* ptr=head;    
    while(ptr->next!=NULL){  
        if (IsMatch(ptr->next->tel,str)){    
            status=Remove(ptr);   
            if(status==0)
                ++cnt;      
        }      
        else      
            ptr=ptr->next;  
    }  
    std::cout<<cnt<<" entries deleted!"<<endl;  
    res.num=cnt;
    return res;
}
struct result Book::RemoveAddr(string str){  
    STATUS status=0;   
    struct result res;   
    res.head=NULL;
    if (size<1){
        res.num=0;
        return res;
    }    
    int cnt=0;   
    Entry* ptr=head; 
    while(ptr->next!=NULL){   
        if (IsMatch(ptr->next->addr,str)){   
            status=Remove(ptr);    
            if(status==0)
                ++cnt;      
        }     
        else      
            ptr=ptr->next;   
    }   
    std::cout<<cnt<<" entries deleted!"<<endl;   
    res.num=cnt;
    return res;
}
bool Book::IsMatch(string s,string matcher){ 
    int index1,index2;   
    if (s.size()<=1 ||matcher.size()<=1) return true;   
    index1=matcher.find('*');    
    if(index1==string::npos){    
        if (s==matcher)    
            return true;    
        else          
            return false;   
    }   
    if (index1==0){     
        string tmp(matcher,1);   
        if(s.find(tmp)!=s.size()-tmp.size())    
            return false;     
        else         
            return true;   
    }   
    if (index1==matcher.size()-1){  
        string tmp(matcher,0,matcher.size()-1);  
        if(s.find(tmp)!=0)    
            return false;     
        else          
            return true;   
    }  
    if (index1>0 &&index1<matcher.size()-1){  
        string tmp(matcher,0,index1);    
        if(s.find(tmp)!=0)       
            return false;     
        tmp=string(matcher,index1+1);    
        if(s.find(tmp)!=s.size()-tmp.size())    
            return false;    
        return true;   
    }    //return IsMatch(s,matcher);   
    return false;
}
STATUS Book::BookRestore(ifstream& logfile){  
    STATUS status=0; 
    struct result res;  
    string n,t,a;   
    while(!logfile.eof()){ 
        n="";t="";a="";    
        logfile>>n>>t>>a;      
        if(n!="" && t!="" && a!="")   
            res=this->Add(n,t,a);    
        //if(res.num==0) return ; 
    }   
    return status;
}
STATUS Book::BookStore(ofstream& logfile){ 
    STATUS status=0;  
    if(!logfile) return 1;   
    Entry *ptr=head; 
    while(ptr->next!=NULL){   
        status=ptr->next->store(logfile);   
        if(status!=0)
            return status;    
        ptr=ptr->next;  
    }   
    return status;
}

bool Entry::Validator(){  
    if (name.size()<1 ||tel.size()<10 || addr.size()<1)   
        return false; 
    for(int i=0;i<tel.size();++i)  
        if(tel[i]<'0' || tel[i]>'9')   
            return false;  
    return true;
}
STATUS Entry::store(ofstream& logfile){  
    STATUS status=0;  
    if(!logfile)     
        return BOOK_FILE_ERROR;  
    logfile<<name<<endl<<tel<<endl<<addr<<endl; 
    return status;
}
