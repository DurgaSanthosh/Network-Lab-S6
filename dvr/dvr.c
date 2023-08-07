#include <stdio.h>
#include <stdbool.h>

#define MAX_ROUTERS 10
#define INFINITY 9999

int distanceMatrix[MAX_ROUTERS][MAX_ROUTERS];
int numRouters;

void initializeMatrix() {
    for (int i = 0; i < MAX_ROUTERS; i++) {
        for (int j = 0; j < MAX_ROUTERS; j++) {
            if (i == j)
                distanceMatrix[i][j] = 0;
            else
                distanceMatrix[i][j] = INFINITY;
        }
    }
}

void addLink(int router1, int router2, int distance) {
    distanceMatrix[router1][router2] = distance;
    distanceMatrix[router2][router1] = distance;
}

void printDistanceMatrix() {
    printf("Distance Matrix:\n");
    for (int i = 0; i < numRouters; i++) {
        for (int j = 0; j < numRouters; j++) {
            if (distanceMatrix[i][j] == INFINITY)
                printf("INF\t");
            else
                printf("%d\t", distanceMatrix[i][j]);
        }
        printf("\n");
    }
}

void distanceVectorRouting() {
    int distanceVector[MAX_ROUTERS][MAX_ROUTERS];

    for (int i = 0; i < numRouters; i++) {
        for (int j = 0; j < numRouters; j++) {
            distanceVector[i][j] = distanceMatrix[i][j];
        }
    }

    for (int k = 0; k < numRouters; k++) {
        for (int i = 0; i < numRouters; i++) {
            for (int j = 0; j < numRouters; j++) {
                if (distanceVector[i][j] > distanceVector[i][k] + distanceVector[k][j]) {
                    distanceVector[i][j] = distanceVector[i][k] + distanceVector[k][j];
                }
            }
        }
    }

    printf("\nUpdated Distance Vector:\n");
    for (int i = 0; i < numRouters; i++) {
        for (int j = 0; j < numRouters; j++) {
            if (distanceVector[i][j] == INFINITY)
                printf("INF\t");
            else
                printf("%d\t", distanceVector[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int numLinks, router1, router2, distance;

    printf("Enter the number of routers: ");
    scanf("%d", &numRouters);

    initializeMatrix();

    printf("Enter the number of links: ");
    scanf("%d", &numLinks);

    printf("Enter router1, router2, distance for each link:\n");
    for (int i = 0; i < numLinks; i++) {
        scanf("%d %d %d", &router1, &router2, &distance);
        addLink(router1, router2, distance);
    }

    printDistanceMatrix();
    distanceVectorRouting();

    return 0;
}

