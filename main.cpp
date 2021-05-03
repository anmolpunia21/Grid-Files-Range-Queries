#include <bits/stdc++.h>
// #include <DataStructure.cpp>

using namespace std;
#define XMIN 0
#define XMAX 400
#define YMIN 0
#define YMAX 400
#define x_axis "x"
#define y_axis "y"
int FILE_ID = 0;
int GRID_ID = 0;
int BUCKET_SIZE = 0;

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
        string filename;
        Bucket(){}
        ~Bucket(){}
        Bucket(int &no){
            filename = "./buckets/bucket"+to_string(no)+".txt";
            no++;
            size=0;
        }
        
        void insert(Point p){
            ofstream fout;
            fout.open (filename,ios::app);
            string str = to_string(p.id)+" "+to_string(p.x)+" "+to_string(p.y)+"\n";
            fout<< str;
            fout.close();
            size++;
        }
        void insert(vector<Point> v){
            ofstream fout;
            fout.open (filename,ios::app);
            for(Point p : v){
                string str = to_string(p.id)+" "+to_string(p.x)+" "+to_string(p.y)+"\n";
                fout<< str;
                size++;
            }
            fout.close();
        }
        vector<Point> readBucket(){
            int id,x,y;
            ifstream fin;
            vector<Point> v;
            fin.open(filename);
            while (fin>>id>>x>>y){
                v.push_back(Point(id,x,y));
            }
            return v;
        }
        void erase(){
            ofstream fout;
            fout.open (filename,ios::trunc);
            string str = "";
            fout<< str;
            fout.close();
            size=0;
        }
        void remove(Point p){
            vector<Point> bucketPoints(readBucket());
            erase();
            for(Point t:bucketPoints){
                if(p.id!=t.id){
                    insert(t);
                }
            }
        }
};

class Grid{
    public:
        int x_min,x_max,y_min,y_max;
        int size,id;
        Bucket *bucket;
        Grid(int x1,int x2,int y1,int y2,Bucket* _bucket,int &_id){
            x_min=x1;
            x_max=x2;
            y_min=y1;
            y_max=y2;
            bucket=_bucket;
            size=0;
            id=_id;
            _id++;
        }
        bool is_present(Point p){
            if (p.x > x_min and p.x <= x_max and p.y > y_min and p.y <= y_max)
                return true;
            return false;
        }
        vector<Point> readGridsPointInBucket(){
            vector<Point> v(bucket->readBucket()),res;
            for(Point p:v){
                if(is_present(p)){
                    res.push_back(p);
                }
            }
            return res;
        }
        void insert(Point p){
            size++;
            bucket->insert(p);
        }
        void erase(){
            size=0;
            bucket->erase();
        }
        vector<Point> removeGridPointsFromBucket(){
            // Remove grid points from this bucket and assign the rest to the existed bucket and return grid Points
            vector<Point> gridPoints;
            vector<Point> bucketPoints(this->bucket->readBucket()),newBucketPoints;

            for(Point p : bucketPoints){
                if(!this->is_present(p)){
                    newBucketPoints.push_back(p);
                    // cout<<"Non Grid point "<<p.id<<' '<<p.x<<' '<<p.y<<'\n';
                }
                else{
                    gridPoints.push_back(p);
                    // cout<<"Grid point "<<p.id<<' '<<p.x<<' '<<p.y<<'\n';
                }
            }
            erase();
            this->bucket->insert(newBucketPoints);
            return gridPoints;
        }
        bool is_overlapping(int x_min,int x_max,int y_min,int y_max){
            if(this->x_min == this->x_max || this->y_max == this->y_min || x_min==x_max || y_min==y_max)
            return false;

            if(this->x_min >= x_max || x_min >= this->x_max)
            return false;

            if(this->y_max <= y_min || y_max <= this->y_min)
            return false;

            return true;
        }
};

struct Node{
    Grid* grid;
    Node* next;
    Node(Grid*);
};
 
Node::Node(Grid* _grid)       //Parameterized Constructor
{
    grid=_grid;
    next = NULL;
}
 
class GridList
{
public:
    GridList();
    ~GridList();
    void insert_front(Grid*);
    bool is_empty();
    void display();
    int length();
    Node* search(Point);
    void splitGrid(int,string,Node*,bool);
    void splitList(int,string);
    vector<Point> rangeQuery(int,int,int,int);
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
        temp->next = head;
        head = temp;
    }
    size++;
}

