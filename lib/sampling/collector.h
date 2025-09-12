#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "sample.h"

namespace sampling {

class Collector {
private:
  const unsigned long intervalMs;
  unsigned long lastSampleTimeMs;

  const unsigned long getIntervalMs() const;

  unsigned long getLastSampleTimeMs() const;

  void setLastSampleTimeMs(const unsigned long lastSampleTimeMs);

public:
  Collector(unsigned long intervalMs);

  ~Collector();

  bool IsSamplingTimeReached();

  Sample Collect();
};

} // namespace sampling

#endif // COLLECTOR_H