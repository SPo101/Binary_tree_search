#include <stdio.h>
#include <stdlib.h>


typedef struct chain {
    int data;
    struct chain *less;
    struct chain *more;
}chain;

enum errors{
    bin_success,
    bin_invalid_par
};

int create_chain(chain *ch, int data){
    if ((ch == NULL) || (data == ch->data))
        return bin_invalid_par;
    
    if ((ch->less == NULL) && (ch->more == NULL)){
        chain *new_ch = malloc(sizeof(chain));
        new_ch->data = data;

        if (data > ch->data){
            ch->less = NULL;
            ch->more = new_ch;
        }
        if (data < ch->data){
            ch->less = new_ch;
            ch->more = NULL;
        }
    }

    if ( ((ch->less == NULL) && (ch->more != NULL))  ||  ((ch->less != NULL) && (ch->more == NULL)) ) {
        if ((ch->less == NULL) && (data < ch->data)){
            chain *new_ch = malloc(sizeof(chain));
            new_ch->data = data;
            ch->less = new_ch;
        }
        if ((ch->more == NULL) && (data > ch->data)){
            chain *new_ch = malloc(sizeof(chain));
            new_ch->data = data;
            ch->more = new_ch;
        }
        if ((ch->less != NULL) && (data < ch->data)){
            create_chain(ch->less, data);
        }
        if ((ch->more != NULL) && (data > ch->data)){
            create_chain(ch->more, data);
        }
    }

    if ((ch->less != NULL) && (ch->more != NULL)){
        if (data < ch->data){
            create_chain(ch->less, data);
        }
        if (data > ch->data){
            create_chain(ch->more, data);
        }
    }

    
    return bin_success;
}

struct chain *find_chain(chain *ch, int value){
    chain *find = malloc(sizeof(chain));
    find = ch;
    while(find->data != value){

            if (find->data < value){
                if (find->more != NULL){
                    find = find->more;
                    continue;
                }
                else 
                    return ch;
            }
            if (find->data > value){
                if (find->less != NULL)
                    find = find->less;
                else 
                    return ch;
            }
        
    }
    return find;
}

struct chain *chain_find_parent(chain *ch, int value){
    chain *find = malloc(sizeof(chain));
    find = ch;
    
    while((find->more->data != value) && (find->less->data != value)){

        if (find->data < value){
            if (find->more != NULL){
                find = find->more;
                continue;
            }
            else 
                return ch;
        }
        if (find->data > value){
            if (find->less != NULL)
                find = find->less;
            else 
                return ch;
        }

    }
    return find;
}

int chain_del_0sn(chain *ch, chain *par){
    if(ch->more != 0)
        chain_del_0sn(ch->more, ch);
    
    if(ch->less != 0)
        chain_del_0sn(ch->less, ch);
    

    if (par->more == ch)
        par->more = NULL;
    else
        par->less = NULL;

    free(ch);
    return bin_success; 
}

int chain_del_1sn(chain *ch, chain *par){

    chain *son = malloc(sizeof(chain));
    if (ch->more != NULL)
        son = ch->more;
    else 
        son = ch->less;

    if (par->more == ch)
        par->more = son;
    else
        par->less = son;

    free(ch);
    return bin_success;
}

int chain_del_2sn(chain *ch, chain *par){

    chain *next = malloc(sizeof(chain));
    next = ch->more;
    while(next->less->less != NULL){
        next = next->less;
    }
    chain *par_next = malloc(sizeof(chain));
    par_next = next;
    next = next->less;
    par_next->less = NULL;

    if (par->more == ch)
        par->more = next;
    else
        par->less = next;

    next->more = ch->more;
    next->less = ch->less;
    
    
    free(ch);
    return bin_success;
}

int chain_del(chain *ch, chain *par){
    if (ch == NULL)
        return bin_invalid_par;

    if ((ch->less == NULL) && (ch->more == NULL))//0son
        chain_del_0sn(ch, par);
    if (((ch->less != NULL) && (ch->more == NULL)) || ((ch->less == NULL) && (ch->more != NULL)))//1son
        chain_del_1sn(ch, par);
    else//2sons
        chain_del_2sn(ch, par);

    return bin_success;
}

int inorder_print(chain *ch) {
    if (ch == NULL) 
        return bin_invalid_par;

    inorder_print(ch->less);
    printf("%d -> ", ch->data);
    inorder_print(ch->more);
    
    return bin_success;
}


int main() {

    chain *root = malloc(sizeof(chain));
    root->data = 10;
    


    create_chain(root, 4);    
    create_chain(root, 15);
    create_chain(root, 21);
    create_chain(root, 14);
    create_chain(root, 7);
    create_chain(root, 3);
    create_chain(root, 8);
    create_chain(root, 6);
    create_chain(root, 5);

 
    inorder_print(root);
    
    int find_num = 7;
    printf("\npar %d", find_chain(root, find_num)->data);
    printf("\npar %d\n", chain_find_parent(root, find_num)->data);

    int del_num = 6;
    chain_del(find_chain(root, del_num), chain_find_parent(root, del_num));
    inorder_print(root);
    return 0;

}