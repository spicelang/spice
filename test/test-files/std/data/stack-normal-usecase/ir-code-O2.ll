; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__int__Stack__intptr_long_long_int = type { i32*, i64, i64, i32 }

@0 = private unnamed_addr constant [16 x i8] c"Stack size: %d\0A\00", align 1
@1 = private unnamed_addr constant [20 x i8] c"Stack capacity: %d\0A\00", align 1
@2 = private unnamed_addr constant [18 x i8] c"Stack item 3: %d\0A\00", align 1
@3 = private unnamed_addr constant [18 x i8] c"Stack item 2: %d\0A\00", align 1
@4 = private unnamed_addr constant [18 x i8] c"Stack item 1: %d\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %s1 = alloca %_s__int__Stack__intptr_long_long_int, align 8
  call void @_mp__Stack_int__ctor(%_s__int__Stack__intptr_long_long_int* nonnull %s1)
  call void @_mp__Stack_int__push__int(%_s__int__Stack__intptr_long_long_int* nonnull %s1, i32 123)
  call void @_mp__Stack_int__push__int(%_s__int__Stack__intptr_long_long_int* nonnull %s1, i32 456)
  call void @_mp__Stack_int__push__int(%_s__int__Stack__intptr_long_long_int* nonnull %s1, i32 789)
  %0 = call i64 @_mf__Stack_int__getSize(%_s__int__Stack__intptr_long_long_int* nonnull %s1)
  %1 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([16 x i8], [16 x i8]* @0, i64 0, i64 0), i64 %0)
  %2 = call i64 @_mf__Stack_int__getCapacity(%_s__int__Stack__intptr_long_long_int* nonnull %s1)
  %3 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([20 x i8], [20 x i8]* @1, i64 0, i64 0), i64 %2)
  %4 = call i32 @_mf__Stack_int__pop(%_s__int__Stack__intptr_long_long_int* nonnull %s1)
  %5 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @2, i64 0, i64 0), i32 %4)
  %6 = call i32 @_mf__Stack_int__pop(%_s__int__Stack__intptr_long_long_int* nonnull %s1)
  %7 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @3, i64 0, i64 0), i32 %6)
  %8 = call i32 @_mf__Stack_int__pop(%_s__int__Stack__intptr_long_long_int* nonnull %s1)
  %9 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([18 x i8], [18 x i8]* @4, i64 0, i64 0), i32 %8)
  call void @_mp__Stack_int__dtor(%_s__int__Stack__intptr_long_long_int* nonnull %s1)
  ret i32 0
}

declare void @_mp__Stack_int__ctor(%_s__int__Stack__intptr_long_long_int*) local_unnamed_addr

declare void @_mp__Stack_int__push__int(%_s__int__Stack__intptr_long_long_int*, i32) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

declare i64 @_mf__Stack_int__getSize(%_s__int__Stack__intptr_long_long_int*) local_unnamed_addr

declare i64 @_mf__Stack_int__getCapacity(%_s__int__Stack__intptr_long_long_int*) local_unnamed_addr

declare i32 @_mf__Stack_int__pop(%_s__int__Stack__intptr_long_long_int*) local_unnamed_addr

declare void @_mp__Stack_int__dtor(%_s__int__Stack__intptr_long_long_int*) local_unnamed_addr

attributes #0 = { nofree nounwind }
