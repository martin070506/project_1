#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0),auto_sync(false),bpm_tolerance(0)
{
    // Your implementation here
    decks[0]=nullptr;
    decks[1]=nullptr;
     std::cout << "[MixingEngineService] Initialized with 2 empty decks."<<std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout << "[MixingEngineService] Cleaning up decks..."<<std::endl;
    for(int i=0;i<2;i++)
    {
        if(decks[i])
        {
            delete decks[i];
            decks[i]=nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    PointerWrapper<AudioTrack> trackWrapper=track.clone();
    if(!trackWrapper)
    {
        std::cout <<" [ERROR] Track: "<<track.get_title()<<" failed to clone "<<std::endl;
        return -1;
    }  
    //INITIAL STATE
    if(!decks[0] && !decks[0])
    {
        decks[0]=trackWrapper.release();
        active_deck=0;
        return 0;
    }

    //INSTANT TRANSITION PATTERN
    std::cout <<"\n=== Loading Track to Deck ==="<<std::endl;
    int target_Deck=1-active_deck;
    std::cout <<"[Deck Switch] Target deck: "<<target_Deck<<std::endl;

    //UNLOAD TARGET IF OCCUPIED (THERE MAY BE A CASE WHERE DECK 0 IS ACTIVE, BUT WE ADD TO DECK 1 SO THERES NO NEED TO UNLOAD
    if(decks[target_Deck])
    {
        delete decks[target_Deck];
        decks[target_Deck]=nullptr;
    }
    trackWrapper.get()->load();
    trackWrapper.get()->analyze_beatgrid();

    //BPM MANAGEMENT
    if(decks[active_deck] && auto_sync)
    {
        if(!can_mix_tracks(trackWrapper))
        {
            sync_bpm(trackWrapper);
        }
    }
    decks[target_Deck]=trackWrapper.release();
    std::cout <<"[Load Complete] "<<track.get_title() <<" is now loaded on deck "<<target_Deck <<std::endl;

    //INSTANT TRANSMISSION
    std::cout <<"[Unload] Unloading previous deck "<<active_deck<< "("<<track.get_title()<<")"<<std::endl;
    delete decks[active_deck];
    decks[active_deck]=nullptr;

    active_deck=target_Deck;
    std::cout <<"[Active Deck] Switched to deck "<<target_Deck<<std::endl;
    return active_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
       if (!decks[active_deck] || !track || !track.get()) {
        std::cout << "\n[ERROR] nullptr failed to mix" << std::endl;
        return false;
   }

    int bpm_Difference = decks[active_deck]->get_bpm() - track.get()->get_bpm();
    if (bpm_Difference < 0) 
        bpm_Difference *= -1;

    return (bpm_Difference <= bpm_tolerance);
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
   if (!decks[0] || !decks[1] || !track) {
        std::cout << "\n[ERROR] nullptr failed to sync" << std::endl;
        return;
   }
   
   int originalBPM = track.get()->get_bpm();
   int averageBPM = (originalBPM + decks[active_deck]->get_bpm()) / 2;
   track.get()->set_bpm(averageBPM);
}
