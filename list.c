
SplitList makeSplitList(){
    SplitList lst;
    lst.head = lst.tail = NULL;
    return lst;
}
 
int isEmptyList(const SplitList* lst){
    return lst->head == NULL;
}
 
void insertLineToTail(SplitList* lst, SplitLine* line){
    if(isEmptyList(lst)){
        lst->head = lst->tail = line;
    }
    else{
        lst->tail->next = line;
        lst->tail = line;
    }
}
 
void freeList(SplitList* list){
    SplitLine* curr = list->head;
    SplitLine* next;
    while(curr){
        next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
}
 
SymbolList makeSymbolList(){
    SymbolList lst;
    lst.head = lst.tail = NULL;
    return lst;
}
 
int isEmptysList(const SymbolList* lst){
    return lst->head == NULL;
}
 
void insertSymbolToTail(SymbolList* lst, SymbolTable* symbol){
    if(isEmptyList(lst)){
        lst->head = lst->tail = symbol;
    }
    else{
        lst->tail->next = symbol;
        lst->tail = symbol;
    }
}
 
SymbolTable* searchSymbol(SymbolTable* tmp, char* symbol){
    SymbolTable* p = tmp;
    while(p){
        if((strcmp(p->label, symbol)) == 0){
            return p;
        }
        p = p->next;
    }
    return NULL;
}