void GridList::display()
{
    Node* temp = head;   
    cout<<"====================================== Displaying Grids ===========================\n"; 
    while (temp != NULL)
    {
        cout<<"Grid:"<<temp->grid->id<<'\n';
        cout<<"Grid_Size:"<<temp->grid->size<<'\n';
        cout<<temp->grid->x_min<<' '<<temp->grid->x_max<<' '<<temp->grid->y_min<<' '<<temp->grid->y_max;
        cout<<" :: x_min x_max y_min y_max\n";
        cout<<temp->grid->bucket->filename<<'\n';
        vector<Point> v(temp->grid->readGridsPointInBucket());
        for(Point p:v){
            cout<<p.id<<' '<<p.x<<' '<<p.y<<'\n';
        }
        cout<<'\n';
        temp = temp->next;
    }
}

bool GridList::is_empty()
{
    if (size <= 0)
    {
        return true;
    }
 
    return false;
}
 
int GridList::length()
{
    return size;
}

Node* GridList::search(Point p){
    Node* temp = head;
    Node* t=NULL;
    // cout<<"in searching"<<'\n';
    while (temp)
    {
        if(temp->grid->is_present(p)){
            return temp;
        }
        temp = temp->next;
    }
    return t;
}

void GridList::splitGrid(int value,string axis,Node* temp,bool create_new_bucket){
    if(axis==x_axis){
        if(temp->grid->x_min<value and value<temp->grid->x_max){
            // if axis=value passes through the grid
            vector<Point> bucketPoints(temp->grid->bucket->readBucket());
            temp->grid->erase();
            vector<Point> l,r;
            // cout<<"In splitting x\n";
            for(Point p:bucketPoints){
                // cout<<p.id<<' '<<p.x<<' '<<p.y<<'\n';
                if(temp->grid->is_present(p)){
                    if(p.x<=value) l.push_back(p);
                    else r.push_back(p);
                }
                else l.push_back(p);
            }
            for(Point p:l)temp->grid->insert(p);

            Bucket* temp_bucket;
            if(create_new_bucket){
                Bucket* bucket= new Bucket(FILE_ID);
                temp_bucket = bucket;
            }
            else{
                temp_bucket = temp->grid->bucket;
            }
            Grid* right_split = new Grid(value,temp->grid->x_max,temp->grid->y_min,temp->grid->y_max,temp_bucket,GRID_ID);
            for(Point p:r)right_split->insert(p);
            insert_front(right_split);
            temp->grid->x_max=value;
        }
    }
    else{
        if(temp->grid->y_min<value and value<temp->grid->y_max){
            // if axis=value passes through the grid
            vector<Point> bucketPoints(temp->grid->bucket->readBucket());
            temp->grid->erase();
            vector<Point> l,r;
            // cout<<"In splitting y\n";
            for(Point p:bucketPoints){
                // cout<<p.id<<' '<<p.x<<' '<<p.y<<'\n';
                if(temp->grid->is_present(p)){
                    if(p.y<=value) l.push_back(p);
                    else r.push_back(p);
                }
                else l.push_back(p);
            }
            for(Point p:l)temp->grid->insert(p);

            Bucket* temp_bucket;
            if(create_new_bucket){
                Bucket* bucket= new Bucket(FILE_ID);
                temp_bucket = bucket;
            }
            else{
                temp_bucket = temp->grid->bucket;
            }
            Grid* right_split = new Grid(temp->grid->x_min,temp->grid->x_max,value,temp->grid->y_max,temp_bucket,GRID_ID);
            for(Point p:r)right_split->insert(p);
            insert_front(right_split);
            temp->grid->y_max=value;
        }
    }
}

void GridList::splitList(int value,string axis){
    // Split through and throughout
    Node* temp = head;
    while (temp)
    {
        splitGrid(value,axis,temp,false); // use bucket sharing now
        temp=temp->next;
    }
    
}

vector<Point> GridList::rangeQuery(int x_min,int x_max,int y_min,int y_max){
    Node* temp = head;
    vector<Point> res;
    while(temp){
        if(temp->grid->is_overlapping(x_min,x_max,y_min,y_max)){
            vector<Point> gridPoints(temp->grid->readGridsPointInBucket());
            for(Point p:gridPoints){
                if (p.x > x_min and p.x <= x_max and p.y > y_min and p.y <= y_max)
                    res.push_back(p);
            }
        }
        temp = temp->next;
    }
    return res;
}

inline int randomNumber(int n)
{
    return (rand() % (n + 1));
}

vector<Point> generateDataSet(int n)
{
    vector<Point> v;

    for (int i = 0; i < n; i++)
    {
        Point p(i, randomNumber(XMAX), randomNumber(YMAX));
        v.push_back(p);
    }
    return v;
}

