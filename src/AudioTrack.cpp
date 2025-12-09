#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>
#include <stdexcept>

AudioTrack::AudioTrack(const std::string& title,
                       const std::vector<std::string>& artists,
                       int duration,
                       int bpm,
                       size_t waveform_samples)
    : title(title),
      artists(artists),
      duration_seconds(duration),
      bpm(bpm),                     // 4th (matches class)
      waveform_size(waveform_samples),
      waveform_data(new double[waveform_size])
{
    // Allocate memory for waveform analysis

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }

    // std::cout << "AudioTrack created: " << title << " by " << std::endl;
    // for (const auto& artist : artists) {
    //    std::cout << artist << " ";
    // }
    // std::cout << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack() {
    // TODO: Implement the destructor
   // std::cout << "AudioTrack destructor called for: " << title << std::endl;
    delete[] waveform_data;
    waveform_data=nullptr;
}

AudioTrack::AudioTrack(const AudioTrack& other)
    : title(other.title),
      artists(other.artists),
      duration_seconds(other.duration_seconds),
      bpm(other.bpm),
      waveform_size(other.waveform_size),
      waveform_data(other.waveform_size > 0
                    ? new double[other.waveform_size]
                    : nullptr)
{
    //std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;

    
    if (waveform_data) {
        for (size_t i = 0; i < waveform_size; ++i) {
            waveform_data[i] = other.waveform_data[i];
        }
    }
}
AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    // TODO: Implement the copy assignment operator
    // std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    // Your code here...

    //SELF CHECK--- SO WE DONT DELETE OURSELVES THAN COPY FROM IT
    if (this == &other)
        return *this;   

    delete[] waveform_data;

    this->title = other.title;
    this->duration_seconds = other.duration_seconds;
    this->bpm = other.bpm;
    this->artists = other.artists;

    this->waveform_size = other.waveform_size;

    if (other.waveform_size > 0)
        this->waveform_data = new double[this->waveform_size];
    else 
        this->waveform_data = nullptr;

    for(size_t i = 0; i < waveform_size; i++)
        this->waveform_data[i] = other.waveform_data[i];

        //so we need to copy all variables and go through the array, create a new one and copy each variable from that array

    return *this;
}

AudioTrack::AudioTrack(AudioTrack&& other) noexcept
    : title(std::move(other.title)),
      artists(std::move(other.artists)),
      duration_seconds(other.duration_seconds),
      bpm(other.bpm),
      waveform_size(other.waveform_size),
      waveform_data(other.waveform_data)
{
    // leave 'other' in a safe, empty state
    other.waveform_size = 0;
    other.waveform_data = nullptr;
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    // TODO: Implement the move assignment operator
    //std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    // Your code here...

    if (this != &other) {

        delete[] waveform_data;

        this->title = other.title;
        this->duration_seconds = other.duration_seconds;
        this->bpm = other.bpm;
        this->artists = other.artists;

        this->waveform_size = other.waveform_size;
        this->waveform_data = other.waveform_data;
        other.waveform_data=nullptr;
        other.waveform_size=0;
        other.bpm=0;
        other.duration_seconds=0;
    }
    
    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}

void AudioTrack::set_bpm(int bpm)
{
    if (bpm < 0)
        throw std::invalid_argument("BPM cannot be negative");
    this->bpm = bpm;
}