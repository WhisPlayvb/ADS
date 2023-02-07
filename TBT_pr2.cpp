// Complete C++ program to demonstrate deletion in threaded BST
#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    Node *left, *right;
    int info;

    // True if left pointer points to predecessor in Inorder Traversal
    bool lthread;

    // True if right pointer points to successor in Inorder Traversal
    bool rthread;

    /*
    Node(int data)
    {
        info=data;
        left=NULL;
        right=NULL;
        lthread=true;
        rthread=true;
    }
    */

    // Insert a Node in Binary Threaded Tree
    Node *insert(Node *root, int ikey)
    {
        // Searching for a Node with given value
        Node *ptr = root;

        Node *par = NULL; // Parent of key to be inserted
        while (ptr != NULL)
        {
            // If key already exists, return
            if (ikey == ptr->info)
            {
                cout << endl
                     << "\t\t Duplicate Key!";
                return root;
            }

            par = ptr; // Update parent pointer

            // Moving on left subtree.
            if (ikey < ptr->info)
            {
                if (ptr->lthread == false)
                    ptr = ptr->left;
                else
                    break;
            }
            else
            { // Moving on right subtree.
                if (ptr->rthread == false)
                    ptr = ptr->right;
                else
                    break;
            }
        }

        // Create a new Node
        Node *tmp = new Node;
        tmp->info = ikey;
        tmp->lthread = true;
        tmp->rthread = true;

        if (par == NULL)
        {
            root = tmp;
            tmp->left = NULL;
            tmp->right = NULL;

            // cout<<endl<<root<<"=>"<<root->lthread<<" "<<root->left<<" "<<"=="<<root->info<<"=="<<root->right<<" "<<root->rthread;
        }
        else if (ikey < (par->info))
        {

            tmp->left = par->left; // Prececessor
            tmp->right = par;      // Successor
            par->lthread = false;  // lthread is converted to left link T ===> False
            par->left = tmp;       // Tmp will become left child.

            // cout<<endl<<tmp<<"=>"<<tmp->lthread<<" "<<tmp->left<<" "<<"=="<<tmp->info<<"=="<<tmp->right<<" "<<tmp->rthread;
        }
        else
        {
            tmp->left = par;         // Parent will become the predecessor for its right child.
            tmp->right = par->right; // right pointer will Point to Successor
            par->rthread = false;    // Convert Thread to link.
            par->right = tmp;        // New node will become right child of parent.

            // cout<<endl<<tmp<<"=>"<<tmp->lthread<<" "<<tmp->left<<" "<<"=="<<tmp->info<<"=="<<tmp->right<<" "<<tmp->rthread;
        }
        return root;
    }

    // Recursive Inorder Traversing
    void inorder(Node *root)
    {
        if (root == NULL)
            return;

        // First recur on left subtree
        if (root->lthread == false)
            inorder(root->left);
        // Then read the data of child
        cout << root->info << " ";
        // Recur on the right subtree
        if (root->rthread == false)
            inorder(root->right);
    }

    // Recursive Preorder Traversing
    void preorder(Node *root)
    {
        if (root == NULL)
            return;

        // First read the data of child
        cout << root->info << " ";

        // Then recur on left subtree
        if (root->lthread == false)
            preorder(root->left);

        // Then Recur on the right subtree
        if (root->rthread == false)
            preorder(root->right);
    }

    // Recursive Podtorder Traversing
    void postorder(Node *root)
    {
        if (root == NULL)
            return;

        // Then recur on left subtree
        if (root->lthread == false)
            postorder(root->left);
        // Then Recur on the right subtree
        if (root->rthread == false)
            postorder(root->right);
        // First read the data of child
        cout << root->info << " ";
    }

    // Returns inorder successor using rthread (Used in inorder)
    Node *inorderSuccessor(Node *ptr)
    {
        // If rthread is set, we can quickly find
        if (ptr->rthread == true)
            return ptr->right;

        // Else return leftmost child of right subtree
        ptr = ptr->right;
        while (ptr->lthread == false)
            ptr = ptr->left;
        return ptr;
    }

    // Non-recursive Printing the threaded tree in Inorder
    void inOrder(Node *root)
    {
        if (root == NULL)
            cout << endl
                 << "\t Tree is empty";

        // Reach leftmost Node
        Node *ptr = root;
        while (ptr->lthread == false)
            ptr = ptr->left;

        // One by one print successors
        // cout<<endl<<"\t Tree is as :-";
        while (ptr != NULL)
        {
            cout << " " << ptr->info;
            ptr = inorderSuccessor(ptr);
        }
    }

    // Non-recursive Preorder Traversal of TBT
    void preOrder(Node *root)
    {
        Node *ptr;
        if (root == NULL)
        {
            cout << "Tree is empty";
            return;
        }
        ptr = root;
        while (ptr != NULL)
        {
            cout << ptr->info << " ";
            if (ptr->lthread == false)
                ptr = ptr->left;
            else if (ptr->rthread == false)
                ptr = ptr->right;
            else
            {
                while (ptr != NULL && ptr->rthread == true)
                    ptr = ptr->right; // go to successor / parent
                if (ptr != NULL)
                    ptr = ptr->right; // go to right child of successor / parent
            }
        }
    } /*End of pre-order*/

    // Returns inorder successor using left and right children (Used in deletion)
    Node *inSucc(Node *ptr)
    {
        if (ptr->rthread == true)
            return ptr->right;

        ptr = ptr->right;
        while (ptr->lthread == false)
            ptr = ptr->left;

        return ptr;
    }

    // Returns inorder predessor using left and right children (Used in deletion)
    Node *inPred(Node *ptr)
    {
        if (ptr->lthread == true)
            return ptr->left;

        ptr = ptr->left;
        while (ptr->rthread == false)
            ptr = ptr->right;

        return ptr;
    }

    // Here 'par' is pointer to parent Node and 'ptr' is pointer to current Node.
    Node *caseA(Node *root, Node *par, Node *ptr)
    {
        // If Node to be deleted is root
        if (par == NULL)
            root = NULL;

        // If Node to be deleted is left of its parent
        else if (ptr == par->left)
        {
            par->lthread = true;
            par->left = ptr->left;
        }
        else
        {
            par->rthread = true;
            par->right = ptr->right;
        }

        // Free memory and return new root
        delete ptr;

        return root;
    }

    // Here 'par' is pointer to parent Node and 'ptr' is pointer to current Node.
    Node *caseB(Node *root, Node *par, Node *ptr)
    {
        Node *child;

        // Initialize child Node to be deleted has left child.
        if (ptr->lthread == false)
            child = ptr->left;

        // Node to be deleted has right child.
        else
            child = ptr->right;

        // Node to be deleted is root Node.
        if (par == NULL)
            root = child;

        // Node is left child of its parent.
        else if (ptr == par->left)
            par->left = child;
        else
            par->right = child;

        // Find successor and predecessor
        Node *s = inSucc(ptr);
        Node *p = inPred(ptr);

        // If ptr has left subtree.
        if (ptr->lthread == false)
            p->right = s;

        // If ptr has right subtree.
        else
        {
            if (ptr->rthread == false)
                s->left = p;
        }

        delete ptr;

        return root;
    }

    // Here 'par' is pointer to parent Node and 'ptr' is pointer to current Node.
    Node *caseC(Node *root, Node *par, Node *ptr)
    {
        // Find inorder successor and its parent.
        Node *parsucc = ptr;
        Node *succ = ptr->right;

        // Find leftmost child of successor
        while (succ->lthread == false)
        {
            parsucc = succ;
            succ = succ->left;
        }

        ptr->info = succ->info;

        if (succ->lthread == true && succ->rthread == true)
            root = caseA(root, parsucc, succ);
        else
            root = caseB(root, parsucc, succ);

        return root;
    }

    // Deletes a key from threaded BST with given root and
    // returns new root of BST.
    Node *delThreadedBST(Node *root, int dkey)
    {
        // Initialize parent as NULL and ptrent Node as root.
        Node *par = NULL, *ptr = root;

        // Set true if key is found
        int found = 0;

        // Search key in BST : find Node and its parent.
        while (ptr != NULL)
        {
            if (dkey == ptr->info)
            {
                found = 1;
                break;
            }
            par = ptr;
            if (dkey < ptr->info)
            {
                if (ptr->lthread == false)
                    ptr = ptr->left;
                else
                    break;
            }
            else
            {
                if (ptr->rthread == false)
                    ptr = ptr->right;
                else
                    break;
            }
        }

        if (found == 0)
            cout << "dkey not present in tree\n";

        // Two Children
        else if (ptr->lthread == false && ptr->rthread == false)
            root = caseC(root, par, ptr);

        // Only Left Child
        else if (ptr->lthread == false)
            root = caseB(root, par, ptr);

        // Only Right Child
        else if (ptr->rthread == false)
            root = caseB(root, par, ptr);

        // No child
        else
            root = caseA(root, par, ptr);

        return root;
    }

    // Searching in TBT
    Node *search(Node *root, int key)
    {
        if (root == NULL || root->info == key)
            return root;

        // Key is greater than root's data
        if (root->info < key && root->rthread == false)
            return search(root->right, key);

        // Key is smaller than root's data
        else if (root->info > key && root->lthread == false)
            return search(root->left, key);
    }

    // Level Order Printing
    void printLevelOrder(Node *root)
    {
        // Base Case
        if (root == NULL)
            return;

        // Create an empty queue for level order tarversal
        queue<Node *> q;

        // Enqueue Root and initialize height
        q.push(root);

        while (q.empty() == false)
        {
            // Dequeue all nodes of current level and Enqueue all nodes of next level
            cout << " ===> ";
            while (q.size() > 0)
            {
                Node *node = q.front();
                cout << node->info << " ";
                q.pop();
                if (node->left != NULL)
                    q.push(node->left);
                if (node->right != NULL)
                    q.push(node->right);
            }
        }
    }
    /*
    // Non-Recursive Postorder Traversal
    void postOrder(Node* root)
    {
        stack<Node*> S1;
        stack<Node*> S2;
        S1.push(root);
        while(!S1.empty())
        {
            S2.push(S1.top());
            Node* temp=S1.top();
            S1.pop();
            if(temp->lthread==false) S1.push(temp->left);
            if(temp->rthread==false) S1.push(temp->right);
        }
        while(!S2.empty())
        {
            Node* temp=S2.top();
            S2.pop();
            cout<<temp->data<<" ";
        }
        cout<<endl;
    }
    */
};

