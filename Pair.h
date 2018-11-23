#ifndef PAIR_H
#define PAIR_H


#include "socketThread.h"
//Struct for pairs
//===========================
//custom pair class, avoided pair::pair from c++ for custom call functions
class Pair{
    public:
        SocketThread* left;
        SocketThread* right;

        //constructor
        Pair(){
            left = NULL;
            right = NULL;
        }

        //destructor
        ~Pair() { }

        //--Don't know if necessary {========
        //                                  =
        //get right                         =
        //--returns address of thread       =
        &SocketThread getRight();
        //--returns address of thread       =
        &SocketThread getLeft();
        //}==================================

        //get other
        //--return thread pointed at (or returns address of thread pointed at?)
        //needed here?
        SocketThread getPartner(SocketThread &input);

        //is empty?
        bool isEmpty();

        //is full?
        bool isFull();
        //add 
        //--implement logic to ensure is full is not true on enter from vector
        //--vector, call empty. If empty returns false, then always
        //--adds to the right side. else add to the left
        void Add(SocketThread &input);
};
//============================

#endif