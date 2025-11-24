#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;

    std::cout << "[MixingEngineService] Initialized with 2 empty decks."<< "\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    for (size_t i = 0; i < 2; ++i) {
        if(decks[i]) delete decks[i];
        decks[i] = nullptr;
    }

    active_deck = 0;
    auto_sync = false;
    bpm_tolerance = 0;

    std::cout << "[MixingEngineService] Cleaning up decks."<< "\n";  
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===" << "\n";
    PointerWrapper<AudioTrack> curr_track = track.clone();
    if(!curr_track) {
        std::cout << "[ERROR] Track: " << track.get_title() << " failed to clone" << "\n";
        return -1;
    }

    size_t deck_to_load = 1;
    
    //check if this is the first load or active deck is 1 - then the deck to load is 0
    if((decks[0] == nullptr && decks[1] == nullptr) || active_deck == 1) {
        deck_to_load = 0;
    }

    std::cout << "[Deck Switch] Target deck: " << deck_to_load << "\n";

    //unload target deck if needed
    if(decks[deck_to_load] != nullptr) {
        delete decks[deck_to_load];
        decks[deck_to_load] = nullptr;
    }

    //simulating and loading prossess
    curr_track.get()->load();
    curr_track.get()->analyze_beatgrid();


    //BPM Management
    if(decks[active_deck] != nullptr && auto_sync) {
        if(!can_mix_tracks) {
            sync_bpm(curr_track);
        }
    }

    //Release pointer from PointerWrapper and assign to decks[target]
    decks[deck_to_load]=curr_track.release();
    std::cout << "[Load Complete]" << decks[deck_to_load]->get_title() << "is now loaded on deck"<< deck_to_load << "\n";
    //instant transition
    if(decks[active_deck]!= nullptr){
        std::cout << "[Unload] Unloading previous deck " << active_deck << " ('" << decks[active_deck]->get_title() << "')\n";
        delete decks[active_deck];
        decks[active_deck]=nullptr;
    }

    active_deck=deck_to_load;
    std::cout << "[Active Deck] Switched to deck " << active_deck << "\n";

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
    //no track to compare with
    if(decks[active_deck]== nullptr) return false;
    if(track.get()==nullptr) return false;
    //getting both bpm
    int active_deck_bpm= decks[active_deck]->get_bpm();
    int track_bpm= track->get_bpm();
    //calculating differnce
    int differnce=std::abs(active_deck_bpm-track_bpm);
    //can mix
    if(differnce<=bpm_tolerance) return true;
    //canot mix
    return false;
    
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck]!= nullptr && track.get()!=nullptr){
        int track_bpm= track->get_bpm();
        int active_deck_bpm= decks[active_deck]->get_bpm();
        int average_bpm=(track_bpm+active_deck_bpm)/2;
        track.get()->set_bpm(average_bpm);
        std::cout << "[Sync BPM] Syncing BPM from " << track_bpm << " to " << average_bpm << "\n";
    }
}
