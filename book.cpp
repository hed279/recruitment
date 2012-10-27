#include "common.h"
#include "book.h"


Book::Book(void)
{
    head=new Entry;
    size=0;
}

Book::Book(ifstream& logfile)
{
    head=new Entry;
    size=0;
    BookRestore(logfile);
}
Book::~Book(){
    while (head->next!=NULL)
        Remove(head);
    delete head;
}
STATUS Book::Search(string str,int type){
    STATUS status=0;
    switch (type)
    {
    case 1:
        status=SearchName(str);
        break;
    case 2:
        status=SearchTel(str);
        break;
    case 3:
        status=SearchAddr(str);
        break;
    default:
        status=BOOK_SEARCH_ERROR;
    }
    return status;
}

STATUS Book::SearchName(string str){
    STATUS status=0;
    if (size<1)
        return BOOK_EMPTY;
    Entry* ptr=head;
    int cnt=0;
    while(ptr->next!=NULL){
        if (IsMatch(ptr->next->name,str)){
            ptr->next->Print();
            ++cnt;
        }
        ptr=ptr->next;
    }
    std::cout<<cnt<<" entries are found!"<<endl;
    return status;
}

STATUS Book::SearchTel(string str){
    STATUS status=0;
    if (size<1)
    {
        return BOOK_EMPTY;
    }
    Entry* ptr=head;
    int cnt=0;
    while(ptr->next!=NULL){
        if (IsMatch(ptr->next->tel,str)){
            ptr->next->Print();
            ++cnt;
        }
        ptr=ptr->next;
    }
    std::cout<<cnt<<" entries are found!"<<endl;
    return status;
}

STATUS Book::SearchAddr(string str){
    STATUS status=0;
    if (size<1)
    {
        return BOOK_EMPTY;
    }
    Entry* ptr=head;
    int cnt=0;
    while(ptr->next!=NULL){
        if (IsMatch(ptr->next->addr,str)){
            ptr->next->Print();
            ++cnt;
        }
        ptr=ptr->next;
    }
    std::cout<<cnt<<" entries are found!"<<endl;
    return status;
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

STATUS Book::Add(string n,string t,string a){
    STATUS status=0;
    Entry* newentry=new Entry(n,t,a);
    status=Add(newentry);
    return status;
    
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

STATUS Book::Remove(string str,int type){
    STATUS status=0;
    switch (type)
    {
    case 1:
        status=RemoveName(str);
        break;
    case 2:
        status=RemoveTel(str);
        break;
    case 3:
        status=RemoveAddr(str);
        break;
    default:
        status=BOOK_REMOVE_ERROR;
    }
    return status;
}

STATUS Book::RemoveName(string str){
    STATUS status=0;
    if (size<1)
    {
        return BOOK_EMPTY;
    }
    
    int cnt=0;
    /*if (IsMatch(head->name,str)){
            status=RemoveHead();
            ++cnt;
        }*/
    Entry* ptr=head;
    

        while(ptr->next!=NULL){
            if (IsMatch(ptr->next->name,str)){
                status=Remove(ptr);
                if(status!=0) return status;
                ++cnt;
            }
            else
                ptr=ptr->next;
        }
    std::cout<<cnt<<" entries deleted!"<<endl;
    return status;
}

STATUS Book::RemoveTel(string str){
    STATUS status=0;
    if (size<1)
    {
        return BOOK_EMPTY;
    }
    
    int cnt=0;

    Entry* ptr=head;
    while(ptr->next!=NULL){
        if (IsMatch(ptr->next->tel,str)){
            status=Remove(ptr);
            if(status!=0) return status;
            ++cnt;
        }
        else
            ptr=ptr->next;
    }
    std::cout<<cnt<<" entries deleted!"<<endl;
    return status;
}

STATUS Book::RemoveAddr(string str){
    STATUS status=0;
    if (size<1)
    {
        return BOOK_EMPTY;
    }
    
    int cnt=0;

    Entry* ptr=head;
    while(ptr->next!=NULL){
        if (IsMatch(ptr->next->addr,str)){
            status=Remove(ptr);
            if(status!=0) return status;
            ++cnt;
        }
        else
            ptr=ptr->next;
    }
    std::cout<<cnt<<" entries deleted!"<<endl;
    return status;
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
    }
    //return IsMatch(s,matcher);
    return false;

}


STATUS Book::BookRestore(ifstream& logfile){
    STATUS status=0;
    string n,t,a;
    while(!logfile.eof()){
        n="";t="";a="";
        logfile>>n>>t>>a;
        if(n!="" && t!="" && a!="")
            status=this->Add(n,t,a);
        if(status!=0) return status;
    }
    return status;

}
STATUS Book::BookStore(ofstream& logfile){
    STATUS status=0;
    if(!logfile) return 1;
    Entry *ptr=head;
    while(ptr->next!=NULL){
        status=ptr->next->store(logfile);
        if(status!=0) return status;
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