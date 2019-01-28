#include <iostream>
#include <stack>
#include <vector>
#include <utility>
#include <functional>
#include <fstream>
#include <string>

using namespace std;
string dirPath = "";

int main(void)
{
    vector<int> st(2,0); //start
    vector<pair<int,int> > dtn; //destination(exit)
    vector<vector<int> > input_maze;
    vector<pair< pair<int,int>, pair<int,int> > > trace;
    vector<vector<int> > dist; //distance
    stack<pair<int,int>  > stack_;
    vector<vector<pair<int,int> > > divert;

    string dirName;

    cout << "File Directory : ";
    cin >> dirName;

    dirPath = dirName;
    dirName += "/input.txt";

    ifstream ifile(dirName, fstream::in);

    int w, h;
    ifile >> h >> w;
    vector<vector<int> > visit_;

    for( int i = 0; i < h; i++ )
    {
        vector<int> tmp;
        vector<int> tmp_visit;
        vector<int> tmp_dist;
        vector<pair<int, int> > tmp_divert;

        for( int j = 0; j < w; j++ )
        {
            int input;
            ifile >> input;
            
            if( input == 3 )
            {
                st[0]=j; //start point width
                st[1]=i; //start point height
            }

            if( input == 4 )
            {
                dtn.push_back( make_pair(j,i) );                
            }
            
            tmp_visit.push_back(0);
            tmp_dist.push_back(0);
            tmp_divert.push_back(make_pair(0,0));
            tmp.push_back(input);
        }

        input_maze.push_back(tmp);
        visit_.push_back(tmp_visit);
        dist.push_back(tmp_dist);
        divert.push_back(tmp_divert);
    }
    
    int time=0;
    int x, y;
    int count=0;
    int flag;
    
    for(;;)
    {
        x = st[0]; // start point width
        y = st[1]; // start point height
        dist[y][x] == 0; // set 0 as the distance of the start point. 
        trace.clear();
        for(int i=0; i<h; i++)
        {
            
            for(int j=0; j<w; j++)
            {
                if(input_maze[i][j] == 0)
                    input_maze[i][j] = 2;
                visit_[i][j] = 0;
                dist[i][j] = 0;
            }
        }

        int exit_ = 0;
       // count = 0;
        flag = 0; 
        while(1)
        { 

           visit_[y][x] == 1; //check visited.


            int dx[4] = {0, -1, 0, 1}; //down, left, up, right
            int dy[4] = {1, 0, -1, 0};

            for(int i=0; i<4; i++)
            {
                int tmp_x = 0, tmp_y = 0;
                tmp_x = x + dx[i];
                tmp_y = y + dy[i];

                if( tmp_x < 0 || tmp_x >= w || tmp_y < 0 || tmp_y >= h ) continue;


                if( input_maze[tmp_y][tmp_x] == 4 )
                {
                    time++;
                    flag = 1;
                    trace.push_back(make_pair(make_pair(x,y),make_pair(dx[i],dy[i]))); //save coordinates when finding the exit.

                    break;
                }
                
                if( visit_[tmp_y][tmp_x] != 0 ) continue;
                if( dist[tmp_y][tmp_x] <= dist[y][x]+1 && dist[tmp_y][tmp_x] != 0 ) continue;

                if( input_maze[tmp_y][tmp_x] != 1 && dist[y][x] != count )
                {

                    if(dist[tmp_y][tmp_x] == 0 ){
                        stack_.push(make_pair(tmp_x, tmp_y));
                        dist[tmp_y][tmp_x]  = dist[y][x] + 1; //store distance.
                        divert[tmp_y][tmp_x].first = dx[i]; //save the moving direction (up, down, left, right)
                        divert[tmp_y][tmp_x].second = dy[i]; //save the moving direction (up, down, left, right)
                    }
                    else
                    {
                        if(dist[tmp_y][tmp_x] > dist[y][x]+1) 
                        {
                            stack_.push(make_pair(tmp_x,tmp_y));
                            dist[tmp_y][tmp_x] = dist[y][x] + 1; //store distance.
                            divert[tmp_y][tmp_x].first = dx[i]; //save the moving direction (up,down, left, right)
                            divert[tmp_y][tmp_x].second = dy[i]; //save the moving direction (up, down, left, right)
                        }

                    }
                }
            }

            if( flag != 0 )
            {
                exit_ = 1;
                break;
            }

            pair< int, int> tmp_q;

            if( stack_.empty() == true )
            {
                break;
            }

            tmp_q = stack_.top();
            stack_.pop();
            x = tmp_q.first;
            y = tmp_q.second;

            time++;


        }

        count++; 
        while(!stack_.empty())
            stack_.pop();
        if( exit_ != 0 ) break;

    }


    int fx, fy, l=0;
    fx = trace[0].first.first;
    fy = trace[0].first.second;
    while(1)
    {
        if( input_maze[fy][fx] == 3 ) break;
        else l++;
        input_maze[fy][fx] = 5;
        int dx, dy;
        dx = divert[fy][fx].first;
        dy = divert[fy][fx].second;
        fy = fy-dy; //
        fx = fx-dx;        
    }

    dirPath += "/output.txt";

    ofstream ofile(dirPath.c_str(), fstream::out);

    // print result 
    for( int i=0; i<h; i++ )
    {
        for( int j=0; j<w; j++ )
        {
            cout << input_maze[i][j] << " ";
            ofile << input_maze[i][j] << " ";
        }
        cout << endl;
        ofile << endl;
    }

    cout << "---" << endl;
    cout << "length=" << l  << endl;
    cout << "time=" << time << endl;
    ofile << "---" << endl;
    ofile << "length=" << l  << endl;
    ofile << "time=" << time << endl;
    
    return 0;
}
