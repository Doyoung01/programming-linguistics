
float area;
float circum;
void main() {
    int i;

    for (i = 1; i <= MAX; i++) {
        area = i * i * PI;
        circum = 2 * PI * i;
    }
}