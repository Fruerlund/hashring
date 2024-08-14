/**
 * @file bst.h
 * @author Fruerlund
 * @brief 
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BST_H
#define BST_H

#include "common-defines.h"
#include "./hashring.h"


/* 
[**************************************************************************************************************************************************]
                                                            STRUCTUES AND PROTOTYPES
[**************************************************************************************************************************************************]
*/

/* 
[**************************************************************************************************************************************************]
                                                            METHODS / FUNCTIONS
[**************************************************************************************************************************************************]
*/

/**
 * @brief Finds the smallest node in a tree.
 * 
 * @param node 
 * @return node_t* 
 */
ring_element_t * bst_findmin(ring_element_t *e) {

    ring_element_t *current = e;

    while(true) {

        if( current->left != NULL ) {
            current = current->left;
            continue;
        }
        else {
            break;
        }
    }

    return current;
}



/**
 * @brief Logic for inserting a node into a tree
 * 
 * @param tree 
 * @param hash 
 * @param data 
 * @return node_t* 
 */
uint32_t bst_insert(hashring_t *r, ring_element_t *e) {

    /* Inserting into an empty tree */

    if(r->tree == NULL) {
        r->tree = e;
        e->parent = NULL;
        e->right = NULL;
        e->left = NULL;
    }

    else {
            
        /* Traverse tree and insert */
        ring_element_t *current = r->tree;

        while(true) {

            if(e->hash > current->hash) {
                /* Check if we should traverse right */
                if(current->right != NULL) {
                    current = current->right;
                    continue;
                }
                /* Insert right */
                else {
                    current->right = e;
                    e->parent = current;
                    break;
                }
            }

            if(e->hash < current->hash) {
                /* Check if we should traverse left */
                if(current->left != NULL) {
                    current = current->left;
                    continue;
                }
                /* Insert left */
                else {
                    current->left = e;
                    e->parent = current;
                    break;
                }
            }
            
            /* Should never hit */
            break;
        }
    }


    return EXIT_SUCCESS;

}

uint32_t bst_remove(hashring_t *r, ring_element_t *e) {

    ring_element_t *current = e;
    ring_element_t *s = NULL;
    
    /**
     * Case 1: Node has no children.
     */
    if( current->left == NULL && current->right == NULL) {
        if(current->parent != NULL) {
            if( current->parent->left == current) {
                current->parent->left = NULL;
            }
            
            if(current->parent->right == current) {
                current->parent->right = NULL;
            }
        }
        else {
            r->tree = NULL;
        }
    }
    
    /*
     * Case 2: Node has a two children.
     */
    else if(current->left != NULL && current->right != NULL) {

        /* Find minimum node of the right subtree */
        s = bst_findmin(current->right);
        s->parent->left = NULL;
        s->parent = NULL;
        
        /* If our node has a parent, we need to update that one*/
        if(current->parent != NULL) {

            s->parent = current->parent;

            if(current->left != NULL) {
                s->left = current->left;
                current->left->parent = s;
            }
            if(current->right != NULL) {
                s->right = current->right;
                current->right->parent = s;
            }

        }
        else {

            /* If node doesn't have a parent we are deleting the top root*/
            s->right = r->tree->right;
            s->right->parent = s;

            if(r->tree->left != NULL) {
                s->left = r->tree->left;
                r->tree->left->parent = s;
            }

            r->tree = s;

        }
    }

    
    /*
    * Case 3: Node has a single child. 
    */
    else {

        ring_element_t *child;

        if(current->left != NULL) {
            child = current->left;
        }
        else {
            child = current->right;
        }

        /* If current has no parent, we are deleting top root */
        if(current->parent == NULL) {
            child->parent = NULL;
            r->tree = child;
            return;
        }
        else {
            if (current->parent->left == current) {
                current->parent->left = child;
            } else {  
                current->parent->right = child;
            }
            child->parent = current->parent;
        }
    }


}



#endif