//Need to know
//-socketThread

#include "Pair.h"
#include "socketThread.h"
//Struct for pairs
//===========================
//custom pair class, avoided pair::pair from c++ for custom call functions
class Pair{
    public:

        //--Don't know if necessary {========
        //                                  =
        //get right                         =
        //--returns address of thread       =
        &SocketThread getRight(){         //=
            return *right;                //=
        }                                 //=
                                          //=
        //get left                          =
        //--returns address of thread       =
        &SocketThread getLeft(){          //=
            return *left;                 //=
        }                                 //=
        //}==================================

        //get other
        //--return thread pointed at (or returns address of thread pointed at?)
        //needed here?
        SocketThread getPartner(SocketThread &input){
            if (this.isFull()==true){   //only runs if has a partner
                if(input == left){
                    return * right;  //if the address of input and address of object pointed at by left are equal, return right object
                }
                if(input == right){
                    return * left;    
                }       //needs to be formatted better. What if it doesn't match?
            }
        }

        //is empty?
        bool isEmpty(){     //never be just right side and left side empty
            if(*left==NULL){    return true;    }
            else{   return false;   }
        }

        //is full?
        bool isFull(){
            if( (*left != NULL) && (*right != NULL) ){    return true;      }
            else{   return false;   }
        }

        //add 
        //--implement logic to ensure is full is not true on enter from vector
        //--vector, call empty. If empty returns false, then always
        //--adds to the right side. else add to the left
        void Add(SocketThread &input){      //pass Thread to this function to use
            if(isEmpty()==false){    right = input;     }        //not empty, not full, only open on right   ///only want to pass location, not copy it ########### Needs change
            else{   left = input;  }                            //value in left = address of the input
        }   
        //^not sure if getting the correct value
    }
}
//============================