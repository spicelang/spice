; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [28 x i8] c"Existing before create: %d\0A\00", align 1
@MODE_ALL_RWX = external local_unnamed_addr global i32
@1 = private unnamed_addr constant [23 x i8] c"mkDir return code: %d\0A\00", align 1
@2 = private unnamed_addr constant [27 x i8] c"Existing after create: %d\0A\00", align 1
@3 = private unnamed_addr constant [23 x i8] c"rmDir return code: %d\0A\00", align 1
@4 = private unnamed_addr constant [27 x i8] c"Existing after delete: %d\0A\00", align 1
@5 = private unnamed_addr constant [7 x i8] c"./test\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %0 = tail call i1 @_f__void__bool__dirExists__string(ptr nonnull @5)
  %1 = zext i1 %0 to i32
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i32 %1)
  %3 = load i32, ptr @MODE_ALL_RWX, align 4
  %4 = tail call i32 @_f__void__int__mkDir__string_int(ptr nonnull @5, i32 %3)
  %5 = tail call i32 (ptr, ...) @printf(ptr nonnull @1, i32 %4)
  %6 = tail call i1 @_f__void__bool__dirExists__string(ptr nonnull @5)
  %7 = zext i1 %6 to i32
  %8 = tail call i32 (ptr, ...) @printf(ptr nonnull @2, i32 %7)
  %9 = tail call i32 @_f__void__int__rmDir__string(ptr nonnull @5)
  %10 = tail call i32 (ptr, ...) @printf(ptr nonnull @3, i32 %9)
  %11 = tail call i1 @_f__void__bool__dirExists__string(ptr nonnull @5)
  %12 = zext i1 %11 to i32
  %13 = tail call i32 (ptr, ...) @printf(ptr nonnull @4, i32 %12)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare i1 @_f__void__bool__dirExists__string(ptr) local_unnamed_addr

declare i32 @_f__void__int__mkDir__string_int(ptr, i32) local_unnamed_addr

declare i32 @_f__void__int__rmDir__string(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
