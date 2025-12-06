#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist),library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    int count =0;
    for (SessionConfig::TrackInfo TI : library_tracks)
    {
        if(TI.type=="MP3")
        {
            AudioTrack * AT = new MP3Track(TI.title,TI.artists,TI.duration_seconds,TI.bpm,TI.extra_param1,TI.extra_param2);
            this->library.push_back(AT);
            std::cout<<"MP3Track created: "<<TI.extra_param1<<" kbps"<<std::endl;
            count++;
        }
        if(TI.type=="WAV")
        {
            AudioTrack * AT = new WAVTrack(TI.title,TI.artists,TI.duration_seconds,TI.bpm,TI.extra_param1,TI.extra_param2);
            this->library.push_back(AT);
            std::cout<<"WAVTrack created: "<<TI.extra_param1<<"Hz/"<<TI.extra_param2 <<"bit"<<std::endl;
            count++;
        }
         std::cout<<"[INFO] Track library built: "<<count<<" tracks loaded"<<std::endl;
    }
    
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    std::cout << "\nFIND TRACK FROM DJLibraryService----- THIS DOES NOT TRANSFER OWNERSHIP\n";
    return playlist.find_track(track_title);
    
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "\n[INFO] Loading playlist: " << playlist_name << std::endl;
    size_t indexCast;
    for (int index : track_indices){
        indexCast=index;
        if (index < 0 || library.size() < indexCast){
            std::cout << "\n[WARNING] Invalid track index: " << index << std::endl;
            continue;
        }

        AudioTrack* cloneTrack = library[index-1]->clone().release();
        if (!cloneTrack){
            std::cout << "\n[ERROR] AudioTrack is nullptr, index = " << index << std::endl;
            continue;
        }

        cloneTrack->load();
        cloneTrack->analyze_beatgrid();

        playlist.add_track(cloneTrack);
        std::cout << "\nAdded " << cloneTrack->get_title() << "to playlist " << playlist_name << std::endl;
    }
    
    // For now, add a placeholder to fix the linker error
    // (void)playlist_name;  // Suppress unused parameter warning
    // (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> Titles;
    for(AudioTrack* t :playlist.getTracks())
    {
        Titles.push_back(t->get_title());
    }
    return Titles;
}
//IMPLEMEBTED DESTRUCTOR AS ASKED THAT WILL DELETE ALL TRACKS IN LIBRARY
DJLibraryService::~DJLibraryService()
{
    for (AudioTrack* AT : library)
        delete AT;
}
