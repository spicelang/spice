; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__string_int_bool__Triple__string_int_bool = type { ptr, i32, i1 }

@0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@1 = private unnamed_addr constant [11 x i8] c"First: %s\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Second: %d\0A\00", align 1
@3 = private unnamed_addr constant [11 x i8] c"Third: %d\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %stringIntBoolTriple = alloca %_s__string_int_bool__Triple__string_int_bool, align 8
  call void @_mp__Triple_string_int_bool__ctor__string_int_bool(ptr nonnull %stringIntBoolTriple, ptr nonnull @0, i32 1234, i1 true)
  %0 = call ptr @_mf__Triple_string_int_bool__getFirst(ptr nonnull %stringIntBoolTriple)
  %1 = call i32 (ptr, ...) @printf(ptr nonnull @1, ptr %0)
  %2 = call i32 @_mf__Triple_string_int_bool__getSecond(ptr nonnull %stringIntBoolTriple)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 %2)
  %4 = call i1 @_mf__Triple_string_int_bool__getThird(ptr nonnull %stringIntBoolTriple)
  %5 = zext i1 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr nonnull @3, i32 %5)
  ret i32 0
}

declare void @_mp__Triple_string_int_bool__ctor__string_int_bool(ptr, ptr, i32, i1) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare ptr @_mf__Triple_string_int_bool__getFirst(ptr) local_unnamed_addr

declare i32 @_mf__Triple_string_int_bool__getSecond(ptr) local_unnamed_addr

declare i1 @_mf__Triple_string_int_bool__getThird(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
