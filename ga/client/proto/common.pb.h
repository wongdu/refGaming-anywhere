// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.proto

#ifndef PROTOBUF_common_2eproto__INCLUDED
#define PROTOBUF_common_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3002000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3002000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_util.h>
// @@protoc_insertion_point(includes)
namespace proto {
class Version;
class VersionDefaultTypeInternal;
extern VersionDefaultTypeInternal _Version_default_instance_;
}  // namespace proto

namespace proto {

namespace protobuf_common_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_common_2eproto

enum SessionType {
  SESSION_TYPE_UNKNOWN = 0,
  SESSION_TYPE_DESKTOP_MANAGE = 1,
  SESSION_TYPE_DESKTOP_VIEW = 2,
  SESSION_TYPE_FILE_TRANSFER = 4,
  SESSION_TYPE_ALL = 7,
  SessionType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SessionType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool SessionType_IsValid(int value);
const SessionType SessionType_MIN = SESSION_TYPE_UNKNOWN;
const SessionType SessionType_MAX = SESSION_TYPE_ALL;
const int SessionType_ARRAYSIZE = SessionType_MAX + 1;

// ===================================================================

class Version : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:proto.Version) */ {
 public:
  Version();
  virtual ~Version();

  Version(const Version& from);

  inline Version& operator=(const Version& from) {
    CopyFrom(from);
    return *this;
  }

  static const Version& default_instance();

  static inline const Version* internal_default_instance() {
    return reinterpret_cast<const Version*>(
               &_Version_default_instance_);
  }

  void Swap(Version* other);

  // implements Message ----------------------------------------------

  inline Version* New() const PROTOBUF_FINAL { return New(NULL); }

  Version* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const Version& from);
  void MergeFrom(const Version& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  void DiscardUnknownFields();
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Version* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::std::string GetTypeName() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // uint32 major = 1;
  void clear_major();
  static const int kMajorFieldNumber = 1;
  ::google::protobuf::uint32 major() const;
  void set_major(::google::protobuf::uint32 value);

  // uint32 minor = 2;
  void clear_minor();
  static const int kMinorFieldNumber = 2;
  ::google::protobuf::uint32 minor() const;
  void set_minor(::google::protobuf::uint32 value);

  // uint32 patch = 3;
  void clear_patch();
  static const int kPatchFieldNumber = 3;
  ::google::protobuf::uint32 patch() const;
  void set_patch(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:proto.Version)
 private:

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::uint32 major_;
  ::google::protobuf::uint32 minor_;
  ::google::protobuf::uint32 patch_;
  mutable int _cached_size_;
  friend struct  protobuf_common_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Version

// uint32 major = 1;
inline void Version::clear_major() {
  major_ = 0u;
}
inline ::google::protobuf::uint32 Version::major() const {
  // @@protoc_insertion_point(field_get:proto.Version.major)
  return major_;
}
inline void Version::set_major(::google::protobuf::uint32 value) {
  
  major_ = value;
  // @@protoc_insertion_point(field_set:proto.Version.major)
}

// uint32 minor = 2;
inline void Version::clear_minor() {
  minor_ = 0u;
}
inline ::google::protobuf::uint32 Version::minor() const {
  // @@protoc_insertion_point(field_get:proto.Version.minor)
  return minor_;
}
inline void Version::set_minor(::google::protobuf::uint32 value) {
  
  minor_ = value;
  // @@protoc_insertion_point(field_set:proto.Version.minor)
}

// uint32 patch = 3;
inline void Version::clear_patch() {
  patch_ = 0u;
}
inline ::google::protobuf::uint32 Version::patch() const {
  // @@protoc_insertion_point(field_get:proto.Version.patch)
  return patch_;
}
inline void Version::set_patch(::google::protobuf::uint32 value) {
  
  patch_ = value;
  // @@protoc_insertion_point(field_set:proto.Version.patch)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace proto

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::proto::SessionType> : ::google::protobuf::internal::true_type {};

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_common_2eproto__INCLUDED