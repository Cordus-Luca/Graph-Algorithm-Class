#include <bits/stdc++.h>
#include <limits>

using namespace std;

int infinit = std::numeric_limits<int>::max();

/*
/// FISIERE BFS
ifstream in("bfs.in");
ofstream out("bfs.out");

/// FISIERE DFS
ifstream in("dfs.in");
ofstream out("dfs.out");

/// FISIERE BICONEX
ifstream in("biconex.in");
ofstream out("biconex.out");

/// FISIERE CTC
ifstream in("ctc.in");
ofstream out("ctc.out");

/// FISIERE SORTARET
ifstream in("sortaret.in");
ofstream out("sortaret.out");

/// FISIERE CC
ifstream in("cc.in");
ofstream out("cc.out");

/// FISIERE DIJKSTRA
ifstream in("dijkstra.in");
ofstream out("dijkstra.out");
*/
/// FISIERE BELLMAN-FORD
ifstream in("bellmanford.in");
ofstream out("bellmanford.out");
/*
/// FISIERE ROY-FLOYD
ifstream in("royfloyd.in");
ofstream out("royfloyd.out");
*/
class Graf
{
    int nr_N, nr_M, S;
    vector<vector<int>> adiac, GT; /// GT = GRAF TRANSPUS(PENTRU CTC)

public:

    /// Constructori
    Graf() : nr_N(0), nr_M(0) {}
    Graf(int N, int M) : nr_N(N), nr_M(M) {}

    void citire_or();
    void citire_neor();
    void citire_or_CTC();
    void citire_or_costuri(vector<vector<pair<int,int>>>&);

    void BFS();

    void conexe();

    void nma_nivel(int, int, vector<vector<int>>&, vector<int>&, vector<int>&, stack<int>&, int&, vector<int>&);
    void Biconex();

    void CTC();
    void DFS_adiac_CTC(int, vector<int>&, vector<bool>&);
    void DFS_GT_CTC(int, vector<bool>&, vector<vector<int>>&, int&);

    void SortareT();

    void nma_nivel_CC(int, int, vector<int>&, vector<int>&, int&, vector<int>&);
    void CC();

    void Dijkstra();

    void BellmanFord();

    void RoyFloyd();

};

void Graf::citire_or()  /// CITIRE GRAF ORIENTAT
{
    in >> nr_N >> nr_M >> S;
    adiac = vector<vector<int>>(nr_N + 1);
    for (int i = 0; i < nr_M; i++)
    {
        int x, y;
        in >> x >> y;
        adiac[x].push_back(y);
    }
}

void Graf::citire_or_CTC()  /// CITIRE GRAF ORIENTAT PENTRU CTC
{
    in >> nr_N >> nr_M;
    adiac = GT = vector<vector<int>>(nr_N + 1);
    for (int i = 0; i < nr_M; i++)
    {
        int x, y;
        in >> x >> y;
        adiac[x].push_back(y);
        GT[y].push_back(x);
    }
}

void Graf::citire_neor()    /// CITIRE GRAF NEORIENTAT
{
    in >> nr_N >> nr_M;
    adiac = vector<vector<int>>(nr_N + 1);
    for (int i = 0; i < nr_M; i++)
    {
        int x, y;
        in >> x >> y;
        adiac[x].push_back(y);
        adiac[y].push_back(x);
    }
}

void Graf::citire_or_costuri(vector<vector<pair<int,int>>>& costuri)
{
    in >> nr_N >> nr_M;
    costuri = vector<vector<pair<int,int>>>(nr_N + 1);
    for(int i = 1; i <= nr_M; i++)
    {
        int x, y, c;
        in >> x >> y >> c;
        costuri[x].push_back({y,c}); /// Muchia de la x plecand in y are costul c
    }
}

void Graf::BFS() /// BFS
{
    queue <int> coada;
    int nod;
    int vizitat[100001] = {};
    vizitat[S] = 1;
    coada.push(S);
    int cost[100001] = {};
    cost[S] = 0;///Costul la nodul de start este 0

    while(coada.size() > 0) ///Cat timp inca mai am noduri in graf
    {
        nod = coada.front();

        for(int j = 0; j < adiac[nod].size(); j++)
        {
            if(vizitat[adiac[nod][j]] == 0)
            {
                coada.push(adiac[nod][j]);
                cost[adiac[nod][j]] = cost[nod] + 1;
                vizitat[adiac[nod][j]]++;
            }
        }
        coada.pop();
    }

    for(int i = 1; i <= nr_N; i++)
    {
        if(vizitat[i] == 0) out << "-1 ";
        else out << cost[i] << " ";
    }
}

void Graf::conexe() /// NUMARARE COMPONENTE CONEXE PENTRU DFS
{
    vector<int> stiva;
    vector<bool> vizitat;
    vizitat = vector<bool> (nr_N + 1, false);
    int nr=0;

    for(int i = 1; i <= nr_N; i++)
    {
        if(!vizitat[i])
        {
            DFS_adiac_CTC(i, stiva, vizitat);
            nr++;
        }
    }

    out << nr;
}

