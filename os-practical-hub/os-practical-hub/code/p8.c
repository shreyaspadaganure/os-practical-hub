/* Practical 8 : Disk Scheduling - SSTF, SCAN, C-LOOK
   Initial head movement: AWAY from the spindle (towards higher cylinder numbers) */
#include <stdio.h>
#include <stdlib.h>
#define MAX 50

int n, head, size, req[MAX];

void sorted_copy(int a[]) {
    for (int i = 0; i < n; i++) a[i] = req[i];
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) { int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t; }
}

void move_to(int *cur, int *total, int to) {
    *total += abs(to - *cur);
    *cur = to;
    printf(" -> %d", to);
}

void sstf(void) {
    int visited[MAX] = {0}, cur = head, total = 0;
    printf("\n========== SSTF ==========\nSeek sequence: %d", cur);
    for (int c = 0; c < n; c++) {
        int best = -1, bd = 1 << 30;
        for (int i = 0; i < n; i++)
            if (!visited[i] && abs(req[i] - cur) < bd) { bd = abs(req[i] - cur); best = i; }
        visited[best] = 1;
        move_to(&cur, &total, req[best]);
    }
    printf("\nTotal head movement = %d cylinders\n", total);
}

void scan(void) {
    int a[MAX], cur = head, total = 0, i = 0;
    sorted_copy(a);
    while (i < n && a[i] < head) i++;
    printf("\n========== SCAN ==========\nSeek sequence: %d", cur);
    for (int j = i; j < n; j++) move_to(&cur, &total, a[j]);          /* move away from spindle */
    if (i > 0) {
        if (cur != size - 1) move_to(&cur, &total, size - 1);         /* go to the last cylinder */
        for (int j = i - 1; j >= 0; j--) move_to(&cur, &total, a[j]); /* reverse direction */
    }
    printf("\nTotal head movement = %d cylinders\n", total);
}

void clook(void) {
    int a[MAX], cur = head, total = 0, i = 0;
    sorted_copy(a);
    while (i < n && a[i] < head) i++;
    printf("\n========== C-LOOK ==========\nSeek sequence: %d", cur);
    for (int j = i; j < n; j++) move_to(&cur, &total, a[j]);          /* up to the largest request */
    if (i > 0) {
        move_to(&cur, &total, a[0]);                                  /* jump to the smallest request */
        for (int j = 1; j < i; j++) move_to(&cur, &total, a[j]);
    }
    printf("\nTotal head movement = %d cylinders\n", total);
}

int main(void) {
    printf("Enter number of requests: ");
    scanf("%d", &n);
    printf("Enter the request queue: ");
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter total number of cylinders: ");
    scanf("%d", &size);
    printf("\nHead initially moving away from the spindle (towards higher cylinders)\n");
    sstf();
    scan();
    clook();
    return 0;
}
