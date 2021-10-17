#include "SampleFilter.h"

static double filter_taps[SAMPLEFILTER_TAP_NUM] = {
  0.03754046486303462,
  -0.024882897624820828,
  -0.009980885754676696,
  0.011395768252876115,
  0.004282543611830531,
  0.00025100930923325755,
  0.017624187329702046,
  0.008378345289191099,
  -0.012193951660292869,
  0.0011557217200803224,
  0.0011691529869300153,
  -0.026563246949780267,
  -0.016913002236039535,
  -0.0006641392342236783,
  -0.02947500991799861,
  -0.03085432665880918,
  0.0002446749227497635,
  -0.024734795046311008,
  -0.04675148723929313,
  -0.005662853586812299,
  -0.01666886114606009,
  -0.06586061462641613,
  -0.02194399468172532,
  -0.0008680789517883956,
  -0.08300674801273705,
  -0.052421732678682956,
  0.03715933780642945,
  -0.09359759585218323,
  -0.14594793103670592,
  0.2556493426979563,
  0.5697230203297072,
  0.2556493426979563,
  -0.14594793103670592,
  -0.09359759585218323,
  0.03715933780642945,
  -0.052421732678682956,
  -0.08300674801273705,
  -0.0008680789517883956,
  -0.02194399468172532,
  -0.06586061462641613,
  -0.01666886114606009,
  -0.005662853586812299,
  -0.04675148723929313,
  -0.024734795046311008,
  0.0002446749227497635,
  -0.03085432665880918,
  -0.02947500991799861,
  -0.0006641392342236783,
  -0.016913002236039535,
  -0.026563246949780267,
  0.0011691529869300153,
  0.0011557217200803224,
  -0.012193951660292869,
  0.008378345289191099,
  0.017624187329702046,
  0.00025100930923325755,
  0.004282543611830531,
  0.011395768252876115,
  -0.009980885754676696,
  -0.024882897624820828,
  0.03754046486303462
};

void SampleFilter_init(SampleFilter* f) {
	int i;
	for (i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
		f->history[i] = 0;
	f->last_index = 0;
}

void SampleFilter_put(SampleFilter* f, double input) {
	f->history[f->last_index++] = input;
	if (f->last_index == SAMPLEFILTER_TAP_NUM)
		f->last_index = 0;
}

double SampleFilter_get(SampleFilter* f) {
	double acc = 0;
	int index = f->last_index, i;
	for (i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
		index = index != 0 ? index - 1 : SAMPLEFILTER_TAP_NUM - 1;
		acc += f->history[index] * filter_taps[i];
	};
	return acc;
}