void Graf::nma_nivel(int k, int tata, vector<vector<int>>& componente_bi, vector<int>& nma, vector<int>& nivel, stack<int>& stiva, int& nr, vector<int>& V) /// BICONEX_DFS
{
    V[k] = 1;
    stiva.push(k);
    nivel[k] = nivel[tata] + 1;
    nma[k] = nivel[k];
    for(int i = 0; i < adiac[k].size(); i++)
    {
        int x = adiac[k][i];
        ///cout << "test ";
        if(x != tata)
        {
            if(V[x] == 1)
            {
                ///cout << k << " " << adiac[k][i] << " | ";
                if(nma[k] > nivel[x])
                    nma[k] = nma[x];
            }
            else
            {
                nma_nivel(adiac[k][i], k, componente_bi, nma, nivel, stiva, nr, V);
                if(nma[k] > nma[x])
                {
                    nma[k] = nma[adiac[k][i]];
                }
                if(nivel[k] <= nma[x])
                {
                    ///cout << k << " " << adiac[k][i] << " | ";
                    while (stiva.top() != x)
                    {
                        componente_bi[nr].push_back(stiva.top());
                        stiva.pop();
                    }
                    componente_bi[nr].push_back(x);
                    stiva.pop();
                    componente_bi[nr].push_back(k);
                    nr++;
                }
            }
        }
    }
}

void Graf::Biconex() /// BICONEX
{
    vector<vector<int>> componente_bi;
    componente_bi.resize(nr_N + 1);
    vector<int> nma(nr_N + 1), nivel(nr_N + 1), V(nr_N + 1);
    int nr = 0;
    stack<int> stiva;
    ///cout << "test";
    nma_nivel(1, 0, componente_bi, nma, nivel, stiva, nr, V);
    ///cout << "TEST";
    out << nr << '\n';
    for(int i = 0; i < nr; i++)
    {
        for(int j = 0; j < componente_bi[i].size(); j++)
        {
            out << componente_bi[i][j] << " ";
        }
        out << '\n';
    }
}

void Graf::DFS_adiac_CTC(int k, vector<int>& stiva_CTC, vector<bool>& V_CTC) /// Sortare Topologica
{
    V_CTC[k] = true;
    for(auto x : adiac[k])
        if(!V_CTC[x])
            DFS_adiac_CTC(x, stiva_CTC, V_CTC);
    stiva_CTC.push_back(k);
}

void Graf::DFS_GT_CTC(int k, vector<bool>& V_CTC, vector<vector<int>>& componente_tc, int& contor)
{
    V_CTC[k] = true;
    for(auto x : GT[k])
        if(!V_CTC[x])
            DFS_GT_CTC(x, V_CTC, componente_tc, contor);
    componente_tc[contor].push_back(k);
}

void Graf::CTC() /// CTC
{
    vector<vector<int>> componente_tc;
    componente_tc.resize(nr_N + 1);
    vector<int> stiva_CTC;
    vector<bool> V_CTC;
    int nr = 0;
    V_CTC = vector<bool> (nr_N + 1, false);
    for(int i = 1; i <= nr_N; i++)
    {
        if(!V_CTC[i])
            DFS_adiac_CTC(i, stiva_CTC, V_CTC);
    }
    V_CTC = vector<bool> (nr_N + 1, false);
    for(vector<int>::reverse_iterator it = stiva_CTC.rbegin() ; it != stiva_CTC.rend() ; it ++)
        if(!V_CTC[*it])
        {
            nr++;
            DFS_GT_CTC(*it, V_CTC, componente_tc, nr);
        }
    out << nr << '\n';
    for(int i = 0; i <= nr_N; i++)
    {
        if(componente_tc[i].size() >= 1)
        {
            for(int j = 0; j <componente_tc[i].size(); j++)
            {
                out << componente_tc[i][j] << " ";
            }
            out << '\n';
        }
    }
}

void Graf::SortareT() /// SORTARET
{
    vector<int> stiva_ST;
    vector<bool> V_ST;
    V_ST = vector<bool> (nr_N + 1, false);
    for(int i = 1; i <= nr_N; i++)
    {
        if(V_ST[i] == false)
            DFS_adiac_CTC(i, stiva_ST, V_ST);
    }

    for(int i = stiva_ST.size() - 1; i >= 0; i--)
        out << stiva_ST[i] << " ";
}

