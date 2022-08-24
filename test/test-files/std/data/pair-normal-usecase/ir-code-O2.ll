; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%_s__string_int__Pair__string_int = type { ptr, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@1 = private unnamed_addr constant [11 x i8] c"First: %s\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Second: %d\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %stringIntPair = alloca %_s__string_int__Pair__string_int, align 8
  call void @_mp__Pair_string_int__ctor__string_int(ptr nonnull %stringIntPair, ptr nonnull @0, i32 1234)
  %0 = call ptr @_mf__Pair_string_int__getFirst(ptr nonnull %stringIntPair)
  %1 = call i32 (ptr, ...) @printf(ptr nonnull @1, ptr %0)
  %2 = call i32 @_mf__Pair_string_int__getSecond(ptr nonnull %stringIntPair)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 %2)
  ret i32 0
}

declare void @_mp__Pair_string_int__ctor__string_int(ptr, ptr, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare ptr @_mf__Pair_string_int__getFirst(ptr) local_unnamed_addr

declare i32 @_mf__Pair_string_int__getSecond(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
