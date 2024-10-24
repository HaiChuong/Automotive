#include <stdio.h>
#include <time.h>

int main() {
    // Luu th?i di?m b?t d?u
    clock_t start_time = clock();
    int i;

    // Ðo?n mã c?a chuong trình
    for (i = 0; i < 2000000; ++i) {
        // Th?c hi?n m?t s? công vi?c b?t k?
    }

    // Luu th?i di?m k?t thúc
    clock_t end_time = clock();

    // Tính th?i gian ch?y b?ng miligiây
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Thoi gian chay cua chuong trinh: %f giay\n", time_taken);

    return 0;
}