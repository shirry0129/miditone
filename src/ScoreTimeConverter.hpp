
/*
This class converts the score time (expressed by the number of bar, and note length)
to clock time (written by millisecond).

Author	: Nanami Yamamoto
Date	: 2018/9/21
*/


#ifndef _SCORETIMECONVERTER_HPP_
#define _SCORETIMECONVERTER_HPP_


#include "ScoreReader.hpp"


namespace score {

	class ScoreTimeConverter {

		struct ScoreTimeSec : ScoreTime {
			ScoreTimeSec(double _sec, ScoreTime scoreTime) noexcept
				: sec(_sec), ScoreTime(scoreTime) {}

			const double sec;
		};


	public:
		ScoreTimeConverter(
			const std::vector<score::BeatEvent> &beat,
			const std::vector<score::TempoEvent> &tempo,
			double offset = 0.0
		);

		ScoreTimeConverter();

		~ScoreTimeConverter();

		bool create(
			const std::vector<score::BeatEvent> &beat,
			const std::vector<score::TempoEvent> &tempo,
			double offset = 0.0
		);

		double calcSec(const math::Fraction &barLength) const;
		
	private:
		std::vector<ScoreTimeSec> clockTime;

		math::Fraction lastBeat;
		double lastTempo;

		bool isReady;

		void init();

		const math::Fraction& getBeat(const std::vector<BeatEvent> &beat, const math::Fraction &barLength) const;
		double getTempo(const std::vector<TempoEvent> &tempo, const math::Fraction &barLength) const;
		double calcClockTimeLen(const math::Fraction &beat, double tempo, const math::Fraction &barLen) const;

	};


}


#endif // !_SCORETIMECONVERTER_HPP_
