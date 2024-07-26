struct Point {
    double x;
    double y;
};

void swap(int *, int *);

int main(int argc, char *argv[]) {
    int n;
    printf("How many points?\n");
    scanf("%d", &n);
    struct Point points[n];
    int i = 0;
    while (i < n) {
        scanf("%d%d", &points[i].x, &points[i].y);
    }
    for (i = 0; i < n; ++i) {
        printf("(%d, %d)\n", points[i].x, points[i].y);
    }
    return 0;
}

void swap(int *x, int *y) {
    *x ^= *y;
    *y ^= *x;
    *x ^= *y;
    return;
}