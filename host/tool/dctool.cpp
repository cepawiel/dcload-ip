
#include <stdio.h>
#include <libelf.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/time.h>
#include <unistd.h>

#include <TargetConnection.hpp>
#include <Packet.hpp>

using namespace dctool;

uint32_t load_elf(TargetConnection &tc, std::string path)
{
    int size = 0;
    // int sectsize;
    // unsigned char *inbuf;
    struct timeval starttime;//, endtime;
    unsigned int address;
    Elf *elf;
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    Elf_Scn *section = NULL;
    Elf_Data *data;
    char *section_name;
    size_t index;
    if (elf_version(EV_CURRENT) == EV_NONE)
    {
        fprintf(stderr, "libelf initialization error: %s\n", elf_errmsg(-1));
        return -1;
    }

    int inputfd = open(path.c_str(), O_RDONLY);
    if (inputfd < 0)
    {
        printf("Failed to open ELF\n");
        return -1;
    }

    if (!(elf = elf_begin(inputfd, ELF_C_READ, NULL)))
    {
        fprintf(stderr, "Cannot read ELF file: %s\n", elf_errmsg(-1));
        return -1;
    }

    if (elf_kind(elf) == ELF_K_ELF)
    {
        if (!(ehdr = elf32_getehdr(elf)))
        {
            fprintf(stderr, "Unable to read ELF header: %s\n", elf_errmsg(-1));
            return -1;
        }

        address = ehdr->e_entry;
        printf("File format is ELF, start address is 0x%08x\n", address);

        /* Retrieve the index of the ELF section containing the string table of
           section names */
        if (elf_getshdrstrndx(elf, &index))
        {
            fprintf(stderr, "Unable to read section index: %s\n", elf_errmsg(-1));
            return -1;
        }

        gettimeofday(&starttime, 0);
        while ((section = elf_nextscn(elf, section)))
        {
            if (!(shdr = elf32_getshdr(section)))
            {
                fprintf(stderr, "Unable to read section header: %s\n", elf_errmsg(-1));
                return -1;
            }

            if (!(section_name = elf_strptr(elf, index, shdr->sh_name)))
            {
                fprintf(stderr, "Unable to read section name: %s\n", elf_errmsg(-1));
                return -1;
            }

            if (!shdr->sh_addr)
                continue;

            /* Check if there's some data to upload. */
            data = elf_getdata(section, NULL);
            if (!data->d_buf || !data->d_size)
                continue;

            printf("Section %s, lma 0x%08x, size %d\n", section_name,
                   shdr->sh_addr, shdr->sh_size);
            size += shdr->sh_size;

            do
            {
                if (!tc.MemWrite(shdr->sh_addr + data->d_off, (uint8_t *) data->d_buf, data->d_size)) {
                    printf("Failed to write %ld bytes at 0x%lX\n", data->d_size, shdr->sh_addr + data->d_off);
                    return -1;
                }
            } while ((data = elf_getdata(section, data)));
        }

        elf_end(elf);
        close(inputfd);
    }
    return address;
}

int main()
{
    auto config = TargetConnection::Config{"dut", "172.16.0.99"};

    auto tc = TargetConnection();
    if (!tc.Setup(config))
    {
        printf("Failed Setup!\n");
        return -1;
    }

    printf("Geting Target Version\n");
    auto ver = tc.GetTargetVersion();
    if (!ver)
    {
        printf("Failed to get Target Version\n");
        return 0;
    }
    printf("Target Version: %s\n", ver->c_str());

if(0) {
    uint32_t startAddr = load_elf(tc, "/home/coltonp/Git/dcload-ip/target/builddir/loader/loader.elf");
    if(startAddr == (uint32_t)(-1) ){
        printf("Failed to Write ELF to Target\n");
    }

    if(!tc.Execute(startAddr)) {
        printf("Failed to Execute\n");
    }
    printf("Started ELF\n");
    sleep(8);
    // tc.Reboot();
}

    // for(int i = 0; i  < 2; i++) 
    {
    uint8_t mapleBase[] = { 0x02, 0x20, 0x00, 0x00 };
    if(!tc.Maple(&mapleBase[0], sizeof(mapleBase))) {
        printf("Maple Failed!\n");
    }
    }

    // for(int i = 0; i  < 2; i++) 
    {
        uint8_t mapleSub1[] = { 0x02, 0x1, 0x00, 0x00 };
        if(!tc.Maple(&mapleSub1[0], sizeof(mapleSub1))) {
            printf("Maple Failed!\n");
        }
    }
    
    // for(int i = 0; i  < 2; i++) 
    {
        uint8_t mapleSub2[] = { 0x02, 0x2, 0x00, 0x00 };
        if(!tc.Maple(&mapleSub2[0], sizeof(mapleSub2))) {
            printf("Maple Failed!\n");
        }
    }

    return 0;
}