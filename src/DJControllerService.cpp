#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    std::string key = track.get_title();

    if (cache.contains(key)){
        cache.get(key);
        return 1; 
    }

    PointerWrapper<AudioTrack> trackCloneWrapper = track.clone();

    if (!trackCloneWrapper) {
        std::cerr << "[ERROR] Track: \"" << key << "\" failed to clone" << std::endl;
        return 0; 
    }

    AudioTrack* rawTrack = trackCloneWrapper.release();;

    rawTrack->load();
    rawTrack->analyze_beatgrid();

    PointerWrapper<AudioTrack> rawTrackClone = PointerWrapper<AudioTrack>(rawTrack);
    //rawTrackClone's pointer points to same Address as rawTrack and trackCloneWrapper
    
    if (cache.put(std::move(rawTrackClone))) 
        return -1;
    else     
        return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
   //DOES NOT TRANSFER OWNERSHIP
   std::cout << "\nGET TRACK FROM CACHE WAS CALLED----- THIS DOES NOT TRANSFER OWNERSHIP\n";
   AudioTrack* AT_ptr = cache.get(track_title);
   return AT_ptr;
}
