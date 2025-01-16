#include "./core/headers.h"
#include "./demo/demo.h"

int main()
{
    init_demo();

    // press ESC to quit demo
    play_demo();

    delete_demo();

    return 0;
}