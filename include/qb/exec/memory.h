#ifndef _QB_EXEC_MEMORY__H
#define _QB_EXEC_MEMORY__H

// map_demap (add_map, remove_map, add_default, remove_default)
// memory_space (read, write, [ access, space_lookup ])

struct memory_map_info {
    hwaddr  base;
    hwaddr  start;
    hwaddr  length;
    int     function;
    int     align;
};

struct map_interface {
    int (*add_map)(qb_object_t *space,
                   qb_object_t *dev,
                   qb_object_t *target,
                   map_info_t map_info);
    int (*remove_map)(qb_object_t *space,
                      qb_object_t *dev,
                      int function);
    int (*add_default)(qb_object_t *space,
                       qb_object_t *dev,
                       qb_object_t *target,
                       map_info_t map_info);
    void (*remove_default)(qb_object_t *space);
};

struct memory_space_interface {
    /* Read from the memory region. @addr is relative to @mr; @size is
     * in bytes. */
    uint64_t (*read)(qb_object_t *obj,
                     hwaddr addr,
                     unsigned size,
                     enum device_endian endianness);
    /* Write to the memory region. @addr is relative to @mr; @size is
     * in bytes. */
    void (*write)(qb_object_t *obj,
                  hwaddr addr,
                  uint64_t data,
                  unsigned size);
};

extern PyTypeObject qb_memory_interface_t;

#endif
