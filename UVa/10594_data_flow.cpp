/**
 * Author: Ertsiger
 * 18/11/2016
 * Time: 0.100
 */
/* From Stanford University's notebook.
 * To perform minimum weighted bipartite matching:
 *  - Capacity between nodes = 1 (cost whatever given by the problem)
 *  - Capacity from source = 1 and cost = 0
 *  - Capacity to sink = 1 and cost = 0
 * Output: <maximum flow value - minimum cost value>
 * Complexity: O(|V|^2) per augmentation
 *             max flow: O(|V|^3) augmentations
 *             min cost max flow: O(|V|^4 * MAX_EDGE_COST) augmentations
 */
#include <iostream>
#include <vector>
#include <utility>
#include <limits>

using namespace std;
 
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long L;
typedef vector<L> VL;
typedef vector<VL> VVL;
typedef pair<int, int> PII;
typedef vector<PII> VPII;

const L INF = numeric_limits<L>::max() / 4;

struct MinCostMaxFlow {
    int N;
    VVL cap, flow, cost;
    VI found;
    VL dist, pi, width;
    VPII dad;

    MinCostMaxFlow(int N) :
        N(N), cap(N, VL(N)), flow(N, VL(N)), cost(N, VL(N)),
        found(N), dist(N), pi(N), width(N), dad(N) {}

    void AddEdge(int from, int to, L cap, L cost) {
        this->cap[from][to] = cap;
        this->cost[from][to] = cost;
    }

    void Relax(int s, int k, L cap, L cost, int dir) {
        L val = dist[s] + pi[s] - pi[k] + cost;
        if (cap && val < dist[k]) {
            dist[k] = val;
            dad[k] = make_pair(s, dir);
            width[k] = min(cap, width[s]);
        }
    }

    L Dijkstra(int s, int t) {
        fill(found.begin(), found.end(), false);
        fill(dist.begin(), dist.end(), INF);
        fill(width.begin(), width.end(), 0);
        dist[s] = 0;
        width[s] = INF;

        while (s != -1) {
            int best = -1;
            found[s] = true;
            for (int k = 0; k < N; k++) {
                if (found[k]) continue;
                Relax(s, k, cap[s][k] - flow[s][k], cost[s][k], 1);
                Relax(s, k, flow[k][s], -cost[k][s], -1);
                if (best == -1 || dist[k] < dist[best]) best = k;
            }
            s = best;
        }

        for (int k = 0; k < N; k++)
            pi[k] = min(pi[k] + dist[k], INF);
        return width[t];
    }

    pair<L, L> GetMaxFlow(int s, int t) {
        L totflow = 0, totcost = 0;
        while (L amt = Dijkstra(s, t)) {
            totflow += amt;
            for (int x = t; x != s; x = dad[x].first) {
                if (dad[x].second == 1) {
                    flow[dad[x].first][x] += amt;
                    totcost += amt * cost[dad[x].first][x];
                }
                else {
                    flow[x][dad[x].first] -= amt;
                    totcost -= amt * cost[x][dad[x].first];
                }
            }
        }
        return make_pair(totflow, totcost);
    }
};

int main()
{
	int n, m;
	while (cin >> n >> m)
	{
		vector< pair< pair<L, L>, L> > edges(m);
	
		for (int i = 0; i < m; ++i)
		{
			cin >> edges[i].first.first >> edges[i].first.second >> edges[i].second;
		}
		
		L d, k;
		cin >> d >> k;
		
		MinCostMaxFlow prob(n + 2);
		
		for (int i = 0; i < edges.size(); ++i)
		{
			prob.AddEdge(edges[i].first.first, edges[i].first.second, k, edges[i].second);
			prob.AddEdge(edges[i].first.second, edges[i].first.first, k, edges[i].second);
		}
		
		prob.AddEdge(0, 1, d, 0);
		prob.AddEdge(n, n + 1, d, 0);
		
		pair<L, L> res = prob.GetMaxFlow(0, n+1);
		
		if (res.first != d) // cant send all data
		{
			cout << "Impossible.\n";
		}
		else
		{
			cout << res.second << "\n";
		}
	}

	return 0;
}


