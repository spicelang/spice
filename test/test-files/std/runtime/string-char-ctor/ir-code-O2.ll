; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [5 x i8] c"ello\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Length: %d\0A\00", align 1
@3 = private unnamed_addr constant [15 x i8] c"Capacity: %d\0A\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %s = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__ctor__char(ptr nonnull %s, i8 72)
  %0 = call ptr @_mf__String__getRaw(ptr nonnull %s)
  %1 = call i32 (ptr, ...) @printf(ptr nonnull @1, ptr %0)
  %2 = call i64 @_mf__String__getLength(ptr nonnull %s)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull @2, i64 %2)
  %4 = call i64 @_mf__String__getCapacity(ptr nonnull %s)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull @3, i64 %4)
  call void @_mp__String__append__string(ptr nonnull %s, ptr nonnull @0)
  %6 = call ptr @_mf__String__getRaw(ptr nonnull %s)
  %7 = call i32 (ptr, ...) @printf(ptr nonnull @1, ptr %6)
  %8 = call i64 @_mf__String__getLength(ptr nonnull %s)
  %9 = call i32 (ptr, ...) @printf(ptr nonnull @2, i64 %8)
  %10 = call i64 @_mf__String__getCapacity(ptr nonnull %s)
  %11 = call i32 (ptr, ...) @printf(ptr nonnull @3, i64 %10)
  ret i32 0
}

declare void @_mp__String__ctor__char(ptr, i8) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare ptr @_mf__String__getRaw(ptr) local_unnamed_addr

declare i64 @_mf__String__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__getCapacity(ptr) local_unnamed_addr

declare void @_mp__String__append__string(ptr, ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
