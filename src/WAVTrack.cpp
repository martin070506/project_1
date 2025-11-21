#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, int duration, int bpm, int sample_rate, int bit_depth):
    AudioTrack(title, artists, duration, bpm), 
    sample_rate(sample_rate), 
    bit_depth(bit_depth) 
{

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    std::cout<< "Loading WAV: <"<<this->title<<"> at <" <<this->sample_rate<<"> Hz/"<<this->bit_depth<<"bit (uncompressed)..."<<std::endl;
    long size (this->duration_seconds*this->sample_rate*(this->bit_depth/8)*2);
    std::cout<< "  → Estimated file size: <"<<size<<"> bytes" << std::endl;
     std::cout<< "  → Fast loading due to uncompressed format." << std::endl;

}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"
    int beats_estimated ((this->duration_seconds/60.0)*this->bpm);
    std::cout << "  → Estimated beats: <"<<beats_estimated<<">  → Precision factor: 1 (uncompressed audio)"<<std::endl;
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    double quality_Score(70.0);
    if(this->sample_rate>=44100)
    {
        quality_Score+=10;
        if(this->sample_rate>=96000) quality_Score+=5;
    } 
    if(this->bit_depth>=16)
    {
        quality_Score+=10;
        if(this->bit_depth>=24) quality_Score+=5;
    }
    if(quality_Score>100) quality_Score=100;
    return quality_Score;
    
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    WAVTrack* copy =new WAVTrack(this->title,this->artists,this->duration_seconds,this->bpm,this->sample_rate,this->bit_depth);
    copy->waveform_size=this->waveform_size;
    for(size_t i=0 ; i<this->waveform_size ; i++)
    { 
        copy->waveform_data[i]=this->waveform_data[i];
    }
    return PointerWrapper<AudioTrack>(copy);
}