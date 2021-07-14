#include "../include/BasePlugin.hh"
#include <dlfcn.h>
#include <errno.h>
// #include <glib.h>
#include <unistd.h>

int main()
{
    std::cout << "start dlopen" << std::endl;

    void* libtest = dlopen("/home/miao/Documents/github/summer2021-101/plugins_def/test/libtest.so", RTLD_LAZY);
    if (!libtest) {
        std::cerr << "cannot open lib " << dlerror() << std::endl;
        return -1;
    }

    create_t* create_test = (create_t*)dlsym(libtest, "create");
    if (!create_test) {
        std::cerr << "cannot open function" << dlerror() << std::endl;

        return -2;
    }

    destroy_t* destroy_test = (destroy_t*)dlsym(libtest, "destroy");
    if (!destroy_test) {
        std::cerr << "cannot open function" << dlerror() << std::endl;

        return -3;
    }

    BasePlugin* test = create_test();
    std::cout << test->getinfo() << std::endl;

    int status = system("nautilus /home");

    return 0;
}