// Driver Program
int main()
{
    Node Tree;

    Node *root = NULL;
    int choice;
    char ch = 'Y';

    while (ch == 'Y' || ch == 'y')
    {
        cout << endl
             << "\t You Can Perform Following Operations on Threaded Binary Tree :-" << endl;
        cout << endl
             << "\t\t 1. Create" << endl
             << "\t\t 2. Insert" << endl
             << "\t\t 3. Delete" << endl
             << "\t\t 4. Search" << endl
             << "\t\t 5. Display" << endl
             << "\t\t 6. Breadth First Search / LOP" << endl
             << "\t\t 7. Quit" << endl;
        cout << endl
             << "\t Enter your Choice :- ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            // Number of nodes to be inserted
            int t;
            cout << endl
                 << "\t Enter number of nodes you want to insert :- ";
            cin >> t;
            while (t--)
            {
                int data;
                cout << endl
                     << "\t\t Enter " << t << " Element......:- ";
                cin >> data;
                root = Tree.insert(root, data);
            }
            cout << endl
                 << "\t ................TBT Constructed............" << endl;
            break;
        case 2:
            t = 0;
            cout << endl
                 << "\t Enter data to insert in TBT :- ";
            cin >> t;
            root = Tree.insert(root, t);
            cout << endl
                 << "\t\t ................Node added in TBT............";
            break;
        case 3:
            int delete_data;
            cout << endl
                 << "\t Enter the node tobe Deleted :-";
            cin >> delete_data;

            root = Tree.delThreadedBST(root, delete_data);

            cout << endl
                 << "\t\t Non-Recursive Inorder Traversal of TBT is as :- ";
            Tree.inOrder(root);

            break;
        case 4:
            // Searching
            if (root != NULL)
            {
                cout << endl
                     << "\t\t Enter the data to Search in TBT :";
                int data;
                cin >> data;

                if (Tree.search(root, data) != NULL)
                    cout << endl
                         << "\t\t ......." << data << " is found in TBT..............";
                else
                    cout << endl
                         << "\t\t ......." << data << " is not found in TBT..............";
            }
            else
                cout << endl
                     << "\t ...............TBT is Empty................";
            break;
        case 5:
            if (root != NULL)
            {
                cout << endl
                     << "\t TBT Traversals are as foll0ws :- ";

                cout << endl
                     << endl
                     << "\t\t Inorder Traversal of Tree is :- ";
                Tree.inorder(root);

                cout << endl
                     << endl
                     << "\t\t Non-recursive Inorder Traversal of Tree is :- ";
                Tree.inOrder(root);

                cout << endl
                     << endl
                     << "\t\t Preorder Traversal of Tree is :- ";
                Tree.preorder(root);

                cout << endl
                     << endl
                     << "\t\t Non-recursive Preorder Traversal of Tree is :- ";
                Tree.preOrder(root);

                cout << endl
                     << endl
                     << "\t\t Postorder Traversal of Tree is :- ";
                Tree.postorder(root);

                // cout<<endl<<endl<<"\t\t Non-recursive Postorder Traversal of Tree is :- ";
                // Tree.postOrder(root);
            }
            else
                cout << endl
                     << "\t ...............TBT is Empty................";
            break;
        case 6:
            if (root != NULL)
            {
                cout << endl
                     << "\t Breadth First Search Traversal (LOP) :- ";
                Tree.printLevelOrder(root);
            }
            else
                cout << endl
                     << "\t ...............TBT is Empty................";
            break;
        case 7:
            exit(0);
        default:
            cout << endl
                 << "\t\t ...............Invalid Choice.....Re-enter your choice Again";
            break;
        }

        cout << endl
             << endl
             << "\t Do you want to continue (Y/N)..........";
        cin >> ch;
        if (ch == 'Y' || ch == 'y')
            continue;
        else
            exit(0);
    }
    return 0;
}