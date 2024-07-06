#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ELF_MAGIC 0x464C457F

struct ELF64_Header {
    uint32_t e_magic;
    unsigned char e_ident[12];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

enum format {
    BITS32 = 0x01,
    BITS64 = 0x02
};

enum endianess {
    LITTLE = 0x01,
    BIG = 0x02
};

enum os {
    LINUX = 0x03,
    OPENBSD = 0x0C
};

enum ftype {
    EXECUTABLE = 0x02,
    SHARED_OBJECT = 0x03
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Invalid usage! Please, provide only one file to parse.");
        return EXIT_FAILURE;
    }
    struct ELF64_Header hdr;
    
    FILE* f = fopen(argv[1], "rb");
    if (!f) {
        puts("Failed to open file!");
        return EXIT_FAILURE;
    }

    fread(&hdr, sizeof(struct ELF64_Header), 1, f);

    if (hdr.e_magic != ELF_MAGIC) {
        puts("Invalid file: bad magic");
        goto close;
    }

    switch (hdr.e_ident[0]) {
        case BITS32:
            puts("Binary format: 32 bit");
            break;
        case BITS64:
            puts("Binary format: 64 bit");
            break;
        default:
            puts("Invalid binary format!");
            goto close;
    }

    switch (hdr.e_ident[1]) {
        case LITTLE:
            puts("Endianess: little endian");
            break;
        case BIG:
            puts("Endianess: big endian");
            break;
        default:
            puts("Invalid endianess!");
            goto close;
    }

    switch (hdr.e_ident[4]) {
        case LINUX:
            puts("Target OS: Linux");
            break;
        case OPENBSD:
            puts("Target OS: OpenBSD");
            break;
        default:
            puts("Target OS: unknown");
            break;
    }

    switch (hdr.e_type) {
        case EXECUTABLE:
            puts("File type: executable");
            break;
        case SHARED_OBJECT:
            puts("File type: shared object");
            break;
        default:
            puts("Invalid file type!");
            goto close;
    }

    printf("Architecture hex code: %X\n", hdr.e_machine);
    printf("Entry point: 0x%lX\n", hdr.e_entry);
    printf("Program header offset: 0x%lX\n", hdr.e_phoff);
    printf("Section header offset: 0x%lX\n", hdr.e_shoff);
    printf("ELF header size: 0x%X\n", hdr.e_ehsize);
    printf("Program header entry size: 0x%X\n", hdr.e_phentsize);
    printf("Section header entry size: 0x%X\n", hdr.e_shentsize);
    printf("Count of section header entries: 0x%X\n", hdr.e_shnum);

close:
    fclose(f);
}
