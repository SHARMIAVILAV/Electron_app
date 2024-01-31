#include <napi.h>

namespace firstCpp{
  std::string hello();
  int add(int, int);
  Napi::String HelloWrapped(const Napi::CallbackInfo& info);
  Napi::Number AddWrapped(const Napi::CallbackInfo& info);

  Napi::Buffer<uint8_t> ConvertToGrayscale(const Napi::CallbackInfo& info);
  Napi::Object ConvertToGrayscaleWrapped(const Napi::CallbackInfo& info);

  Napi::Buffer<uint8_t> BBHE(const Napi::CallbackInfo& info);
  Napi::Object BBHEWrapped(const Napi::CallbackInfo& info);

  Napi::Buffer<uint8_t> DSIHE(const Napi::CallbackInfo& info);
  Napi::Object DSIHEWrapped(const Napi::CallbackInfo& info);

  Napi::Object Init(Napi::Env env, Napi::Object exports);
  // Napi::Buffer<uint8_t> ProcessImage(const Napi::Env& env, const Napi::Buffer<uint8_t>& inputBuffer);
}