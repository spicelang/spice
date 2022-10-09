; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"Hello!\00", align 1
@3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l1:
  %0 = alloca %_s__String__charptr_long_long, align 8
  %1 = alloca %_s__String__charptr_long_long, align 8
  %2 = alloca %_s__String__charptr_long_long, align 8
  %3 = alloca %_s__String__charptr_long_long, align 8
  %4 = alloca %_s__String__charptr_long_long, align 8
  %5 = alloca %_s__String__charptr_long_long, align 8
  %6 = alloca %_s__String__charptr_long_long, align 8
  %7 = alloca %_s__String__charptr_long_long, align 8
  %8 = alloca %_s__String__charptr_long_long, align 8
  %9 = alloca %_s__String__charptr_long_long, align 8
  %10 = alloca %_s__String__charptr_long_long, align 8
  %11 = alloca %_s__String__charptr_long_long, align 8
  %12 = alloca %_s__String__charptr_long_long, align 8
  %13 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__ctor__string(ptr nonnull %0, ptr nonnull @0)
  %14 = call i1 @_mf__String__isEmpty(ptr nonnull %0)
  %15 = zext i1 %14 to i32
  %16 = call i32 (ptr, ...) @printf(ptr nonnull @4, i32 %15)
  call void @_mp__String__ctor__string(ptr nonnull %1, ptr nonnull @1)
  %17 = call i1 @_mf__String__isEmpty(ptr nonnull %1)
  %18 = zext i1 %17 to i32
  %19 = call i32 (ptr, ...) @printf(ptr nonnull @4, i32 %18)
  call void @_mp__String__ctor__string(ptr nonnull %2, ptr nonnull @2)
  %20 = call i64 @_mf__String__getLength(ptr nonnull %2)
  %21 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %20)
  call void @_mp__String__ctor__string(ptr nonnull %3, ptr nonnull @3)
  %22 = call i64 @_mf__String__getLength(ptr nonnull %3)
  %23 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %22)
  call void @_mp__String__ctor__string(ptr nonnull %4, ptr nonnull @2)
  %24 = call i64 @_mf__String__getCapacity(ptr nonnull %4)
  %25 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %24)
  call void @_mp__String__ctor__string(ptr nonnull %5, ptr nonnull @3)
  %26 = call i64 @_mf__String__getCapacity(ptr nonnull %5)
  %27 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %26)
  call void @_mp__String__ctor__string(ptr nonnull %6, ptr nonnull @1)
  %28 = call i1 @_mf__String__isFull(ptr nonnull %6)
  %29 = zext i1 %28 to i32
  %30 = call i32 (ptr, ...) @printf(ptr nonnull @4, i32 %29)
  call void @_mp__String__ctor__string(ptr nonnull %7, ptr nonnull @3)
  %31 = call i1 @_mf__String__isFull(ptr nonnull %7)
  %32 = zext i1 %31 to i32
  %33 = call i32 (ptr, ...) @printf(ptr nonnull @4, i32 %32)
  call void @_mp__String__ctor__string(ptr nonnull %8, ptr nonnull @0)
  %34 = call i1 @_mf__String__isEmpty(ptr nonnull %8)
  %35 = zext i1 %34 to i32
  %36 = call i32 (ptr, ...) @printf(ptr nonnull @4, i32 %35)
  call void @_mp__String__ctor__string(ptr nonnull %9, ptr nonnull @1)
  %37 = call i1 @_mf__String__isEmpty(ptr nonnull %9)
  %38 = zext i1 %37 to i32
  %39 = call i32 (ptr, ...) @printf(ptr nonnull @4, i32 %38)
  call void @_mp__String__ctor__string(ptr nonnull %10, ptr nonnull @2)
  %40 = call i64 @_mf__String__getLength(ptr nonnull %10)
  %41 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %40)
  call void @_mp__String__ctor__string(ptr nonnull %11, ptr nonnull @3)
  %42 = call i64 @_mf__String__getLength(ptr nonnull %11)
  %43 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %42)
  call void @_mp__String__ctor__string(ptr nonnull %12, ptr nonnull @2)
  %44 = call i64 @_mf__String__getCapacity(ptr nonnull %12)
  %45 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %44)
  call void @_mp__String__ctor__string(ptr nonnull %13, ptr nonnull @3)
  %46 = call i64 @_mf__String__getCapacity(ptr nonnull %13)
  %47 = call i32 (ptr, ...) @printf(ptr nonnull @4, i64 %46)
  call void @_mp__String__dtor(ptr nonnull %7)
  call void @_mp__String__dtor(ptr nonnull %8)
  call void @_mp__String__dtor(ptr nonnull %9)
  call void @_mp__String__dtor(ptr nonnull %10)
  call void @_mp__String__dtor(ptr nonnull %11)
  call void @_mp__String__dtor(ptr nonnull %12)
  call void @_mp__String__dtor(ptr nonnull %13)
  call void @_mp__String__dtor(ptr nonnull %0)
  call void @_mp__String__dtor(ptr nonnull %1)
  call void @_mp__String__dtor(ptr nonnull %2)
  call void @_mp__String__dtor(ptr nonnull %3)
  call void @_mp__String__dtor(ptr nonnull %4)
  call void @_mp__String__dtor(ptr nonnull %5)
  call void @_mp__String__dtor(ptr nonnull %6)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare void @_mp__String__ctor__string(ptr, ptr) local_unnamed_addr

declare i1 @_mf__String__isEmpty(ptr) local_unnamed_addr

declare i64 @_mf__String__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__getCapacity(ptr) local_unnamed_addr

declare i1 @_mf__String__isFull(ptr) local_unnamed_addr

declare void @_mp__String__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
