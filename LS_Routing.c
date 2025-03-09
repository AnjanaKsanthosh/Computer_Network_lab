#include <stdio.h>
#include <limits.h>

#define MAX 100 

void dijkstra(int cost[MAX][MAX], int n, int src) {
    int dist[MAX], visited[MAX], prev[MAX];

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        prev[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int minDist = INT_MAX, u = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && cost[u][v] && dist[u] + cost[u][v] < dist[v]) {
                dist[v] = dist[u] + cost[u][v];
                prev[v] = u;
            }
        }
    }

    printf("\nRouter %d Shortest Paths:\n", src);
    for (int i = 0; i < n; i++) {
        if (i != src) {
            printf("To %d -> Cost: %d | Path: ", i, dist[i]);
            int path[MAX], count = 0, temp = i;
            while (temp != -1) {
                path[count++] = temp;
                temp = prev[temp];
            }
            for (int j = count - 1; j >= 0; j--) {
                printf("%d ", path[j]);
            }
            printf("\n");
        }
    }
}

int main() {
    int n, src, cost[MAX][MAX];

    printf("Enter number of routers: ");
    scanf("%d", &n);

    printf("Enter cost matrix (use 0 for same node, 999 for no direct link):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            if (i != j && cost[i][j] == 0) cost[i][j] = 999;
        }
    }

    printf("Enter source router (0 to %d): ", n - 1);
    scanf("%d", &src);

    dijkstra(cost, n, src);

    return 0;
}

