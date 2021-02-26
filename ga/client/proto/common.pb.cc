// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "common.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace proto {
class VersionDefaultTypeInternal : public ::google::protobuf::internal::ExplicitlyConstructed<Version> {
} _Version_default_instance_;

namespace protobuf_common_2eproto {


void TableStruct::Shutdown() {
  _Version_default_instance_.Shutdown();
}

void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  _Version_default_instance_.DefaultConstruct();
}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
void AddDescriptorsImpl() {
  InitDefaults();
  ::google::protobuf::internal::OnShutdown(&TableStruct::Shutdown);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
#endif  // GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER

}  // namespace protobuf_common_2eproto

bool SessionType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 7:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Version::kMajorFieldNumber;
const int Version::kMinorFieldNumber;
const int Version::kPatchFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Version::Version()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_common_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:proto.Version)
}
Version::Version(const Version& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&major_, &from.major_,
    reinterpret_cast<char*>(&patch_) -
    reinterpret_cast<char*>(&major_) + sizeof(patch_));
  // @@protoc_insertion_point(copy_constructor:proto.Version)
}

void Version::SharedCtor() {
  ::memset(&major_, 0, reinterpret_cast<char*>(&patch_) -
    reinterpret_cast<char*>(&major_) + sizeof(patch_));
  _cached_size_ = 0;
}

Version::~Version() {
  // @@protoc_insertion_point(destructor:proto.Version)
  SharedDtor();
}

void Version::SharedDtor() {
}

void Version::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const Version& Version::default_instance() {
  protobuf_common_2eproto::InitDefaults();
  return *internal_default_instance();
}

Version* Version::New(::google::protobuf::Arena* arena) const {
  Version* n = new Version;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Version::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.Version)
  ::memset(&major_, 0, reinterpret_cast<char*>(&patch_) -
    reinterpret_cast<char*>(&major_) + sizeof(patch_));
}

bool Version::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:proto.Version)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // uint32 major = 1;
      case 1: {
        if (tag == 8u) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &major_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 minor = 2;
      case 2: {
        if (tag == 16u) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &minor_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 patch = 3;
      case 3: {
        if (tag == 24u) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &patch_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:proto.Version)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:proto.Version)
  return false;
#undef DO_
}

void Version::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:proto.Version)
  // uint32 major = 1;
  if (this->major() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->major(), output);
  }

  // uint32 minor = 2;
  if (this->minor() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->minor(), output);
  }

  // uint32 patch = 3;
  if (this->patch() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->patch(), output);
  }

  // @@protoc_insertion_point(serialize_end:proto.Version)
}

size_t Version::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.Version)
  size_t total_size = 0;

  // uint32 major = 1;
  if (this->major() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->major());
  }

  // uint32 minor = 2;
  if (this->minor() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->minor());
  }

  // uint32 patch = 3;
  if (this->patch() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->patch());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Version::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const Version*>(&from));
}

void Version::MergeFrom(const Version& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto.Version)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.major() != 0) {
    set_major(from.major());
  }
  if (from.minor() != 0) {
    set_minor(from.minor());
  }
  if (from.patch() != 0) {
    set_patch(from.patch());
  }
}

void Version::CopyFrom(const Version& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.Version)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Version::IsInitialized() const {
  return true;
}

void Version::Swap(Version* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Version::InternalSwap(Version* other) {
  std::swap(major_, other->major_);
  std::swap(minor_, other->minor_);
  std::swap(patch_, other->patch_);
  std::swap(_cached_size_, other->_cached_size_);
}

::std::string Version::GetTypeName() const {
  return "proto.Version";
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Version

// uint32 major = 1;
void Version::clear_major() {
  major_ = 0u;
}
::google::protobuf::uint32 Version::major() const {
  // @@protoc_insertion_point(field_get:proto.Version.major)
  return major_;
}
void Version::set_major(::google::protobuf::uint32 value) {
  
  major_ = value;
  // @@protoc_insertion_point(field_set:proto.Version.major)
}

// uint32 minor = 2;
void Version::clear_minor() {
  minor_ = 0u;
}
::google::protobuf::uint32 Version::minor() const {
  // @@protoc_insertion_point(field_get:proto.Version.minor)
  return minor_;
}
void Version::set_minor(::google::protobuf::uint32 value) {
  
  minor_ = value;
  // @@protoc_insertion_point(field_set:proto.Version.minor)
}

// uint32 patch = 3;
void Version::clear_patch() {
  patch_ = 0u;
}
::google::protobuf::uint32 Version::patch() const {
  // @@protoc_insertion_point(field_get:proto.Version.patch)
  return patch_;
}
void Version::set_patch(::google::protobuf::uint32 value) {
  
  patch_ = value;
  // @@protoc_insertion_point(field_set:proto.Version.patch)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)
