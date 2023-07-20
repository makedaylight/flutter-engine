// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_ANDROID_ANDROID_SURFACETRANSACTION_H_
#define FLUTTER_SHELL_PLATFORM_ANDROID_ANDROID_SURFACETRANSACTION_H_

#include "flutter/fml/macros.h"

#include <cstdint>

namespace flutter {

//------------------------------------------------------------------------------
/// The Android SurfaceTransaction API is used by `AndroidSurface` to
/// synchronize frame presentation times. It's only available on API 29+.
///
class AndroidSurfaceTransaction {
 public:
  static AndroidSurfaceTransaction* GetInstance();

  explicit AndroidSurfaceTransaction();

  void PresentWithVsyncId(int64_t vsync_id);

 private:
  static AndroidSurfaceTransaction* instance_;
  typedef void ASurfaceTransaction;
  typedef AndroidSurfaceTransaction::ASurfaceTransaction* (
      *ASurfaceTransaction_create_FPN)();
  typedef void (*ASurfaceTransaction_setFrameTimeline_FPN)(
      AndroidSurfaceTransaction::ASurfaceTransaction* transaction,
      int64_t vsyncId);
  typedef void (*ASurfaceTransaction_apply_FPN)(
      AndroidSurfaceTransaction::ASurfaceTransaction* transaction);
  typedef void (*ASurfaceTransaction_delete_FPN)(
      AndroidSurfaceTransaction::ASurfaceTransaction* transaction);
  ASurfaceTransaction_create_FPN ASurfaceTransaction_create;
  ASurfaceTransaction_setFrameTimeline_FPN ASurfaceTransaction_setFrameTimeline;
  ASurfaceTransaction_apply_FPN ASurfaceTransaction_apply;
  ASurfaceTransaction_delete_FPN ASurfaceTransaction_delete;
  ASurfaceTransaction* pending_transaction_;

  FML_DISALLOW_COPY_AND_ASSIGN(AndroidSurfaceTransaction);
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_ANDROID_ANDROID_SURFACETRANSACTION_H_