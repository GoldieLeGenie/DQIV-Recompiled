#pragma once
#include <globaldefs.h>



//almost exactly the same as the mobile version
#pragma push
#pragma thumb off

namespace dss {
    template <typename T>
    struct Node {
        static const unsigned char NODE_NONE = 0xff;

        T value_;                               
        unsigned char parentIndex_;             
        unsigned char childIndex_;              
        unsigned char nextIndex_;               
        unsigned char prevIndex_;               

        Node();
        Node(T value);
        void setValue(T value);
        T getValue();
        void clear();
    };

    template <typename T, int N>
    struct NodeArray {
        static const int ARRAY_MAX = N;

        Node<T> array_[N];                     
        int useIndex_;                      

        NodeArray();
        void clear();
        Node<T>* getNode(T value);
        int getNodeIndex(T value);
        Node<T>& operator[](int index) { return array_[index]; }
    };

    template <typename T, int N>
    struct Tree {
        static const int NODE_NONE = -1;

        int rootNodeIndex;                      
        int currentNodeIndex;                   
        int level;                              
        NodeArray<T, N> Nodes;                  

        Tree();
        void clear();
        void setRoot(T value);
        void addChild(T value);
        void addNext(T value);
        void moveCurrentRoot();
        void moveCurrentChild();
        void moveCurrentNext();
        void moveCurrentPrev();
        void moveCurrentParent(int count);
        bool isCurrentChild();
        bool isCurrentNext();
        bool isCurrentPrev();
        bool isCurrentParent();
        Node<T>* getCurrentNode();
        int getLevel();
        virtual void display();
        void recursiveTree();
    };

    template <typename T>
    Node<T>::Node()
    {
        parentIndex_ = NODE_NONE;
        childIndex_ = NODE_NONE;
        nextIndex_ = NODE_NONE;
        prevIndex_ = NODE_NONE;
    }

    template <typename T>
    Node<T>::Node(T value)
    {
        value_ = value;
        parentIndex_ = NODE_NONE;
        childIndex_ = NODE_NONE;
        nextIndex_ = NODE_NONE;
        prevIndex_ = NODE_NONE;
    }

    template <typename T>
    void Node<T>::setValue(T value)
    {
        value_ = value;
    }

    template <typename T>
    T Node<T>::getValue()
    {
        return value_;
    }

    template <typename T>
    void Node<T>::clear()
    {
        parentIndex_ = NODE_NONE;
        childIndex_ = NODE_NONE;
        nextIndex_ = NODE_NONE;
        prevIndex_ = NODE_NONE;
    }

    template <typename T, int N>
    NodeArray<T, N>::NodeArray()
    {
        useIndex_ = 0;
    }

    template <typename T, int N>
    void NodeArray<T, N>::clear()
    {
        useIndex_ = 0;
        for (int i = 0; i < ARRAY_MAX; i++) {
            array_[i].clear();
        }
    }

    template <typename T, int N>
    Node<T>* NodeArray<T, N>::getNode(T value)
    {
        array_[useIndex_].setValue(value);
        return &array_[useIndex_++];
    }

    template <typename T, int N>
    int NodeArray<T, N>::getNodeIndex(T value)
    {
        array_[useIndex_].setValue(value);
        return useIndex_++;
    }

    template <typename T, int N>
    Tree<T, N>::Tree() : rootNodeIndex(NODE_NONE), currentNodeIndex(NODE_NONE)
    {
    }

    template <typename T, int N>
    void Tree<T, N>::clear()
    {
        rootNodeIndex = NODE_NONE;
        currentNodeIndex = NODE_NONE;
        Nodes.clear();
    }

    template <typename T, int N>
    void Tree<T, N>::setRoot(T value)
    {
        int index = Nodes.getNodeIndex(value);
        if (rootNodeIndex == NODE_NONE) {
            rootNodeIndex = index;
        }
        currentNodeIndex = rootNodeIndex;
        level = 0;
    }

    template <typename T, int N>
    void Tree<T, N>::addChild(T value)
    {
        int index = Nodes.getNodeIndex(value);
        Nodes[currentNodeIndex].childIndex_ = index;
        Nodes[index].parentIndex_ = currentNodeIndex;
        moveCurrentChild();
    }

    template <typename T, int N>
    void Tree<T, N>::addNext(T value)
    {
        int index = Nodes.getNodeIndex(value);
        Nodes[currentNodeIndex].nextIndex_ = index;
        Nodes[index].parentIndex_ = Nodes[currentNodeIndex].parentIndex_;
        Nodes[index].prevIndex_ = currentNodeIndex;
        moveCurrentNext();
    }

    template <typename T, int N>
    void Tree<T, N>::moveCurrentRoot()
    {
        currentNodeIndex = rootNodeIndex;
        level = 0;
    }

    template <typename T, int N>
    void Tree<T, N>::moveCurrentChild()
    {
        currentNodeIndex = Nodes[currentNodeIndex].childIndex_;
        level++;
    }

    template <typename T, int N>
    void Tree<T, N>::moveCurrentNext()
    {
        currentNodeIndex = Nodes[currentNodeIndex].nextIndex_;
    }

    template <typename T, int N>
    void Tree<T, N>::moveCurrentPrev()
    {
        currentNodeIndex = Nodes[currentNodeIndex].prevIndex_;
    }

    template <typename T, int N>
    void Tree<T, N>::moveCurrentParent(int count)
    {
        for (int i = 0; i < count; i++) {
            currentNodeIndex = Nodes[currentNodeIndex].parentIndex_;
            level--;
        }
    }

    template <typename T, int N>
    bool Tree<T, N>::isCurrentChild()
    {
        return Nodes[currentNodeIndex].childIndex_ != Node<T>::NODE_NONE;
    }

    template <typename T, int N>
    bool Tree<T, N>::isCurrentNext()
    {
        return Nodes[currentNodeIndex].nextIndex_ != Node<T>::NODE_NONE;
    }

    template <typename T, int N>
    bool Tree<T, N>::isCurrentPrev()
    {
        return Nodes[currentNodeIndex].prevIndex_ != Node<T>::NODE_NONE;
    }

    template <typename T, int N>
    bool Tree<T, N>::isCurrentParent()
    {
        return Nodes[currentNodeIndex].parentIndex_ != Node<T>::NODE_NONE;
    }

    template <typename T, int N>
    Node<T>* Tree<T, N>::getCurrentNode()
    {
        return &Nodes[currentNodeIndex];
    }

    template <typename T, int N>
    int Tree<T, N>::getLevel()
    {
        return level;
    }

    template <typename T, int N>
    void Tree<T, N>::display()
    {
        moveCurrentRoot();
        recursiveTree();
    }

    template <typename T, int N>
    void Tree<T, N>::recursiveTree()
    {
        while (true) {
            if (isCurrentChild()) {
                moveCurrentChild();
                recursiveTree();
                moveCurrentParent(1);
            }
            if (!isCurrentNext()) {
                return;
            }
            moveCurrentNext();
        }
    }
}

#pragma pop
