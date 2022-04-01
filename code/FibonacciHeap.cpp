#include "FibonacciHeap.h"

// private functions
void FibHeap::Insert2RootList(FibNode*x) {
    // insert x to the right of min
    // update right sibling's left link
    // x's right sibling is min's right sibling
    // update min's right sibling
    //x's left sibling is min
    this->min->rightSibling->leftSibling = x;
    x->rightSibling = this->min->rightSibling;
    this->min->rightSibling = x;
    x->leftSibling = min;
}

void FibHeap::InsertHelper(FibNode*x) {
    // do insert and update
    if (this->min == nullptr) {
        x->rightSibling = x->leftSibling = x;
        this->min = x;
    }
    else {
        Insert2RootList(x);
        this->min = (x->dist < min->dist) ? x : min;
    }
}

void FibHeap::MergeRootList(FibNode *x, FibNode *y) {
     //merge two root lists
    // empty, no need to merge
    if (x == nullptr || y == nullptr) return;

    x->rightSibling->leftSibling = y->leftSibling;
    y->leftSibling->rightSibling = x->rightSibling;
    x->rightSibling = y;
    y->leftSibling = x;
}

void FibHeap::RemoveFromList(FibNode*x) {
    // remove x from list
    x->leftSibling->rightSibling = x->rightSibling;
    x->rightSibling->leftSibling = x->leftSibling;
}

void FibHeap::FibonacciLink(FibNode*x,FibNode*y) {
    // make y x's child

    // find the first child of x
    FibNode *fChild = x->firstChild;

    // make y x's child, insert at the left of fChild
    if (fChild == nullptr) {
        y->leftSibling = y;
        y->rightSibling = y;
        x->firstChild = y;
    }
    else {
        fChild->leftSibling->rightSibling = y;
        y->leftSibling = fChild->leftSibling;
        fChild->leftSibling = y;
        y->rightSibling = fChild;
    }
    y->parent = x;
    y->mark = false;
    x->degree++;
}

void FibHeap::AdjustHeap() {

    // O(logN)
    int num = (log(this->size) / log(2)) * 2, degree;
    // i_th position stores the root has degree i
    vector<FibNode *> helper(num, nullptr);

    FibNode *cur, *restore, *next = min->rightSibling, *temp;

    do {
        restore = cur = next;
        next = cur->rightSibling;
        // exist node having same degree
        degree = cur->degree;
        while (helper[degree] != nullptr) {
            // exchange two nodes
            if (cur->dist > helper[degree]->dist) {
                temp = cur;
                cur = helper[degree];
                helper[degree] = temp;
            }
            // make helper[degree] cur's child, lazy merge
            FibonacciLink(cur, helper[degree]);
            helper[degree] = nullptr;
            degree++; // after merge, degree plus 1
        }
        helper[degree] = cur;
    } while (restore != min);

    this->min = nullptr;

    // items in helper copy to root list
    for (auto &item: helper) {
        // exists, insert into root list
        if (item != nullptr) {
            InsertHelper(item);
        }
    }
}

void FibHeap::Cut(FibNode *x, FibNode *par) {
    // cut off the relation between x and its parent

    // remove x from its parent
    // only child is x
    if (x == x->rightSibling) par->firstChild = nullptr;
    RemoveFromList(x); // remove x from the list
    // avoid the case that x is the first child of its parent
    if (x == par->firstChild) par->firstChild = x->rightSibling;
    par->degree--;

    // insert to root list
    Insert2RootList(x);
    x->parent = nullptr;
    x->mark = false; // unmark
}

void FibHeap::CascadeCut(FibNode *x) {
    if (x != nullptr && x->parent != nullptr) {
        // a child has been cut, mark changed to true
        if (!x->mark) {
            x->mark = true;
        }
        else { // recursively cut
            Cut(x, x->parent);
            CascadeCut(x->parent);
        }
    }
}

// public functions
void FibHeap::Insert(const int & x,const int & y) {
    FibNode *X = new FibNode;
    X->dist = x;
    X->vertex = y;

    // store in entry array
    directEntryInHeap.insert(pair<int,FibNode*>(y,X));

    // root list is empty
    InsertHelper(X);
    this->size++;
}

FibHeap* FibHeap::Union(FibHeap *heap) {
    FibHeap *H = new FibHeap; // create new FibHeap

    MergeRootList(this->min, heap->min); // merge two root lists

    // assign to the new FibHeap
    H->min = (this->min->dist < heap->min->dist) ? this->min : heap->min;
    H->size = this->size + heap->size;
    return H;
}

FibNode* FibHeap::DeleteMin() {
    /* delete the min node, extract all children of min to the root list,
     * merge the subtrees that have degree until there doesn't exist same degree
     */
    FibNode *deletedRoot = this->min;
    FibNode *cur, *next;

    if (deletedRoot != nullptr) {
        cur = deletedRoot->firstChild;
        // traverse its children
        while (deletedRoot->degree > 0) {
            deletedRoot->degree--;
            next = cur->rightSibling; // restore the next before change
            Insert2RootList(cur); // insert into root list
            cur->parent = nullptr; // update cur's parent
            cur = next; // do next
        }
        RemoveFromList(deletedRoot); // remove min

        // FibHeap has only one node
        if (deletedRoot == deletedRoot->rightSibling) {
            this->min = nullptr;
        }
        else {
            // let min points to a new node, not minimum actually
            this->min = deletedRoot->rightSibling;
            AdjustHeap(); // adjust the heap structure, actually merge
        }
        this->size--;
        // delete the entry
        directEntryInHeap.erase(deletedRoot->vertex);
    }
    return deletedRoot;
}

void FibHeap::DecreaseKey(FibNode* x, const int & y) {
    /* Three cases
     * case1
     * [heap order not violated]
     * Decrease key of x.
     * Change heap min pointer (if necessary)
     *
     * case2 (cut & cascade cut)
     * [heap order violated]
     * Decrease key of x.
     * Cut tree rooted at x, meld into root list, and unmark.
     * If parent p of x is unmarked (hasn't yet lost a child), mark it;
     * Otherwise, cut p, meld into root list, and unmark
     * (and do so recursively for all ancestors that lose a second child)
     */
    if (this->min == nullptr) return;
    x->dist = y; // assign new value
    FibNode *par = x->parent;

    if (par != nullptr && x->dist < par->dist) {
        Cut(x, par);  // cut link between x and its parent
        CascadeCut(par); // upstairs and do cascade cut
    }
    min = (min->dist > x->dist) ? x : min; // update min
}