void Graf::nma_nivel_CC(int k, int tata, vector<int>& nma, vector<int>& nivel, int& nr, vector<int>& V) /// Critical connections
{

    V[k] = 1;
    nivel[k] = nivel[tata] + 1;
    nma[k] = nivel[k];
    for(int i = 0; i < adiac[k].size(); i++)
    {
        int x = adiac[k][i];
        ///cout << "test ";
        if(x != tata)
        {
            if(V[x] == 1)
            {
                ///cout << k << " " << adiac[k][i] << " | ";
                if(nma[k] > nivel[x])
                    nma[k] = nma[x];
            }
            else
            {
                nma_nivel_CC(adiac[k][i], k, nma, nivel, nr, V);
                if(nma[k] > nma[x])
                {
                    nma[k] = nma[x];
                }
                if(nivel[k] < nma[x])
                    out << "[" << k << ", " << x << "] ";
            }
        }
    }
}

void Graf::CC()
{
    vector<int> nma(nr_N + 1), nivel(nr_N + 1), V(nr_N + 1);
    int nr = 0;
    stack<int> stiva;
    nma_nivel_CC(1, 0, nma, nivel, nr, V);
}

void Graf::Dijkstra()
{
    vector<vector<pair<int,int>>> costuri;
    int V[100001] = {}, dist[100001] = {};
    citire_or_costuri(costuri);

    for(int i = 1; i <= nr_N; i++)
        dist[i] = infinit;

    dist[1]=0;

    priority_queue<pair<int,int>> H; /// Va sorta singur in functie de cea mai mica distanta
    H.push({0,1});

    while(H.size() > 0)
    {
        int u = H.top().second;
        H.pop();
        if(V[u])
            continue;
        else
            V[u]++;
        for(auto v : costuri[u]) /// Verificam toti vecinii lui u
        {
            if(dist[v.first] > dist[u] + v.second)
            {
                dist[v.first] = dist[u] + v.second;
                H.push({-dist[v.first], v.first}); /// Folosim -dist[v.first] ca sa avem in top cea mai mica valoare
            }
        }
    }

    for(int i = 2; i <= nr_N; i++)
    {
        if(dist[i] != infinit)
            out << dist[i] << " ";
        else
            out << "0 ";
    }
}

void Graf::BellmanFord()
{
    vector<vector<pair<int,int>>> costuri;
    int V[100001] = {}, dist[100001] = {}, ok = 0;
    citire_or_costuri(costuri);

    for(int i = 1; i <= nr_N; i++)
        dist[i] = infinit;

    dist[1]=0;

    priority_queue<pair<int,int>> H; /// Va sorta singur in functie de cea mai mica distanta
    H.push({0,1});

    while(H.size() > 0)
    {

        int u = H.top().second;
        H.pop();
        V[u]++;
        if (V[u] >= nr_N)               /// Daca ciclul da un numar negativ se va invartii in jurul acelui cilcu la infinit,
        {                               /// asa ca opresc cand numarul de vizitari este mai mare decat numarul de noduri
            ok = 1;
            out << "Ciclu negativ!";
            break;
        }
        else
            V[u]++;
        for(auto v : costuri[u]) /// Verificam toti vecinii lui u
        {
            if(dist[v.first] > dist[u] + v.second)
            {
                dist[v.first] = dist[u] + v.second;
                H.push({-dist[v.first], v.first}); /// Folosim -dist[v.first] ca sa avem in top cea mai mica valoare
            }
        }
    }

    if(ok == 0)
    {
        for(int i = 2; i <= nr_N; i++)
        {
            if(dist[i] != infinit)
                out << dist[i] << " ";
        }
    }
}

void Graf::RoyFloyd()
{
    int matr[101][101] = {}, N, i, j, k, x;

    in >> N;

    for (i = 1; i <= N; i++)
    {
        for (j = 1; j <= N; j++)
        {
            in >> x;
            if (x || (i == j))
                matr[i][j] = x;
            else
                matr[i][j] = infinit;
        }
    }

    for (k = 1; k <= N; k++)
    {
        for (i = 1; i <= N; i++)
        {
            for (j = 1; j <= N; j++)
            {
                if(matr[i][j] >= matr[i][k] + matr[k][j])
                    matr[i][j] = matr[i][k] + matr[k][j];
            }
        }
    }

    for (i = 1; i <= N; i++)
    {
        for (j = 1; j <= N; j++)
        {
            if(matr[i][j] == infinit)
                out << "0 ";
            else
                out << matr[i][j] << " ";
        }
        out << '\n';
    }
}

int main()
{
    Graf G;
    /*
    ///BFS
    G.citire_or();
    G.BFS();

    ///DFS
    G.citire_neor();
    G.conexe();

    ///Biconex
    G.citire_neor();
    G.Biconex();

    ///CTC
    G.citire_or_CTC();
    G.CTC();

    ///SortareT
    G.citire_or_CTC();
    G.SortareT();

    ///CCIN
    G.citire_neor();
    G.CC();

    /// Dijkstra
    G.Dijkstra();
    */
    /// Bellman-Ford
    G.BellmanFord();
    /*
    /// Roy-Floyd
    G.RoyFloyd();
    */

    return 0;
}
