#include <stdio.h>
#include <string.h>
#include "hashmap.h"

struct user {
    size_t id;
    double age;
};

int user_compare(const void *a, const void *b, void *udata) {
    const struct user *ua = a;
    const struct user *ub = b;
    // return strcmp(ua->name, ub->name);
    if (ua->id == ub->id) {
        return 0;
    } else if (ua->id < ub->id) {
        return -1;
    } else {
        return 1;
    }
}

bool user_iter(const void *item, void *udata) {
    const struct user *user = item;
    printf("%zu (age=%f)\n", user->id, user->age);
    return true;
}

// uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1) {
//     const struct user *user = item;
//     return hashmap_sip(user->id, strlen(user->id), seed0, seed1);
// }

uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct user *user = item;
    return hashmap_sip(&user->id, sizeof(size_t), seed0, seed1);
}

int main() {
    // create a new hash map where each item is a `struct user`. The second
    // argument is the initial capacity. The third and fourth arguments are 
    // optional seeds that are passed to the following hash function.
    struct hashmap *map = hashmap_new(sizeof(struct user), 0, 0, 0, 
                                     user_hash, user_compare, NULL, NULL);

    // Here we'll load some users into the hash map. Each set operation
    // performs a copy of the data that is pointed to in the second argument.
    hashmap_set(map, &(struct user){ .id=14, .age=4.4 });
    hashmap_set(map, &(struct user){ .id=13, .age=6.8 });
    hashmap_set(map, &(struct user){ .id=7, .age=4.7 });

    struct user *user; 
    
    printf("\n-- get some users --\n");
    user = hashmap_get(map, &(struct user){ .id=13 });
    printf("%zu age=%f\n", user->id, user->age);

    user = hashmap_get(map, &(struct user){ .id=7 });
    printf("%zu age=%f\n", user->id, user->age);

    user = hashmap_get(map, &(struct user){ .id=14 });
    printf("%zu age=%f\n", user->id, user->age);

    user = hashmap_get(map, &(struct user){ .id=99 });
    printf("%d does %s\n", 99, user?"exist":"not exist");

    printf("\n-- iterate over all users (hashmap_scan) --\n");
    hashmap_scan(map, user_iter, NULL);

    printf("\n-- iterate over all users (hashmap_iter) --\n");
    size_t iter = 0;
    void *item;
    while (hashmap_iter(map, &iter, &item)) {
        const struct user *user = item;
        printf("%zu (age=%f)\n", user->id, user->age);
    }

    hashmap_free(map);
}

// output:
// -- get some users --
// Jane age=47
// Roger age=68
// Dale age=44
// not exists
// 
// -- iterate over all users (hashmap_scan) --
// Dale (age=44)
// Roger (age=68)
// Jane (age=47)
//
// -- iterate over all users (hashmap_iter) --
// Dale (age=44)
// Roger (age=68)
// Jane (age=47)
