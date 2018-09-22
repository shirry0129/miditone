#include "ScoreTimeConverter.hpp"


namespace score {


	ScoreTimeConverter::ScoreTimeConverter(
		const std::vector<score::BeatEvent>& beat,
		const std::vector<score::TempoEvent>& tempo,
		int maxBar,
		double offset
	) : isReady(false) {
		create(beat, tempo, maxBar, offset);
	}

	ScoreTimeConverter::ScoreTimeConverter() : isReady(false) {}

	ScoreTimeConverter::~ScoreTimeConverter() {}

	bool ScoreTimeConverter::create(
		const std::vector<score::BeatEvent>& beat,
		const std::vector<score::TempoEvent>& tempo,
		int calcLimitBar,
		double offset
	) {
		
		isReady = false;

		if (beat.empty() || tempo.empty())
			return false;

		if (beat.front().bar + beat.front().posInBar != 1 ||
			tempo.front().bar + tempo.front().posInBar != 1)
			return false;


		// add clock time of score head
		clockTime.emplace_back(
			ScoreTimeSec(offset, ScoreTime(1))
		);


		math::Fraction begBarLen(0);  // distance bar from origin (bar:1)
		math::Fraction endBarLen(0);  // distance bar from origin (bar:1)
		auto beatEvent = beat.begin();
		auto tempoEvent = tempo.begin();

		const math::Fraction limitBarLen = ScoreTime(calcLimitBar).getBarLength();

		while (endBarLen < limitBarLen) {

			// next event
			beatEvent++;
			tempoEvent++;

			// check that whether event exist
			if (beatEvent == beat.cend())
				beatEvent--;
			if (tempoEvent == tempo.cend())
				tempoEvent--;


			// assign earlier event
			 ScoreTime *endScoreTime;
			if (beatEvent->getBarLength() < tempoEvent->getBarLength()) {
				endBarLen = beatEvent->getBarLength();
				endScoreTime = const_cast<BeatEvent*>(&(*beatEvent));
			} else {
				endBarLen = tempoEvent->getBarLength();
				endScoreTime = const_cast<TempoEvent*>(&(*tempoEvent));
			}

			// user end point
			if (endBarLen == begBarLen) {
				endBarLen = limitBarLen;
				endScoreTime = std::move(&ScoreTime(calcLimitBar));
			}

			// calculate clock time

			const math::Fraction delta = endBarLen - begBarLen;
			// note: beat and tempo are constant within [delta]

			const auto currentBeat = getBeat(beat, begBarLen);
			const auto currentTempo = getTempo(tempo, begBarLen);

			const double oneBeatLength = 60.0 / currentTempo;
			const double numofBeats = 4 * currentBeat.to_f();  // the number of quarter notes in a bar
			const double deltaClockTime = oneBeatLength * numofBeats * delta.to_f();


			// add to [scoreTimeSecList]
			const double ans = clockTime.back().sec + deltaClockTime;
			clockTime.emplace_back(
				ans, ScoreTime(*endScoreTime)
			);


			begBarLen = endBarLen;
		}


		// update member variable
		isReady = true;
		
		return true;
	}

	double ScoreTimeConverter::calcSec(const math::Fraction &barLen) {
		if (!isReady)
			return 0.0;

		// score time before convert
		const math::Fraction &srcBarLen = barLen;
		math::Fraction tmp = clockTime.back().getBarLength();

		if (tmp < srcBarLen)
			return -1.0f;

		// calculate delta time between two the clock times
		// get the clock time at previous [scoreTime]
		math::Fraction deltaBarLen;
		math::Fraction barLenBeg;
		double deltaClockTime;
		double clockTimeBeg;

		for (auto it = clockTime.crbegin(); it != clockTime.crend(); it++) {
			barLenBeg = it->getBarLength();

			if (barLenBeg <= srcBarLen) {
				clockTimeBeg = it->sec;
				math::Fraction barLenEnd = (it - 1)->getBarLength();
				deltaBarLen = barLenEnd - barLenBeg;
				deltaClockTime = (it - 1)->sec - it->sec;
				break;
			}
		}

		math::Fraction extraBarLen = srcBarLen - barLenBeg;
		double extraClockTime = deltaClockTime * (extraBarLen/deltaBarLen).to_f();

		return clockTimeBeg + extraClockTime;
	}

	const math::Fraction& ScoreTimeConverter::getBeat(const std::vector<BeatEvent> &beat, const math::Fraction &barLen) {
		for (auto it = beat.crbegin(); it != beat.crend(); it++) {
			if (it->getBarLength() <= barLen)
				return it->beat;
		}

		return std::move(math::Fraction(0));
	}

	double ScoreTimeConverter::getTempo(const std::vector<TempoEvent> &tempo, const math::Fraction &barLen) {
		for (auto it = tempo.crbegin(); it != tempo.crend(); it++) {
			if (it->getBarLength() <= barLen)
				return it->tempo;
		}

		return 0.0;
	}


}