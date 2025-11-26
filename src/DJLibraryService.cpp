#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    //std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    
    //clean library first
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();

    for(size_t i=0; i<library_tracks.size(); ++i){
        AudioTrack* new_track = nullptr;
        if(library_tracks[i].type=="MP3") {
            new_track = new MP3Track(library_tracks[i].title,library_tracks[i].artists,library_tracks[i].duration_seconds,library_tracks[i].bpm,library_tracks[i].extra_param1,library_tracks[i].extra_param2);

            std::cout << "MP3: MP3Track created:" << library_tracks[i].extra_param1 << "kbps" << "\n";
        }
        else if(library_tracks[i].type=="WAV"){
            new_track = new WAVTrack(library_tracks[i].title,library_tracks[i].artists,library_tracks[i].duration_seconds,library_tracks[i].bpm,library_tracks[i].extra_param1,library_tracks[i].extra_param2);

            std::cout << "WAV: WAVTrack created:" << library_tracks[i].extra_param1 << "Hz/"<<library_tracks[i].extra_param2 <<"bit";

        }

        if(new_track) library.push_back(new_track);
    }
    std::cout << "[INFO] Track library built: "<< library_tracks.size() << "tracks loaded";
}

DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }

    library.clear();
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
    for (AudioTrack* track : library) {
        if(track) {
            if(track->get_title() == track_title) return track;
        }
    }

    return nullptr;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout<<"[INFO] Loading playlist: "<<playlist_name << "\n";
    //create playlist
    playlist= Playlist(playlist_name);
    //find each track index in the library - if exists and add it as a trackAudio to the new playlist
     for(size_t i=0; i<track_indices.size(); ++i){
        if(size_t(track_indices[i]-1) >= library.size()){
           std::cout << "[WARNING] Invalid track index: "<<track_indices[i]; 
        }
        else{
            PointerWrapper<AudioTrack> to_add = library[track_indices[i]-1]->clone();
            if(!to_add) std::cout << "[ERROR] clone failed "; 
            else{
                std::string title_track = to_add.get()->get_title();
                to_add.get()->load();
                to_add.get()->analyze_beatgrid();
                playlist.add_track(to_add.release());
                std::cout<< "Added '" << title_track <<"' to playlist ’" <<playlist_name<< "\n" << "’ Log summary: [INFO] Playlist loaded:" <<playlist_name<< " (" << playlist.get_track_count() <<" tracks)";
            }
        }
     }    
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> all_titles{};
    std::vector<AudioTrack*> all_tracks = playlist.getTracks();
    for(size_t i = 0; i < all_tracks.size(); ++i) {
        all_titles.push_back(all_tracks[i]->get_title());
    }

    return all_titles;
}
