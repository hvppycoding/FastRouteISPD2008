#include <flute.h>
#include <stdio.h>

using namespace Flute;

int main() {
    printf("Start\n");
    Tree t = Tree();

    printf("Tree Generated\n");
    DTYPE tmp_xs[4] = {0, 1, 1, 2};
    DTYPE tmp_ys[4] = {0, 1, 1, 2};
    int s[4]      = {1, 0, 3, 2};
    int acc = 9;
    int d = 4;

    
    printf("readLUT\n");
    Flute::readLUT();
    printf("readLUT Done\n");

    printf("Flute\n");
    t = Flute::flutes(d, tmp_xs, tmp_ys, s, acc);
    printf("Flute Done\n");

    for (int i = 0; i < d * 2 - 2; i++) {
        Branch b = t.branch[i];
        printf("%d, %d -> %d\n", b.x, b.y, b.n);
    }

    return 0;
}

