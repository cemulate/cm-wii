extern "C" {
    #include "Mii.h"
}

#include <string>

namespace CMWii {
    class Mii {
	public:
	    Mii();
	    Mii(unsigned char *);
	    Mii(Mii *cpy);
	    
	    unsigned char rawData[WIIMOTE_MII_DATA_BYTES_PER_SLOT];
	    
	    void writeToFile(std::string filename);
	    
	    std::string name;
	    
	private:
	    void initName();
    };
}