#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigInteger.h"

struct BigInteger initialize(char *s){
    struct BigInteger *newBig = (struct BigInteger *)malloc(sizeof(struct BigInteger));

    // handling DMA failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing properties of newBig
    newBig->length = 0;
    newBig->head = newBig->tail = NULL;
    newBig->sign = '+';

    // required variables
    char loop_control = s[0];
    unsigned short int isFirstDigit = 1;

    while(loop_control != '\0' && loop_control != '\n'){

        // handling sing of inputted integer
        if(isFirstDigit){
            if(s[0] == '-'){
                newBig->sign = '-';
                s += 1;
            } 

            else if(s[0] == '+'){
                newBig->sign = '+';
                s += 1;
            }
            
            else{
                newBig->sign = '+';
                s += 0;
            }

            isFirstDigit = 0;
        }

        // inserting digits into linked list
        if(s[0] >= '0' && s[0] <= '9'){
            insertAtTail(newBig, (s[0] - '0'));
        } 
        else {
            break;
        }

        s += 1;
        loop_control = s[0];
    }

    return *newBig;
}

// --------------- MAIN FUNCTIONS -----------------

struct BigInteger add(struct BigInteger list1, struct BigInteger list2){

    // perform this operation only for like signed lists
    if(list1.sign == '-' && list2.sign == '+'){
        list2.sign = '-';
        return sub(list1, list2);
    }
    else if(list1.sign == '+' && list2.sign == '-'){
        list2.sign = '+';
        return sub(list1, list2);
    }

    struct BigInteger *newBig = (struct BigInteger *)malloc(sizeof(struct BigInteger));

    // handling DMA failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing properties of newBinary
    newBig->head = newBig->tail = NULL;
    newBig->length = 0;
    newBig->sign = list2.sign;

    // required variables
    Node *itr1 = list1.tail, *itr2 = list2.tail;
    unsigned short int carry = 0;


    // loop runs till both the lists becomes null
    while(itr1 != NULL || itr2 != NULL){
        unsigned short int val = 0;

        // handling cases of lists being null
        if(itr1 != NULL && itr2 != NULL){
            val = (itr1->data + itr2->data + carry);
        }
        else if(itr1 != NULL && itr2 == NULL){
            val = (itr1->data + carry);
        }
        else if(itr1 == NULL && itr2 != NULL){
            val = (itr2->data + carry);
        }
        else{
            break;
        }
        
        // checking for value and carry
        if(val >= 10){
            carry = 1;
            val -= 10;
        } else{
            carry = 0;
            val = val;
        }

        // pushing a newNode with new Data into the head of this BigInt
        insertAtHead(newBig, val);

        itr1 = (itr1 != NULL) ? itr1->prev : NULL;
        itr2 = (itr2 != NULL) ? itr2->prev : NULL;

        newBig->length += 1;
    }

    if(carry){
        insertAtHead(newBig, 1);
        carry = 0;
        newBig->length += 1;
    }

    return *newBig;
}

struct BigInteger sub(struct BigInteger list1, struct BigInteger list2){

    // comparing two lists
    short int comparision_result = compareForMag(list1, list2);

    // handling cases for different magnitudes
    if(list1.sign == '+' && list2.sign == '-'){
        list2.sign = '+';
        return add(list1, list2);
    }
    else if(list1.sign == '-' && list2.sign == '+'){
        list2.sign = '-';
        return add(list1, list2);
    }
    else{
        // pass
    }

    struct BigInteger *newBig = (struct BigInteger *)malloc(sizeof(struct BigInteger));

    // handling DMA failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing properties of newBinary
    newBig->head = newBig->tail = NULL;
    newBig->length = 0;


    // interchanign list1 and list2 to keep smaller list at list2
    if(comparision_result == -1){
        struct BigInteger tmp = list1;
        list1 = list2;
        list2 = tmp;
        list2.sign = '-';
    } else{
        // pass
    }

    // required variables
    Node *itr1 = list1.tail, *itr2 = list2.tail;
    short int borrow = 0;
    
    while(itr1 != NULL){
        int diff = 0;
        int digit1 = (itr1 != NULL) ? itr1->data : 0;
        int digit2 = (itr2 != NULL) ? itr2->data : 0;

        if (borrow == 1) {
            digit1 -= 1;
            borrow = 0; 
        }

        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        }

        diff = digit1 - digit2;
        insertAtHead(newBig, diff);

        itr1 = itr1->prev;
        if (itr2 != NULL) {
            itr2 = itr2->prev;
        }
    }

    newBig->sign = list2.sign;
    removeZeroes(newBig);
    return *newBig;
}

struct BigInteger mul(struct BigInteger list1, struct BigInteger list2) {
    struct BigInteger *newBig = (struct BigInteger*)malloc(sizeof(struct BigInteger));

