extern "C" {

void test_copy(int n, int * a, int m, int * b) {
    for (int i = 0; i < m; i++) {
        b[i] = a[i % n];
    }
}

}
