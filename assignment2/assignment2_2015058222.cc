#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <cmath>
#include <queue>
#include <set>

using namespace std;

bool cos_check[338][338];
bool eucli_check[338][338];

int main(void)
{
    string dirName;
    dirName="WordEmbedding.txt";
    ifstream ifile(dirName.c_str(),fstream::in);
    vector<pair<string, vector<string>>> vec;
    vector<pair<double, vector<double>>> vec_to_double;
    double cos_simil[338][338] = {0};
    double eucli_simil[338][338] = {0};
    priority_queue<pair<double, pair<int,int>>> cos_pq;
    priority_queue<pair<double, pair<int,int>>> eucli_pq;
    vector<vector<int>> cos_cluster_set;
    vector<vector<int>> eucli_cluster_set;
    vector<vector<vector<double>>> cos_cluster_word(338);
    vector<vector<vector<double>>> eucli_cluster_word(338);

    for(int i=0; i<380; i++)
    {
        string word_name;
        ifile >> word_name;
        string dim;
        ifile >> dim;
        vector<string> tmp;
        char comma=',';
        stringstream f(dim);
        string s;
        while(getline(f,s,comma))
        {
            tmp.push_back(s);
        }
        vec.push_back(make_pair(word_name,tmp));
    }



    for(int i=0; i<338; i++)
    {
        double squared=0;
        vector<double> vd;
        vector<string> tmp;
        tmp = vec[i].second;
        for(int j=0; j<300; j++)
        {
            double d;
            string stmp = tmp[j];
            
            if(stmp.size()>4 && stmp[stmp.size()-4]=='e')
            {
                d = stod(stmp.substr(0,stmp.size()-4)) * pow(10, stod(stmp.substr(stmp.size()-3))); 
            }
            else
            {
                d = stod(stmp);
            }
            
            squared += pow(d,2);
            vd.push_back(d);
        }

        vec_to_double.push_back(make_pair(squared, vd));

    }


    //similarity 계산
    double max = 0;
    for(int i=0; i<338; i++)
    {
        for(int j=i+1; j<338; j++)
        {
            double inner = 0;
            double distance = 0;

            for(int k=0; k<300; k++)
            {
                double x,y;
                x = vec_to_double[i].second[k];
                y = vec_to_double[j].second[k];
                inner += x * y;
                distance += (x-y)*(x-y);
            }

            cos_simil[i][j] = ((inner/(sqrt(vec_to_double[i].first) * sqrt(vec_to_double[j].first)))+1)/2;
            cos_pq.push(make_pair(cos_simil[i][j],make_pair(i,j)));

            eucli_simil[i][j] = 1 - sqrt(distance)/sqrt(300);
            eucli_pq.push(make_pair(eucli_simil[i][j],make_pair(i,j)));
        }
    }
    

    

    vector<int> tmp;
    //clustering
    for(int i=0; i<338; i++)
    {
        cos_cluster_word[i].resize(2);
        eucli_cluster_word[i].resize(2);
        cos_cluster_word[i][0].push_back(i);
        cos_cluster_word[i][1].push_back(1);
        eucli_cluster_word[i][0].push_back(i);
        eucli_cluster_word[i][1].push_back(1);
        
        tmp.push_back(i);
        cos_cluster_set.push_back(tmp);
        eucli_cluster_set.push_back(tmp);
        tmp.clear();
    }


    // #1. Use the cosine simility
    while(!cos_pq.empty())
    {
        pair<double, pair<int,int>> q_tmp = cos_pq.top();
        cos_pq.pop();
        int x = q_tmp.second.first;
        int y = q_tmp.second.second;
        

        cos_check[x][y]=true;
        cos_check[y][x]=true;

        vector<double>::iterator it_x = cos_cluster_word[x][0].end()-1;
        vector<double>::iterator it_y = cos_cluster_word[y][0].end()-1;

        int tmp_x = cos_cluster_word[x][0].back();
        int tmp_y = cos_cluster_word[y][0].back();
        bool flag = true;
    

        vector<int> vec_x = cos_cluster_set[tmp_x];
        vector<int> vec_y = cos_cluster_set[tmp_y];
        vector<int> vec_tmp;

        for(int i=0; i<vec_x.size(); i++)
        {

            for(int j=0; j<vec_y.size(); j++)
            {

                if(vec_x[i] != vec_y[j] && cos_check[vec_x[i]][vec_y[j]]==false)
                {
                    flag=false;
                    break;
                }
            }
        }

        vec_tmp.reserve(vec_x.size() + vec_y.size());
        vec_tmp.insert(vec_tmp.end(), vec_x.begin(), vec_x.end());
        vec_tmp.insert(vec_tmp.end(), vec_y.begin(), vec_y.end());
        
        if(flag)
        {
            cos_cluster_set.push_back(vec_tmp);

            for(int i=0; i<vec_tmp.size(); i++)
            {
                cos_cluster_word[vec_tmp[i]][0].push_back(cos_cluster_set.size()-1);
                cos_cluster_word[vec_tmp[i]][1].push_back(q_tmp.first);
            }
        }
        
    }


    // #2. Use the euclidean simility
    while(!eucli_pq.empty())
    {
        pair<double, pair<int,int>> q_tmp = eucli_pq.top();
        eucli_pq.pop();
        int x = q_tmp.second.first;
        int y = q_tmp.second.second;
        

        eucli_check[x][y]=true;
        eucli_check[y][x]=true;

        vector<double>::iterator it_x = eucli_cluster_word[x][0].end()-1;
        vector<double>::iterator it_y = eucli_cluster_word[y][0].end()-1;

        int tmp_x = eucli_cluster_word[x][0].back();
        int tmp_y = eucli_cluster_word[y][0].back();
        bool flag = true;
    
        vector<int> vec_x = eucli_cluster_set[tmp_x];
        vector<int> vec_y = eucli_cluster_set[tmp_y];
        vector<int> vec_tmp;

        for(int i=0; i<vec_x.size(); i++)
        {

            for(int j=0; j<vec_y.size(); j++)
            {

                if(vec_x[i] != vec_y[j] && eucli_check[vec_x[i]][vec_y[j]]==false)
                {
                    flag=false;
                    break;
                }
            }
        }
        vec_tmp.reserve(vec_x.size() + vec_y.size());
        vec_tmp.insert(vec_tmp.end(), vec_x.begin(), vec_x.end());
        vec_tmp.insert(vec_tmp.end(), vec_y.begin(), vec_y.end());
        
        if(flag)
        {
            eucli_cluster_set.push_back(vec_tmp);

            for(int i=0; i<vec_tmp.size(); i++)
            {
                eucli_cluster_word[vec_tmp[i]][0].push_back(eucli_cluster_set.size()-1);
                eucli_cluster_word[vec_tmp[i]][1].push_back(q_tmp.first);
            }
        }
        
    }


    //cluster 분할 #1.cosine simility & threshold = 0.2, 0.4, 0.6, 0.8

    
    int word_clustering[4][338]; //각word가 속해있는 cluster number.
 
    for(int count_=1; count_<=4; count_++)
    {

        int cluster_num=1;
        //cout << "\ncosine simility & threshold = "<< 0.2 * count_ <<"\n";
        set<vector<int>> set_;
        set<vector<int>>::iterator it_set;

        bool flag = false;

        for(int i=0; i<338; i++)
        {
            for(int j=0; j<cos_cluster_word[i][1].size(); j++)
            {
               // cout << cos_cluster_word[i][1][j] << " ";
                if(cos_cluster_word[i][1][j] <= 0.2 * count_)
                {
                    set_.insert(cos_cluster_set[cos_cluster_word[i][0][j-1]]);
                    flag = true;
                    break;
                }
            }
        }

        //cout << "\n";
        if(!flag)
        {
            set_.insert(cos_cluster_set[cos_cluster_word[337][0][cos_cluster_word[337][0].size()-1]]);
        }
        
        for(it_set=set_.begin(); it_set != set_.end(); it_set++)
        {
            for(int j=0; j<(*it_set).size(); j++)
            {
        //      cout << (*it_set)[j] << " "; //단어 인덱스 값  
                word_clustering[count_-1][(*it_set)[j]] = cluster_num;
            }
            cluster_num++;
            //cout <<"\n";
        }
/*
        //Word Cluster Number
        //
        for(int n=0; n<338; n++)
        {
            cout << vec[n].first <<" => " << word_clustering[n] <<"\n";
        }
*/
    }

    string output = "WordClustering.txt";
    ofstream ofile(output.c_str(),fstream::out);

    for(int i=0; i<338; i++)
    {
        ofile << vec[i].first <<"\n";
        for(int j=0; j<vec[i].second.size(); j++)
        {
            ofile << (vec[i].second)[j] <<",";
        }
        ofile <<"\n"<< word_clustering[3][i] <<"\n";
    }

    
    
    int word_clustering_eucli[4][338]; //각word가 속해있는 cluster number.
    //cluster 분할 #2.euclidean simility & threshold = 0.2, 0.4, 0.6, 0.8
    for(int count_=1; count_<=4; count_++)
    {
        int cluster_num=1;
    //    cout << "\n#1-"<< count_ <<".euclidean simility & threshold = "<< 0.2 * count_ <<"\n";
        set<vector<int>> set_;
        set<vector<int>>::iterator it_set;

        bool flag = false;

        for(int i=0; i<338; i++)
        {
            for(int j=0; j<eucli_cluster_word[i][1].size(); j++)
            {
                // cout << cos_cluster_word[i][1][j] << " ";
                if(eucli_cluster_word[i][1][j] <= 0.2 * count_)
                {
                    set_.insert(eucli_cluster_set[eucli_cluster_word[i][0][j-1]]);
                    flag = true;
                    break;
                }
            }
        }

      //  cout << "\n";
        if(!flag)
        {
            set_.insert(eucli_cluster_set[eucli_cluster_word[337][0][eucli_cluster_word[337][0].size()-1]]);
        }


        for(it_set=set_.begin(); it_set != set_.end(); it_set++)
        {
            for(int j=0; j<(*it_set).size(); j++)
            {
        //        cout << (*it_set)[j] << " ";
                word_clustering_eucli[count_-1][(*it_set)[j]] = cluster_num;
            }
            cluster_num++;
          //  cout <<"\n";
        }
/*
        //Word Cluster Number
        //
        for(int n=0; n<338; n++)
        {
            cout << vec[n].first <<" => " << word_clustering[n] <<"\n";
        }
*/
    }


    
    // class 분류
    
    string topicFile;
    topicFile="WordTopic.txt";
    ifstream tifile(topicFile.c_str(),fstream::in);

    vector<int> Topics;
    vector<vector<int>> topicCount(339);
    for(int i = 1; i <= 338; i++)
    {
        topicCount[i].resize(8, 0);
    }

    int topicNum = -1;
    for(int i=0; i<338; i++)
    {
        string class_name;
        tifile >> class_name;
        //cout << class_name << " ";
        if(class_name[0] == '[')
        {
            while(class_name[class_name.size()-1] != ']')
            {
                tifile >> class_name;
            }
            topicNum++;
            tifile >> class_name;
        }

        Topics.push_back(topicNum);
    }

    //entropy 계산
    cout<< "*****entropy 계산 결과값 출력*****\n";
    cout<< "=== 1. Cosine similarity ===\n";

    for(int i = 0; i < 4; i++)
    {
        cout << "Threshold : "<<0.2 * (i+1) << "인 경우\t";
        for(int k = 1; k <= 338; k++)
        {
            for(int l = 0; l < 8; l++)
            {
                topicCount[k][l] = 0;
            }
        }
        for(int j = 1; j <= 338; j++)
        {
            topicCount[word_clustering[i][j-1]][Topics[j-1]]++;
        }
        double entropy = 0;
        for(int k = 1; k <= 338; k++)
        {
            int total_count = 0;
            double tmp_entropy = 0;
            for(int j = 0; j < 8; j++)
            {
                total_count += topicCount[k][j];
            }
            if(total_count == 0)
                continue;
            for(int j = 0; j < 8; j++)
            {
                if(topicCount[k][j] != 0)
                    tmp_entropy += -((double)topicCount[k][j]/(double)total_count) * (log((double)topicCount[k][j]/(double)total_count));
            }
            entropy += (double)total_count / (double)338 * tmp_entropy;
        }
        cout << entropy << "\n";
    }

    cout<< "\n===2. Euclidean similarity ===\n";

    for(int i = 0; i < 4; i++)
    {
        cout << "Threshold : "<<0.2 * (i+1) << "인 경우\t";
        for(int k = 1; k <= 338; k++)
        {
            for(int l = 0; l < 8; l++)
            {
                topicCount[k][l] = 0;
            }
        }
        for(int j = 1; j <= 338; j++)
        {
            topicCount[word_clustering_eucli[i][j-1]][Topics[j-1]]++;
        }
        double entropy = 0;
        for(int k = 1; k <= 338; k++)
        {
            int total_count = 0;
            double tmp_entropy = 0;
            for(int j = 0; j < 8; j++)
            {
                total_count += topicCount[k][j];
            }
            if(total_count == 0)
                continue;
            for(int j = 0; j < 8; j++)
            {
                if(topicCount[k][j] != 0)
                    tmp_entropy += -((double)topicCount[k][j]/(double)total_count) * (log((double)topicCount[k][j]/(double)total_count));
            }
            entropy += (double)total_count / (double)338 * tmp_entropy;
        }
        cout << entropy << "\n";
    }



    return 0;
}
