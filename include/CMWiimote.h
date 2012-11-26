extern "C" {
    #include "wiiuse.h"
}

#include "CMWii_Mii.h"

#include <boost/thread.hpp>

class ICMWiimoteListener {
    public:
        virtual void fetchProgressChanged(int newProgress) {}
        virtual ~ICMWiimoteListener() {}
};

class CMWiimote {
    public:
        CMWiimote();
        ~CMWiimote();
        
        /* connect() must be called first after creating an instance of the
          class. Other functions are undefined until connected == 1 */
        
        void connect();
        int wiimoteConnected;
        
        /* After you are connected, you must startMonitoring() for the class
          to actually start listening to the wii remote */
        
        void startMonitoring();
        void endMonitoring();
        int monitoring;
        
        /* These functions get and set the internal array of CMii::Mii objects.
          These DO NOT actually read and write to the wii remote... you must
          use the next set of functions below. getMii will return a NULL
          pointer if the mii requested has not been fetched yet by using
          one of the fetch functions */
        
        CMWii::Mii* getMii(int slot);
        void setMii(CMWii::Mii *src, int slot);
        
        // Return 1 if mii at slot has changed and not been comitted back to
        // the wii remote
        int isMiiUnsaved(int slot);
        
        /* These functions handle reading and writing to/from the wii remote
          to/from the internal CMii:Mii array */
        
        void populateMiis();        //Fetch all miis
        void fetchMii(int slot);    //Fetch specific mii
        void commitChangedMiis();   //Write changed miis back the the wiimote
        
        int fetchProgress;
        ICMWiimoteListener *callbackObject;
        
    private:
        wiimote **wmList;
        wiimote *wm;
        
        int active;
        
        void monitorWiimote();
        int continueMonitoring;
        
        CMWii::Mii *miis[10];
        int changedMiis[10];
        
        unsigned char miiBuffer[100];
        int get_slot;
        int get_done;
};