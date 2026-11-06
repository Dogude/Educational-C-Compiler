#include "common.h"

unsigned char dos_header[64] = {
// PE (50 45 00 00) starts in 64th (0x40)
0x4D, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0x00, 0x00, 0x00

};


void write_to_data(int val) {

    if (DATA.len < DATA.cap) {

        DATA.data[DATA.len++] = val;

    }
    else {
        unsigned int new_cap = DATA.cap + FILE_ALIGN;
        if (new_cap > MAX_ALIGN) {

            // single section can not exceed 2gb
            exit_compiler();
        }      
        void* temp = realloc(DATA.data, new_cap);
        if (!temp) {

            exit_compiler();
        }
        DATA.data = temp;
        DATA.cap = new_cap;
        DATA.data[DATA.len++] = val;
    }

}

void write_to_text(int val) {

    if (TEXT.len < TEXT.cap) {

        TEXT.data[TEXT.len++] = val;

    }
    else {
        unsigned int new_cap = TEXT.cap + FILE_ALIGN;
        if (new_cap > MAX_ALIGN) {

            // single section can not exceed 2gb
            exit_compiler();
        }
        void* temp = realloc(TEXT.data, new_cap);
        if (!temp) {

            exit_compiler();
        }
        TEXT.data = temp;
        TEXT.cap = new_cap;
        TEXT.data[TEXT.len++] = val;
    }

}

void write_to_edata(int val) {

    if (eDATA.len < eDATA.cap) {

        eDATA.data[eDATA.len++] = val;

    }
    else {
        unsigned int new_cap = eDATA.cap + FILE_ALIGN;
        if (new_cap > MAX_ALIGN) {

            // single section can not exceed 2gb
            exit_compiler();
        }

        void* temp = realloc(eDATA.data, new_cap);
        if (!temp) {

            exit_compiler();
        }
        eDATA.data = temp;
        eDATA.cap = new_cap;
        eDATA.data[eDATA.len++] = val;
    }

}
