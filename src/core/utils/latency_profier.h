#ifndef NOCC_LATENCY_PROFILER
#define NOCC_LATENCY_PROFILER

namespace nocc {

#define NOCC_STATICS 1

  // usage: TODO

#if NOCC_STATICS == 1
  // Performance counting stats
  // To be more self-contained
  inline __attribute__ ((always_inline))
    uint64_t rdtsc(void)
  {
    uint32_t hi, lo;
    __asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)lo)|(((uint64_t)hi)<<32);
  }

#define LAT_VARS(X)  uint64_t _## X ##_lat_; uint64_t _pre_## X ##_lat_; \
  uint64_t _## X ##count_; uint64_t _pre_## X ##count_;
#define INIT_LAT_VARS(X) _## X ##_lat_= 0,_pre_## X ##_lat_ = 0,_## X ##count_ = 0,_pre_## X ##count_ = 0;
#define START(X) auto _## X ##start = rdtsc();
#define END(X) { _## X ##_lat_ += rdtsc() - _## X ##start;  \
    _## X ##count_    += 1;                                 \
  }
#define END_C(C,X) { C-> _## X ##_lat_ += rdtsc() - _## X ##start;  \
    C-> _## X ## count_ += 1;                                       \
  }

#define REPORT(X) { auto counts = _## X ##count_ - _pre_## X ##count_;  \
    _pre_## X ##count_ = _## X ##count_;                                \
    counts = counts == 0?1:counts;                                      \
    auto temp = _## X ##_lat_;                                          \
    fprintf(stdout,"%s lat %f\n",#X,(temp - _pre_## X ##_lat_) / (double)counts); \
    _pre_## X ##_lat_ = temp;                                           \
  }

#else
  // clear the counting stats to reduce performance impact
#define LAT_VARS(X) ;
#define INIT_LAT_VARS(X) ;
#define START(X) ;
#define END(X) ;
#define END_C(C,X);
#define REPORT(X) ;

#endif

} // namespace nocc

#endif
