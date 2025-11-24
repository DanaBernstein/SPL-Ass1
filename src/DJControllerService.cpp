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
    std::string track_id = track.get_title();
    if(cache.contains(track_id)) {
        //HIT case
        cache.get(track_id);
        return 1;
    }

    //clone to pointer_wraper
    PointerWrapper<AudioTrack> curr = track.clone();

    if(!curr) {
        std::cerr << "[ERROR] Track: " << track_id << "failed to clone" << std::endl;
        return 0;
    }

    //release then simulate and analyse
    AudioTrack* raw_ptr = curr.release();
    raw_ptr->load();
    raw_ptr->analyze_beatgrid();

    //wrap audio track and insert into cache
    PointerWrapper<AudioTrack> final_track_wrap(raw_ptr);
    bool avicted = cache.put(std::move(final_track_wrap));
    if(avicted) return -1; //MISS with evicted
    return 0; //MISS 
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
    AudioTrack* track = cache.get(track_title);
    if(track) return track;
    return nullptr; 
}
