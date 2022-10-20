int main() {

    int n = 100000;
    while (1) {
        for(int i=2; i<=n/2; ++i) {
            if(n%i==0) {
                break;
            }
        }
    }
    return 0;
}