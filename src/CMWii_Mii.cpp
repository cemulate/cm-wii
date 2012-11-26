#include "CMWii_Mii.h"
#include <cstdio>
#include <cstring>
#include <fcntl.h>

CMWii::Mii::Mii(unsigned char *ptr) {
    memcpy(rawData, ptr, WIIMOTE_MII_DATA_BYTES_PER_SLOT);
    this->initName();
}

CMWii::Mii::Mii() {
    memset(rawData, 0, WIIMOTE_MII_DATA_BYTES_PER_SLOT);
}

CMWii::Mii::Mii(Mii *cpy) {
    memcpy(rawData, cpy->rawData, WIIMOTE_MII_DATA_BYTES_PER_SLOT);
    this->initName();
}

void CMWii::Mii::writeToFile(std::string filename) {
    int fd = open(filename.c_str(), O_RDWR|O_APPEND|O_CREAT);
    write(fd, rawData, WIIMOTE_MII_DATA_BYTES_PER_SLOT);
    close(fd);
}

void CMWii::Mii::initName() {
    ::Mii *m = (::Mii *)rawData;
    char *string = (char *)(m->name);
    
    int i = 0;
    int j = 0;
    char temp1[20];
    memcpy(temp1, string, 20);
    
    for (i = 0; i < 20; i++) {
        if (temp1[i] == 0x00) {
            temp1[i] = ' ';
        }
    }
    
    char fixedString[11];
    int k = 0;
    for (j = 1; j < 20; j +=2) {
        fixedString[k] = string[j];
        k++;
    }
    fixedString[10] = '\0';
    name = fixedString;
}