string find_spread(vector<Point> dataset)
{
    int x_spread, y_spread;
    sort(dataset.begin(), dataset.end(), [](Point &a, Point &b) { return a.x < b.x; });
    x_spread = dataset[dataset.size() - 1].x - dataset[0].x;
    sort(dataset.begin(), dataset.end(), [](Point &a, Point &b) { return a.y < b.y; });
    y_spread = dataset[dataset.size() - 1].y - dataset[0].y;
    return x_spread >= y_spread ? x_axis : y_axis;
}

void insert(Point p,GridList* gridList){
    Node* node = gridList->search(p);
    if (node==NULL){
        cout<<"searching failed\n";
        return;
    }
    node->grid->insert(p);
    cout<<"Inserted Point\n";
    cout<<"Point_Id:"<<p.id<<" Grid_Id:"<<node->grid->id<<" Grid_Size:"<<node->grid->size<<" Bucket_Size:"<<node->grid->bucket->size<<'\n';
    //checking conditions
    // 1. Grid Overflow
    if(node->grid->size>BUCKET_SIZE){
        // grid size is equal to bucket size, so split the grids through and throughout
        vector<Point> gridPoints(node->grid->readGridsPointInBucket());
        // for(Point t:gridPoints)cout<<t.x<<' '<<t.y<<'\n';

        string spread = find_spread(gridPoints);
        int value;
        int half = gridPoints.size() / 2 + gridPoints.size() % 2;
        if(spread==x_axis){
            sort(gridPoints.begin(), gridPoints.end(), [](Point &a, Point &b) { return a.x < b.x; });
            value = gridPoints[half - 1].x;
        }
        else{
            sort(gridPoints.begin(), gridPoints.end(), [](Point &a, Point &b) { return a.y < b.y; });
            value = gridPoints[half - 1].y;
        }
        cout<<"Grid Overflow, Splitted at ";
        cout<<spread<<"="<<value<<'\n';
        //split the grid cell
        gridList->splitGrid(value,spread,node,true);
        //split through and through out
        gridList->splitList(value,spread);
        // gridList->display();
    }
    else if(node->grid->bucket->size>BUCKET_SIZE){
        /*
        2. Bucket Overflow
        2.1 Resolve, try to remove bucket sharing
        */
        cout<<"Bucket Overflow, Sharing buckets\n";
        Bucket *bucket = new Bucket(FILE_ID);
        vector<Point> gridPoints(node->grid->removeGridPointsFromBucket());
        node->grid->bucket = bucket;
        for(Point t:gridPoints){
            node->grid->insert(t);
        }
        cout<<"Done bucket sharing\n";
        // gridList->display();
    }
}

void intialization(GridList* head){
    Bucket* bucket= new Bucket(FILE_ID);
    Grid* temp = new Grid(XMIN,XMAX,YMIN,YMAX,bucket,GRID_ID);
    head->insert_front(temp);
}

int main(){
    BUCKET_SIZE=100;
    cout<<"Enter bucket size :";
    cin>>BUCKET_SIZE;
    GridList* gridList = new GridList();
    vector<Point> dataset(generateDataSet(3000));
    intialization(gridList);
    cout<<"=======================================================\n";
    int i=0;
    while (true)
    {
        int choice;
        cout << "Press 1: Visualize\nPress 2: Insert one point\nPress 3: Insert all point\nPress 4: Test Range Query\nPress 5 for Exit\n";
        cin >> choice;
        if (choice == 5){
            break;
        }
        else if (choice == 1)
        {
            gridList->display();
        }
        else if (choice == 2)
        {
            if(i<dataset.size()){
                insert(dataset[i],gridList);
                i++;
            }
        }
        else if(choice == 3)
        {
            for(;i<dataset.size();i++)
            insert(dataset[i],gridList);
        }
        else if(choice == 4)
        {
            int x1=150,x2=200,y1=150,y2=200;
            cout<<"Enter coordinates of rectangle in this format:- bottom-left-x, bottom-left-y, top-right-x, top-right-y\n";
            cin>>x1>>y1>>x2>>y2;
            vector<Point> answer(gridList->rangeQuery(x1,x2,y1,y2));
            sort(answer.begin(),answer.end(),[](Point &a,Point &b){
                return a.id<b.id;
            });
            cout<<"Range Query Results:-\n";
            for(Point p : answer){
                cout<<p.id<<' '<<p.x<<' '<<p.y<<'\n';
            }
            cout<<"Naive Approach Results:-\n";
            for(Point p:dataset){
                if (p.x > x1 and p.x <= x2 and p.y > y1 and p.y <= y2)
                    cout<<p.id<<' '<<p.x<<' '<<p.y<<'\n';
            }
        }
    }
}