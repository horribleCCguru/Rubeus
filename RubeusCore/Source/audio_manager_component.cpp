#include <audio_manager_component.h>

namespace Rubeus
{
	namespace AudioComponents
	{
		RAudioManager::RAudioManager()
		{
		}

		RAudioManager::~RAudioManager()
		{
			int i = 0;
			while(i < m_MusicTracks.size())
				delete m_MusicTracks[i++];

			i = 0;
			while(i < m_SoundBuffers.size())
				delete m_SoundBuffers[i++];

			i = 0;
			while(i < m_SoundTracks.size())
				delete m_SoundTracks[i++];
		}

		void RAudioManager::addSoundTrack(const int count)
		{
			int i = 0;
			while(i++ < count)
			{
				m_SoundBuffers.push_back(new sf::SoundBuffer());
				m_SoundTracks.push_back(new sf::Sound());
			}
		}

		void RAudioManager::addMusicTrack(const int count)
		{
			m_MusicTracks.push_back(new sf::Music());
		}

		void RAudioManager::loadTrack(ETrackType trackType, ETrackName trackName, std::string filePath, float volume, bool loop_enabled)
		{
			if(trackType == SOUND_TRACK)
			{
				m_SoundTracks[trackName]->setBuffer(*m_SoundBuffers[trackName]);
				m_SoundTracks[trackName]->setVolume(volume);

				if(loop_enabled)
				{
					m_SoundTracks[trackName]->setLoop(true);
				}
			}

			else if(trackType == MUSIC_TRACK)
			{
				m_MusicTracks[trackName]->openFromFile(filePath);

				if(loop_enabled)
				{
					m_MusicTracks[trackName]->setLoop(true);
				}
			}
		}

		void RAudioManager::playTrack(ETrackType trackType, ETrackName trackName)
		{
			if(trackType == SOUND_TRACK)
			{
				m_SoundTracks[trackName]->play();
			}
			else if(trackType == MUSIC_TRACK)
			{
				m_MusicTracks[trackName]->play();
			}
		}

		void RAudioManager::pauseTrack(ETrackType trackType, ETrackName trackName)
		{
			if(trackType == SOUND_TRACK)
			{
				m_SoundTracks[trackName]->pause();
			}
			else if(trackType == MUSIC_TRACK)
			{
				m_MusicTracks[trackName]->pause();
			}
		}

		void RAudioManager::stopTrack(ETrackType trackType, ETrackName trackName)
		{
			if(trackType == SOUND_TRACK)
			{
				m_SoundTracks[trackName]->stop();
			}
			else if(trackType == MUSIC_TRACK)
			{
				m_MusicTracks[trackName]->stop();
			}
		}

		void RAudioManager::stepDownMusicVolume(const float duration, const float floorVolume)
		{
		}

		void RAudioManager::setVolume(ETrackType trackType, ETrackName trackName, const float volume)
		{
			if(trackType == SOUND_TRACK)
			{
				m_SoundTracks[trackName]->setVolume(volume);
			}
			else if(trackType == MUSIC_TRACK)
			{
				m_MusicTracks[trackName]->setVolume(volume);
			}
		}

		void RAudioManager::setPitch(ETrackType trackType, ETrackName trackName, const float pitch)
		{
			if(trackType == SOUND_TRACK)
			{
				m_SoundTracks[trackName]->setPitch(pitch);
			}
			else if(trackType == MUSIC_TRACK)
			{
				m_MusicTracks[trackName]->setPitch(pitch);
			}
		}

		void RAudioManager::addToTick(void(*function)())
		{
			m_TickFunctions.push_back(function);
		}

		void RAudioManager::tick()
		{
			for(size_t i = 0; i < m_TickFunctions.size(); i++)
			{
				m_TickFunctions[i]();
			}

			if(!m_AudioTasks.empty())
			{
				for(size_t i = 0; i < m_AudioTasks.size(); i++)
				{
					auto temp = m_AudioTasks[i];
					switch(temp->m_TrackToApplyTo)
					{
						case SOUND_TRACK:
						{
							/*switch(temp->m_EffectToApply)
							{
								default:
							}*/
						}
						break;

						case MUSIC_TRACK:
						{
							switch(temp->m_EffectToApply)
							{
								case fade_in:
								{
									if(m_MusicTracks[temp->m_TrackName]->getVolume() > 0)
									{
										auto bin = m_MusicTracks[temp->m_TrackName];
										bin->setVolume(bin->getVolume() - VOLUME_STEP);
									}
								}
								break;

								case fade_out:
									if(m_MusicTracks[temp->m_TrackName]->getVolume() <= MAX_VOLUME - VOLUME_STEP)
									{
										auto bin = m_MusicTracks[temp->m_TrackName];
										bin->setVolume(bin->getVolume() + VOLUME_STEP);
									}
									break;

								default:
									break;
							}
						}
						break;

						default:
							break;
					}
				}
			}
		}

		void RAudioManager::onMessage(Message * msg)
		{
		}
	}
}
