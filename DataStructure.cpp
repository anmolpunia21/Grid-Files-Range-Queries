#include <bits/stdc++.h>
using namespace std;

struct Point{
    int id,x,y;
    Point(int _id,int _x,int _y){
        id=_id;
        x=_x;
        y=_y;
    }
};

class Bucket{
    public:
        int size;
        int filename;
        Bucket();
        ~Bucket();
};

class Grid{
    public:
        int x_min,x_max,y_min,y_max;
        vector<Point> points;
        Bucket *bucket;
        Grid(int x1,int x2,int y1,int y2,Bucket* _bucket){
            x_min=x1;
            x_max=x2;
            y_min=y1;
            y_max=y2;
            bucket=_bucket;
        }
        Grid(int x1,int x2,int y1,int y2,Bucket* _bucket,Point p){
            x_min=x1;
            x_max=x2;
            y_min=y1;
            y_max=y2;
            bucket=_bucket;
            points.push_back(p);
        }
};
struct Node{
    Grid* grid;
    Node* next;
    Node* prev;
    Node(Grid*);
};
 
Node::Node(Grid* _grid)       //Parameterized Constructor
{
    grid=_grid;
    next = prev = NULL;
}
 
class GridList
{
public:
    GridList();
    ~GridList();
    void insert_front(Grid*);
    void insert_back(Grid*);
    // int delete_front();
    // int delete_back();
    bool is_empty();
    // void display();
    int length();
    void search(int);
    Node* head;
    Node* tail;
    int size;
};
 
GridList::GridList()
{
    head = tail = NULL;
    size = 0;
}
 
GridList::~GridList()
{
}
 
void GridList::insert_front(Grid* value)
{
    Node* temp = new Node(value);
 
    if (head == NULL)
        head = tail = temp;
    else
    {
        head->prev = temp;
        temp->next = head;
        head = temp;
    }
    size++;
}
 
void GridList::insert_back(Grid* value)
{
    Node* temp = new Node(value);
 
    if (tail == NULL)
        head = tail = temp;
    else
    {
        tail->next = temp;
        temp->prev = tail;
        tail = temp;
    }
    size++;
}
 
// int GridList::delete_front()
// {
//     if (!is_empty())
//     {
//         Node* temp = head;
//         if (head == tail)
//         {
//             tail = NULL;
//         }
//         int delValue = temp->data;
//         head = head->next;
 
//         delete temp;
 
//         size--;
 
//         return delValue;
 
//     }
//     return 0;
// }
 
// int GridList::delete_back()
// {
//     if (!is_empty())
//     {
//         Node* temp = tail;
//         if (head == tail)
//         {
//             head = NULL;
//         }
//         int delValue = temp->data;
//         tail->next = NULL;
//         tail = tail->prev;
 
//         delete temp;
 
//         size--;
 
//         return delValue;
 
//     }
//     return 0;
// }
 
bool GridList::is_empty()
{
    if (size <= 0)
    {
        return true;
    }
 
    return false;
}
 
// void GridList::display()
// {
//     Node* temp = head;
 
//     cout << "\n\nDisplay in forward order\n";
//     while (temp != NULL)
//     {
//         cout << temp->data << " -> ";
//         temp = temp->next;
//     }
//     cout << " NULL";
// }
 
int GridList::length()
{
    return size;
}
 
void GridList::search(int value)
{
    if (!is_empty())
    {
        Node* temp = head;
        while (temp)
        {
            // if (temp->data == value)
            // {
            //     cout << "YES! got it." << endl;
            //     break;
            // }
            temp = temp->next;
        }
    }
    else
    {
        cout << "List is empty" << endl;
    }
}