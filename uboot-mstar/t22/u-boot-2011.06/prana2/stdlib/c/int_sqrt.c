unsigned int int_sqrt(unsigned int n)
{
    unsigned int root;

    for (root = 0; n >= (2 * root) + 1; n -= (2 * root++) + 1);

    return root;
}
