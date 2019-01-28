#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <functional>
#include <fstream>
#include <string>

using namespace std;
string dirPath = "";

int main(void)
{
    priority_queue< pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> > > >  pq;
    vector<int> st(2,0); //start
    vector<pair<int,int> > dtn; //destination(exit)
    vector<vector<int> > input_maze;
    vector<pair<pair<int,int>, pair<int, int> > > trace;
    
    //save the direction of the path in divert array.
    //when end position is found, it traces back using that array.
    vector<vector<pair<int, int> > > divert; 

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
        vector<pair<int, int> > tmp_divert;

        for( int j = 0; j < w; j++ )
        {
            int input;
            ifile >> input;
            
            if( input == 3 )
            {
                st[0]=j; //start point (width)
                st[1]=i; //start point (height)
            }

            if( input == 4 )
            {
                dtn.push_back( make_pair(j,i) );                
            }
            
            tmp_divert.push_back(make_pair(0,0));
            tmp_visit.push_back(0);
            tmp.push_back(input);
        }

        divert.push_back(tmp_divert);
        input_maze.push_back(tmp);
        visit_.push_back(tmp_visit);
    }
    int time=0;
    int x, y;
    x = st[0]; // start point (width)
    y = st[1]; // start point (height)
    
    
    while(1)
    {
        visit_[y][x] == 1; //check visited.

        input_maze[y][x] = 0;
        if( x==st[0] && y == st[1] )
        {
            input_maze[y][x] = 3;
        }
        int dx[4] = {0, -1, 0, 1}; //down, left, up, right
        int dy[4] = {1, 0, -1, 0};
        int flag = 0;

        for(int i=0; i<4; i++)
        {
            int tmp_x = 0, tmp_y = 0;
            tmp_x = x + dx[i];
            tmp_y = y + dy[i];

            if( tmp_x < 0 || tmp_x >= w || tmp_y < 0 || tmp_y >= h ) continue;

            if( visit_[tmp_y][tmp_x] != 0 ) continue;


            if( input_maze[tmp_y][tmp_x] == 4 )
            {
                time++;
                flag = 1;
                trace.push_back(make_pair(make_pair(x,y),make_pair(dx[i],dy[i]))); // save coordinates when finding the exit;

                break;
            }

            if( input_maze[tmp_y][tmp_x] == 2 )
            {
                int hv;

                // Heuristic function.
                // hv means Heuristic value.
                for( int k = 0; k < dtn.size(); k++)
                {

                    hv = 1000000;
                    int tmp_hv = 0;

                    if( dtn[k].first - tmp_x < 0 )
                        tmp_hv -= (dtn[k].first - tmp_x);
                    else
                        tmp_hv += dtn[k].first - tmp_x;


                    if( dtn[k].second - tmp_y < 0 )
                        tmp_hv -= (dtn[k].second - tmp_y);
                    else
                        tmp_hv += dtn[k].second - tmp_y;


                    if( hv > tmp_hv )
                    {
                        hv = tmp_hv;
                    }

                }

                pq.push( make_pair( hv, make_pair( tmp_x, tmp_y ) ) ); // store hv, width, height value. 

                divert[tmp_y][tmp_x].first = dx[i]; // save the moving direction (up, down, left, right)
                divert[tmp_y][tmp_x].second = dy[i]; // save the moving direction (up, down, left, right)

            }


        }

        if( flag != 0 ) break;

        pair< int, pair<int, int> > tmp_q;
        tmp_q = pq.top();
        pq.pop();
        x = tmp_q.second.first;
        y = tmp_q.second.second;
        
        time++;
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
        
        //traces back using divert array.
        dx = divert[fy][fx].first; 
        dy = divert[fy][fx].second;
        fy = fy-dy;
        fx = fx-dx;

    }

    dirPath += "/output.txt";

    ofstream ofile(dirPath.c_str(), fstream::out);

    // print result 
    for( int i=0; i<h; i++ )
    {
        for( int j=0; j<w; j++ )
        {
            if(input_maze[i][j] ==0)
                input_maze[i][j] = 2;

            cout << input_maze[i][j] << " ";
            ofile << input_maze[i][j] << " ";
        }
        cout << endl;
        ofile << endl;
    }

    cout << "---" << endl;
    cout << "length=" << l << endl;
    cout << "time=" << time << endl;

    ofile << "---" << endl;
    ofile << "length=" << l << endl;
    ofile << "time=" << time << endl;

    return 0;
}
