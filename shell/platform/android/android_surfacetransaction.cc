// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/android/android_surfacetransaction.h"

#include "flutter/fml/native_library.h"

namespace flutter {

AndroidSurfaceTransaction* AndroidSurfaceTransaction::instance_ = nullptr;

AndroidSurfaceTransaction* AndroidSurfaceTransaction::GetInstance() {
  if (instance_ == nullptr) {
    instance_ = new AndroidSurfaceTransaction();
  }
  return instance_;
}

AndroidSurfaceTransaction::AndroidSurfaceTransaction()
    : pending_transaction_(nullptr) {
  auto libandroid = fml::NativeLibrary::Create("libandroid.so");
  FML_DCHECK(libandroid);
  ASurfaceTransaction_create =
      libandroid
          ->ResolveFunction<ASurfaceTransaction_create_FPN>(
              "ASurfaceTransaction_create")
          .value_or(nullptr);
  if (ASurfaceTransaction_create) {
    ASurfaceTransaction_setFrameTimeline =
        libandroid
            ->ResolveFunction<ASurfaceTransaction_setFrameTimeline_FPN>(
                "ASurfaceTransaction_setFrameTimeline")
            .value_or(nullptr);
    ASurfaceTransaction_apply =
        libandroid
            ->ResolveFunction<ASurfaceTransaction_apply_FPN>(
                "ASurfaceTransaction_apply")
            .value_or(nullptr);
    ASurfaceTransaction_delete =
        libandroid
            ->ResolveFunction<ASurfaceTransaction_delete_FPN>(
                "ASurfaceTransaction_delete")
            .value_or(nullptr);
  }
}

void AndroidSurfaceTransaction::PresentWithVsyncId(int64_t vsync_id) {
  if (pending_transaction_) {
    if (vsync_id != 0) {
      ASurfaceTransaction_setFrameTimeline(pending_transaction_, vsync_id);
    }
    ASurfaceTransaction_apply(pending_transaction_);
    ASurfaceTransaction_delete(pending_transaction_);
  }

  if (ASurfaceTransaction_create) {
    pending_transaction_ = ASurfaceTransaction_create();
  }
}

}  // namespace flutter