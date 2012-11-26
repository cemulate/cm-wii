#include "CMWiimote.h"
#include <cstdio>
#include <fcntl.h>

#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

CMWiimote::CMWiimote() {
    wiimoteConnected = 0;
    monitoring = 0;
    
    int i = 0;
    for (i = 0; i < 10; i ++) {
        miis[i] = NULL;
        changedMiis[i] = 0;
    }
    
    fetchProgress = 0;
    callbackObject = NULL;
}

CMWiimote::~CMWiimote() {
    wiiuse_cleanup(wmList, 1);
}

void CMWiimote::connect() {
    wiimoteConnected = 1;
    
    wmList = wiiuse_init(1);
    
    int found, connected;
    found = wiiuse_find(wmList, 1, 5);
    if (!found) {
        printf("Couldn't find any wiimotes\n");
        wiimoteConnected = 0;
    }
    
    connected = wiiuse_connect(wmList, 1);
    if (!connected) {
        printf("Couldn't connect to any wiimotes\n");
        wiimoteConnected = 0;
    }
    
    wm = wmList[0];
}

void CMWiimote::startMonitoring() {
    monitoring = 1;
    
    continueMonitoring = 1;
    boost::thread t(boost::bind(&CMWiimote::monitorWiimote, this));
}

void CMWiimote::endMonitoring() {
    continueMonitoring = 0;
    while (monitoring) {
        
    }
}

void CMWiimote::monitorWiimote() {
    printf("\n-----\nThread will now monitor wii remote for events\n\n");
    while (continueMonitoring) {
        if (wiiuse_poll(wmList, 1)) {
            
            if (wm->event == WIIUSE_READ_DATA) {
                
                miis[get_slot] = new CMWii::Mii(miiBuffer);
                get_done = 1;
                
            }
            
        }
    }
    
    monitoring = 0;
}

CMWii::Mii* CMWiimote::getMii(int slot) {
    return miis[slot];
}

void CMWiimote::setMii(CMWii::Mii *src, int slot) {
    miis[slot] = new CMWii::Mii(src);
    changedMiis[slot] = 1;
}

int CMWiimote::isMiiUnsaved(int slot) {
    return changedMiis[slot];
}

void CMWiimote::populateMiis() {
    
    /* Synchronous method to fetch all 10 miis. This method will block the
      calling code until all miis are fetched */
    
    fetchProgress = 0;
    int i = 0;
    for (i = 0; i < 10; i++) {
        this->fetchMii(i);
        
        // The wiiuse framework is very weird about back-to-back read
        // requests... The only solution/workaround that works is to
        // give it some free time...
        
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        fetchProgress += 10;        
        if (callbackObject != NULL) {
            callbackObject->fetchProgressChanged(fetchProgress);
        }
    }
}

void CMWiimote::fetchMii(int slot) {
    
    /* Synchronous method to fetch a mii. This method will block the calling
      code until it is finished */
    
    get_slot = slot;
    get_done = 0;
    wiiuse_read_data(wm, miiBuffer, MII_OFFSET(slot), WIIMOTE_MII_DATA_BYTES_PER_SLOT);
    while (!get_done) {
        
    }
}

void CMWiimote::commitChangedMiis() {
    printf("Not implemented yet!");
}