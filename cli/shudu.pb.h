// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: shudu.proto

#ifndef PROTOBUF_INCLUDED_shudu_2eproto
#define PROTOBUF_INCLUDED_shudu_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_shudu_2eproto 

namespace protobuf_shudu_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_shudu_2eproto
namespace proto {
class echo_data;
class echo_dataDefaultTypeInternal;
extern echo_dataDefaultTypeInternal _echo_data_default_instance_;
class shudu_data;
class shudu_dataDefaultTypeInternal;
extern shudu_dataDefaultTypeInternal _shudu_data_default_instance_;
}  // namespace proto
namespace google {
namespace protobuf {
template<> ::proto::echo_data* Arena::CreateMaybeMessage<::proto::echo_data>(Arena*);
template<> ::proto::shudu_data* Arena::CreateMaybeMessage<::proto::shudu_data>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace proto {

// ===================================================================

class shudu_data : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.shudu_data) */ {
 public:
  shudu_data();
  virtual ~shudu_data();

  shudu_data(const shudu_data& from);

  inline shudu_data& operator=(const shudu_data& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  shudu_data(shudu_data&& from) noexcept
    : shudu_data() {
    *this = ::std::move(from);
  }

  inline shudu_data& operator=(shudu_data&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const shudu_data& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const shudu_data* internal_default_instance() {
    return reinterpret_cast<const shudu_data*>(
               &_shudu_data_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(shudu_data* other);
  friend void swap(shudu_data& a, shudu_data& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline shudu_data* New() const final {
    return CreateMaybeMessage<shudu_data>(NULL);
  }

  shudu_data* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<shudu_data>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const shudu_data& from);
  void MergeFrom(const shudu_data& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(shudu_data* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string data = 1;
  void clear_data();
  static const int kDataFieldNumber = 1;
  const ::std::string& data() const;
  void set_data(const ::std::string& value);
  #if LANG_CXX11
  void set_data(::std::string&& value);
  #endif
  void set_data(const char* value);
  void set_data(const char* value, size_t size);
  ::std::string* mutable_data();
  ::std::string* release_data();
  void set_allocated_data(::std::string* data);

  // string time = 2;
  void clear_time();
  static const int kTimeFieldNumber = 2;
  const ::std::string& time() const;
  void set_time(const ::std::string& value);
  #if LANG_CXX11
  void set_time(::std::string&& value);
  #endif
  void set_time(const char* value);
  void set_time(const char* value, size_t size);
  ::std::string* mutable_time();
  ::std::string* release_time();
  void set_allocated_time(::std::string* time);

  // @@protoc_insertion_point(class_scope:proto.shudu_data)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr data_;
  ::google::protobuf::internal::ArenaStringPtr time_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_shudu_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class echo_data : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.echo_data) */ {
 public:
  echo_data();
  virtual ~echo_data();

  echo_data(const echo_data& from);

  inline echo_data& operator=(const echo_data& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  echo_data(echo_data&& from) noexcept
    : echo_data() {
    *this = ::std::move(from);
  }

  inline echo_data& operator=(echo_data&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const echo_data& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const echo_data* internal_default_instance() {
    return reinterpret_cast<const echo_data*>(
               &_echo_data_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(echo_data* other);
  friend void swap(echo_data& a, echo_data& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline echo_data* New() const final {
    return CreateMaybeMessage<echo_data>(NULL);
  }

  echo_data* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<echo_data>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const echo_data& from);
  void MergeFrom(const echo_data& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(echo_data* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string data = 1;
  void clear_data();
  static const int kDataFieldNumber = 1;
  const ::std::string& data() const;
  void set_data(const ::std::string& value);
  #if LANG_CXX11
  void set_data(::std::string&& value);
  #endif
  void set_data(const char* value);
  void set_data(const char* value, size_t size);
  ::std::string* mutable_data();
  ::std::string* release_data();
  void set_allocated_data(::std::string* data);

  // @@protoc_insertion_point(class_scope:proto.echo_data)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr data_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_shudu_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// shudu_data

// string data = 1;
inline void shudu_data::clear_data() {
  data_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& shudu_data::data() const {
  // @@protoc_insertion_point(field_get:proto.shudu_data.data)
  return data_.GetNoArena();
}
inline void shudu_data::set_data(const ::std::string& value) {
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:proto.shudu_data.data)
}
#if LANG_CXX11
inline void shudu_data::set_data(::std::string&& value) {
  
  data_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:proto.shudu_data.data)
}
#endif
inline void shudu_data::set_data(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:proto.shudu_data.data)
}
inline void shudu_data::set_data(const char* value, size_t size) {
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:proto.shudu_data.data)
}
inline ::std::string* shudu_data::mutable_data() {
  
  // @@protoc_insertion_point(field_mutable:proto.shudu_data.data)
  return data_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* shudu_data::release_data() {
  // @@protoc_insertion_point(field_release:proto.shudu_data.data)
  
  return data_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void shudu_data::set_allocated_data(::std::string* data) {
  if (data != NULL) {
    
  } else {
    
  }
  data_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), data);
  // @@protoc_insertion_point(field_set_allocated:proto.shudu_data.data)
}

// string time = 2;
inline void shudu_data::clear_time() {
  time_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& shudu_data::time() const {
  // @@protoc_insertion_point(field_get:proto.shudu_data.time)
  return time_.GetNoArena();
}
inline void shudu_data::set_time(const ::std::string& value) {
  
  time_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:proto.shudu_data.time)
}
#if LANG_CXX11
inline void shudu_data::set_time(::std::string&& value) {
  
  time_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:proto.shudu_data.time)
}
#endif
inline void shudu_data::set_time(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  time_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:proto.shudu_data.time)
}
inline void shudu_data::set_time(const char* value, size_t size) {
  
  time_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:proto.shudu_data.time)
}
inline ::std::string* shudu_data::mutable_time() {
  
  // @@protoc_insertion_point(field_mutable:proto.shudu_data.time)
  return time_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* shudu_data::release_time() {
  // @@protoc_insertion_point(field_release:proto.shudu_data.time)
  
  return time_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void shudu_data::set_allocated_time(::std::string* time) {
  if (time != NULL) {
    
  } else {
    
  }
  time_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), time);
  // @@protoc_insertion_point(field_set_allocated:proto.shudu_data.time)
}

// -------------------------------------------------------------------

// echo_data

// string data = 1;
inline void echo_data::clear_data() {
  data_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& echo_data::data() const {
  // @@protoc_insertion_point(field_get:proto.echo_data.data)
  return data_.GetNoArena();
}
inline void echo_data::set_data(const ::std::string& value) {
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:proto.echo_data.data)
}
#if LANG_CXX11
inline void echo_data::set_data(::std::string&& value) {
  
  data_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:proto.echo_data.data)
}
#endif
inline void echo_data::set_data(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:proto.echo_data.data)
}
inline void echo_data::set_data(const char* value, size_t size) {
  
  data_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:proto.echo_data.data)
}
inline ::std::string* echo_data::mutable_data() {
  
  // @@protoc_insertion_point(field_mutable:proto.echo_data.data)
  return data_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* echo_data::release_data() {
  // @@protoc_insertion_point(field_release:proto.echo_data.data)
  
  return data_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void echo_data::set_allocated_data(::std::string* data) {
  if (data != NULL) {
    
  } else {
    
  }
  data_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), data);
  // @@protoc_insertion_point(field_set_allocated:proto.echo_data.data)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_shudu_2eproto