    // handling DMA failure
    if(newBig == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    // initializing newBig
    newBig->head = newBig->tail = NULL;
    newBig->length = 0;

    //handling sign
    if(list1.sign == list2.sign){
        newBig->sign = '+';
    } else{
        newBig->sign = '-';
    }

    // putting list with greater length as list1
    if(list1.length < list2.length){
        struct BigInteger swap = list1;
        list1 = list2;
        list2 = swap;
    }

    // required variables
    struct BigInteger tmp;
    int trailing_zeroes = 0;

    // for accessing elements of list2
    while (list2.tail != NULL) {

        int carry = 0;

        // initializing tmp every time
        tmp.head = tmp.tail = NULL;
        tmp.length = 0;
        tmp.sign = '+';

        // adding zeroes
        for (int i = 0; i < trailing_zeroes; i++) {
            insertAtTail(&tmp, 0);
        }

        Node *itr = list1.tail;

        // for accessing elements of list1
        while (itr != NULL) {
            int product = (itr->data * list2.tail->data + carry);
            
            carry = product / 10;
            product = product % 10;
            
            insertAtHead(&tmp, product);
            itr = itr->prev;
        }

        // adding last carry-if
        if (carry != 0) {
            insertAtHead(&tmp, carry);
        }

        // finally adding to the main list
        *newBig = add(*newBig, tmp);
        // recalculating length
        newBig->length = calculateLength(newBig);

        list2.tail = list2.tail->prev;
        trailing_zeroes += 1;
    }

    removeZeroes(newBig);
    return *newBig;
}

struct BigInteger div1(struct BigInteger list1, struct BigInteger list2){

    // required variables
    struct BigInteger ans = initialize("");
    struct BigInteger quotient = initialize("0");
    struct BigInteger one = initialize("1");
    
    // handling sign
    char keep_sign = '+';
    if(list1.sign != list2.sign){
        keep_sign = '-';
    }

    // for the sake of simplicity
    list1.sign = list2.sign = '+';

    // div by 1
    if(compareForMag(list2, one) == 0){
        list1.sign = keep_sign;
        return list1;
    }

    // div by zero
    else if(compareForMag(list1, quotient) == 0){
        return quotient;
    }

    // div by zero
    else if(compareForMag(list2, quotient) == 0){
        return ans;
    }

    // div by greater number
    else if(compareForMag(list1, list2) == -1){
        return quotient;
    }

    // div by same number
    else if(compareForMag(list1, list2) == 0){
        return one;
    }

    // div greater by smaller
    else{
        ans = list1;
        
        while(compareForMag(ans, list2) != -1){
            ans = sub(ans, list2);
            quotient = add(quotient, one);
        }
        
        quotient.sign = keep_sign;
        return quotient;
    }

    return ans;
}

struct BigInteger mod(struct BigInteger list1, struct BigInteger list2) {
    
    // required variables
    struct BigInteger zero = initialize("0");
    struct BigInteger one = initialize("1");
    struct BigInteger nothing = initialize("");

    // mod by same number
    if(compareForMag(list1, list2) == 0){
        return nothing;
    }

    // mod by greater number
    else if(compareForMag(list1, list2) == -1){
        return list1;
    }

    // mod by zero
    else if(compareForMag(list1, zero) == 0 || compareForMag(list2, zero) == 0){
        return zero;
    }

    // mod by one
    else if(compareForMag(list2, one) == 0){
        return zero;
    }

    // mod of greater by smaller number
    else{
        struct BigInteger quotient = div1(list1, list2);
        struct BigInteger product = mul(quotient, list2);
        struct BigInteger modulus = sub(list1, product);
        return modulus;
    }

    return nothing;
}



// --------------- HELPER FUNCTIONS ---------------

void insertAtHead(struct BigInteger *list, int val){
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));

    // handling DMA failure
    if(newNode == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    newNode->data = val;

    // corner case : no head node
    if(list->head == NULL){
        newNode->next = newNode->prev = NULL;
        list->head = list->tail = newNode;
        list->length += 1;
        return;
    }

    (list->head)->prev = newNode;
    newNode->prev = NULL;

    newNode->next = (list->head);
    (list->head) = newNode;
    list->length += 1;
}

void insertAtTail(struct BigInteger *list, int val){
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));

    // handling DMA failure
    if(newNode == NULL){
        printf("Memory Allocation Failed\n");
        exit(0);
    }

    newNode->data = val;

    if(list->head == NULL){
        newNode->next = newNode->prev = NULL;
        list->head = list->tail = newNode;
        list->length += 1;
        return;
    }

    newNode->next = NULL;
    newNode->prev = list->tail;
    (list->tail)->next = newNode;
    list->tail = newNode;
    list->length += 1;
}

int compareForMag(struct BigInteger list1, struct BigInteger list2){
    // list1 > lsit2
    if(list1.length > list2.length){
        return 1;
    }

    // list2 > list1
    if(list1.length < list2.length){
        return -1;
    }

    Node *itr1 = list1.head, *itr2 = list2.head;

    while(itr1 != NULL && itr2 != NULL){
        if(itr1->data > itr2->data){
            return 1;
        }

        if(itr1->data < itr2->data){
            return -1;
        }

        itr1 = itr1->next;
        itr2 = itr2->next;
    }
    
    if(itr1 == NULL && itr2 == NULL){
        return 0;
    }
    return 0;
}

void removeZeroes(struct BigInteger *list){
    if(list->head == NULL){
        return;
    }

    while(list->head->data == 0 && list->head->next != NULL){
        Node* tmp = list->head;
        list->head = list->head->next;
        list->head->prev = NULL;
        list->length -= 1;
        free(tmp);
    }
}

void freeBigInteger(struct BigInteger *list)
{
    Node *current = list->head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int calculateLength(struct BigInteger *list) {
    // required variables
    int len = 0;
    Node *current = list->head;

    while (current != NULL) {
        len++;
        current = current->next;
    }
    return len;
}


// -------------------- DISPLAY ----------------------

void display(struct BigInteger list){
    
    Node *head = list.head;
    
    // corner case : no head node
    if(head == NULL){
        printf("NULL\n");
        return;
    }

    printf("%c", list.sign);

    while(head != NULL){
        printf("%d", head->data);
        head = head->next;
    }
    printf("\n");
}
