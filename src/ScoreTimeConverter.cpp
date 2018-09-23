#include "ScoreTimeConverter.hpp"


namespace score {


	ScoreTimeConverter::ScoreTimeConverter(
		const std::vector<score::BeatEvent>& beat,
		const std::vector<score::TempoEvent>& tempo,
		double offset
	) : isReady(false) {
		create(beat, tempo, offset);
	}

	ScoreTimeConverter::ScoreTimeConverter() : isReady(false) {}

	ScoreTimeConverter::~ScoreTimeConverter() {}

	bool ScoreTimeConverter::create(
		const std::vector<score::BeatEvent>& beat,
		const std::vector<score::TempoEvent>& tempo,
		double offset
	) {
		
		init();


		if (beat.empty() || tempo.empty())
			return false;

		if (beat.front().getBarLength() != 0 || 
			tempo.front().getBarLength() != 0)
			return false;


		// add clock time of score head
		clockTime.emplace_back(
			ScoreTimeSec(offset, ScoreTime(1))
		);


		math::Fraction begBarLen(0);  // bar length from origin (bar:1)
		math::Fraction endBarLen(0);  // bar length from origin (bar:1)
		auto beatEvent = beat.begin();
		auto tempoEvent = tempo.begin();

		// next event
		if (beatEvent + 1 != beat.end())
			beatEvent++;
		if (tempoEvent + 1 != tempo.end())
			tempoEvent++;

		while (true) {

			// assign earlier event
			 ScoreTime *endScoreTime;
			if (beatEvent->getBarLength() < tempoEvent->getBarLength()) {
				endBarLen = beatEvent->getBarLength();
				endScoreTime = const_cast<BeatEvent*>(&(*beatEvent));

				// next event
				if (beatEvent + 1 != beat.end())
					beatEvent++;

			} else {
				endBarLen = tempoEvent->getBarLength();
				endScoreTime = const_cast<TempoEvent*>(&(*tempoEvent));

				// next event
				if (tempoEvent + 1 != tempo.end())
					tempoEvent++;
			}


			if (endBarLen == begBarLen)
				break;  // exit


			// calculate clock time

			const math::Fraction delta = endBarLen - begBarLen;
			// note: beat and tempo are constant within [delta]

			const auto currentBeat = getBeat(beat, begBarLen);
			const auto currentTempo = getTempo(tempo, begBarLen);

			const double deltaClockTime = calcClockTimeLen(currentBeat, currentTempo, delta);

			// add to [scoreTimeSecList]
			const double ans = clockTime.back().sec + deltaClockTime;
			clockTime.emplace_back(
				ans, ScoreTime(*endScoreTime)
			);


			begBarLen = endBarLen;
		}


		// update member variable
		isReady = true;
		lastBeat = beat.back().beat;
		lastTempo = tempo.back().tempo;
		
		return true;
	}

	double ScoreTimeConverter::calcSec(const math::Fraction &barLen) {
		if (!isReady)
			return 0.0;

		// bar length before convert
		const math::Fraction &srcBarLen = barLen;

		math::Fraction deltaBarLen;
		math::Fraction barLenBeg;
		double deltaClockTime;
		double clockTimeBeg;


		if (srcBarLen < clockTime.back().getBarLength()) {
			// within clock time list

			// calculate delta time between two the clock times
			// get the clock time at previous [srcBarLen]

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
			const double extraClockTime = deltaClockTime * (extraBarLen/deltaBarLen).to_f();

			return clockTimeBeg + extraClockTime;

		} else {
			// outside clock time list

			clockTimeBeg = clockTime.back().sec;
			barLenBeg = clockTime.back().getBarLength();
			deltaBarLen = srcBarLen - barLenBeg;

			const double extraClockTime = calcClockTimeLen(lastBeat, lastTempo, deltaBarLen);

			return clockTimeBeg + extraClockTime;
		}

	}

	void ScoreTimeConverter::init() {
		clockTime.clear();
		lastBeat = 0;
		lastTempo = 0;
		isReady = false;
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

	double ScoreTimeConverter::calcClockTimeLen(const math::Fraction &beat, double tempo, const math::Fraction &barLen) {
		const double oneBeatLength = 60.0 / tempo;
		const double numofBeats = 4 * beat.to_f();  // the number of quarter notes in a bar
		const double ClockTimeLen = oneBeatLength * numofBeats * barLen.to_f();

		return ClockTimeLen;
	}



}