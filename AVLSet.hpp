// AVLSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node
    {
        ElementType element;
        Node* right;
        Node* left;
    };

    Node* root;
    int AVLHeight;
    int AVLSize;

    void recPreorder(VisitFunction visit, Node* node) const;
    void recInorder(VisitFunction visit, Node* node) const;
    void recPostorder(VisitFunction visit, Node* node) const;
    void destroyNodes(Node* node) noexcept;
    Node* copyNodes(Node* node);
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    root = nullptr;
    AVLHeight = -1;
    AVLSize = 0;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    destroyNodes(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    root = nullptr;
    AVLHeight = -1;
    AVLSize = 0;

    root = copyNodes(s.root);
    AVLHeight = s.AVLHeight;
    AVLSize = s.AVLSize;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    root = nullptr;
    AVLHeight = -1;
    AVLSize = 0;

    std::swap(root, s.root);
    std::swap(AVLHeight, s.AVLHeight);
    std::swap(AVLSize, s.AVLSize);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if (this != &s)
    {
        Node* newNode = copyNodes(s.root);
        destroyNodes(root);
        root = newNode;
    }
    
    AVLHeight = s.AVLHeight;
    AVLSize = s.AVLSize;

    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    std::swap(root, s.root);
    std::swap(AVLHeight, s.AVLHeight);
    std::swap(AVLSize, s.AVLSize);
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    Node* newNode = nullptr; 
    Node* prevNode = nullptr;
    Node* curr = root;
    int dir, heightCounter = 0;

    if (AVLSize == 0)
    {
        newNode = new Node{element, newNode, newNode};
        root = newNode;
        AVLHeight = 0;
    }
    
    else
    {
        while (curr != nullptr)
        {
            prevNode = curr;
            if (curr->element > element)
            {
                curr = curr->left;
                dir = -1;
            }
            else if (curr->element < element)
            {
                curr = curr->right;
                dir = 1;
            }
            else
            {
                return;
            }

            heightCounter++;    
            if (heightCounter > AVLHeight)
            {
                AVLHeight = heightCounter;
            }
        }
        
        curr = prevNode;
        if (dir == 1)
        {
            curr->right = new Node{element, newNode, newNode};
        }
        else if (dir == -1)
        {
            curr->left = new Node{element, newNode, newNode};
        }
    }
    
    AVLSize++; 
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    Node* curr = root;
    
    while (curr != nullptr)
    {
        if (curr->element > element)
        {
            curr = curr->left;
        }
        else if (curr->element < element)
        {
            curr = curr->right;
        }
        else
        {
            return true;
        }
    }

    return false;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return AVLSize;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    return AVLHeight;
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    recPreorder(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    recInorder(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    recPostorder(visit, root);
}


template<typename ElementType>
void AVLSet<ElementType>::recPreorder(VisitFunction visit, Node* node) const
{   
    if (node != nullptr)
    {
        visit(node->element);
        recPreorder(visit, node->left);
        recPreorder(visit, node->right);
    }
}


template<typename ElementType>
void AVLSet<ElementType>::recInorder(VisitFunction visit, Node* node) const
{
    if (node != nullptr)
    {
        recInorder(visit, node->left);
        visit(node->element);
        recInorder(visit, node->right);
  
    }
}


template<typename ElementType>
void AVLSet<ElementType>::recPostorder(VisitFunction visit, Node* node) const
{
    if (node != nullptr)
    {
        recPostorder(visit, node->left);
        recPostorder(visit, node->right);
        visit(node->element);
    }
}


template<typename ElementType>
void AVLSet<ElementType>::destroyNodes(Node* node) noexcept
{
    if (node != nullptr)
    {
        destroyNodes(node->left);
        destroyNodes(node->right);
        delete node;
    }
}


template<typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::copyNodes(Node* node)
{
    Node* newNode = nullptr;

    try
    {
        if (node != nullptr)
        {
            newNode = new Node{node->element, copyNodes(node->right), copyNodes(node->left)};
        }

        return newNode;
    }
    catch (...)
    {
        destroyNodes(newNode);
        throw;
    }
}



#endif // AVLSET_HPP

