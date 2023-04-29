#include "heapmon.h"
#include "esphome/core/log.h"

#ifdef USE_ESP_IDF
#include <esp_heap_caps.h>
#include <esp_system.h>
#endif

#ifdef USE_ARDUINO
#include <Esp.h>
#endif

namespace esphome {
namespace debug {

static const char *TAG = "heapmon";

void HeapMonitor::update() {
  uint32_t free_heap;
#ifdef USE_ARDUINO
  free_heap = ESP.getFreeHeap();
#elif defined(USE_ESP_IDF)
  free_heap = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
#endif
  
  // ESP.getFreeHeap() - returns total amount of available memory
  // ESP.getHeapFragmentation() - returns an indication of how badly the heap is fragmented. The bigger, the worse - over 50 is very bad.
  // ESP.getMaxFreeBlockSize() - the biggest block you can allocate.
  
  ESP_LOGD(TAG, "Free Heap Size: %u bytes", free_heap);
  this->publish_state(free_heap);
}

}  // namespace debug
}  // namespace esphome
