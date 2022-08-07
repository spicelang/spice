; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__string_int__Pair__string_int = type { i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@1 = private unnamed_addr constant [11 x i8] c"First: %s\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Second: %d\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %stringIntPair = alloca %_s__string_int__Pair__string_int, align 8
  call void @_mp__Pair_string_int__ctor__string_int(%_s__string_int__Pair__string_int* nonnull %stringIntPair, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i64 0, i64 0), i32 1234)
  %0 = call i8* @_mf__Pair_string_int__getFirst(%_s__string_int__Pair__string_int* nonnull %stringIntPair)
  %1 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([11 x i8], [11 x i8]* @1, i64 0, i64 0), i8* %0)
  %2 = call i32 @_mf__Pair_string_int__getSecond(%_s__string_int__Pair__string_int* nonnull %stringIntPair)
  %3 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([12 x i8], [12 x i8]* @2, i64 0, i64 0), i32 %2)
  ret i32 0
}

declare void @_mp__Pair_string_int__ctor__string_int(%_s__string_int__Pair__string_int*, i8*, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

declare i8* @_mf__Pair_string_int__getFirst(%_s__string_int__Pair__string_int*) local_unnamed_addr

declare i32 @_mf__Pair_string_int__getSecond(%_s__string_int__Pair__string_int*) local_unnamed_addr

attributes #0 = { nofree nounwind }
