; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [7 x i8] c"Hello!\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@4 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@5 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@7 = private unnamed_addr constant [4 x i8] c"o W\00", align 1

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
  %14 = alloca %_s__String__charptr_long_long, align 8
  %15 = alloca %_s__String__charptr_long_long, align 8
  %16 = alloca %_s__String__charptr_long_long, align 8
  %17 = alloca %_s__String__charptr_long_long, align 8
  %18 = alloca %_s__String__charptr_long_long, align 8
  %19 = alloca %_s__String__charptr_long_long, align 8
  %20 = alloca %_s__String__charptr_long_long, align 8
  %21 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__ctor__string(ptr nonnull %0, ptr nonnull @0)
  %22 = call i1 @_mf__String__isEmpty(ptr nonnull %0)
  %23 = zext i1 %22 to i32
  %24 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %23)
  call void @_mp__String__ctor__string(ptr nonnull %1, ptr nonnull @4)
  %25 = call i1 @_mf__String__isEmpty(ptr nonnull %1)
  %26 = zext i1 %25 to i32
  %27 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %26)
  call void @_mp__String__ctor__string(ptr nonnull %2, ptr nonnull @1)
  %28 = call i64 @_mf__String__getLength(ptr nonnull %2)
  %29 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %28)
  call void @_mp__String__ctor__string(ptr nonnull %3, ptr nonnull @2)
  %30 = call i64 @_mf__String__getLength(ptr nonnull %3)
  %31 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %30)
  call void @_mp__String__ctor__string(ptr nonnull %4, ptr nonnull @1)
  %32 = call i64 @_mf__String__getCapacity(ptr nonnull %4)
  %33 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %32)
  call void @_mp__String__ctor__string(ptr nonnull %5, ptr nonnull @2)
  %34 = call i64 @_mf__String__getCapacity(ptr nonnull %5)
  %35 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %34)
  call void @_mp__String__ctor__string(ptr nonnull %6, ptr nonnull @4)
  %36 = call i1 @_mf__String__isFull(ptr nonnull %6)
  %37 = zext i1 %36 to i32
  %38 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %37)
  call void @_mp__String__ctor__string(ptr nonnull %7, ptr nonnull @2)
  %39 = call i1 @_mf__String__isFull(ptr nonnull %7)
  %40 = zext i1 %39 to i32
  %41 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %40)
  call void @_mp__String__ctor__string(ptr nonnull %8, ptr nonnull @2)
  %42 = call i1 @_mf__String__contains__string(ptr nonnull %8, ptr nonnull @3)
  %43 = zext i1 %42 to i32
  %44 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %43)
  call void @_mp__String__ctor__string(ptr nonnull %9, ptr nonnull @2)
  %45 = call i1 @_mf__String__contains__string(ptr nonnull %9, ptr nonnull @4)
  %46 = zext i1 %45 to i32
  %47 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %46)
  call void @_mp__String__ctor__string(ptr nonnull %10, ptr nonnull @2)
  %48 = call i1 @_mf__String__contains__string(ptr nonnull %10, ptr nonnull @5)
  %49 = zext i1 %48 to i32
  %50 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %49)
  call void @_mp__String__ctor__string(ptr nonnull %11, ptr nonnull @2)
  %51 = call i1 @_mf__String__contains__string(ptr nonnull %11, ptr nonnull @7)
  %52 = zext i1 %51 to i32
  %53 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %52)
  %putchar = call i32 @putchar(i32 10)
  call void @_mp__String__ctor__string(ptr nonnull %12, ptr nonnull @0)
  %54 = call i1 @_mf__String__isEmpty(ptr nonnull %12)
  %55 = zext i1 %54 to i32
  %56 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %55)
  call void @_mp__String__ctor__string(ptr nonnull %13, ptr nonnull @4)
  %57 = call i1 @_mf__String__isEmpty(ptr nonnull %13)
  %58 = zext i1 %57 to i32
  %59 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %58)
  call void @_mp__String__ctor__string(ptr nonnull %14, ptr nonnull @1)
  %60 = call i64 @_mf__String__getLength(ptr nonnull %14)
  %61 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %60)
  call void @_mp__String__ctor__string(ptr nonnull %15, ptr nonnull @2)
  %62 = call i64 @_mf__String__getLength(ptr nonnull %15)
  %63 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %62)
  call void @_mp__String__ctor__string(ptr nonnull %16, ptr nonnull @1)
  %64 = call i64 @_mf__String__getCapacity(ptr nonnull %16)
  %65 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %64)
  call void @_mp__String__ctor__string(ptr nonnull %17, ptr nonnull @2)
  %66 = call i64 @_mf__String__getCapacity(ptr nonnull %17)
  %67 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %66)
  call void @_mp__String__ctor__string(ptr nonnull %18, ptr nonnull @2)
  %68 = call i1 @_mf__String__contains__string(ptr nonnull %18, ptr nonnull @3)
  %69 = zext i1 %68 to i32
  %70 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %69)
  call void @_mp__String__ctor__string(ptr nonnull %19, ptr nonnull @2)
  %71 = call i1 @_mf__String__contains__string(ptr nonnull %19, ptr nonnull @4)
  %72 = zext i1 %71 to i32
  %73 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %72)
  call void @_mp__String__ctor__string(ptr nonnull %20, ptr nonnull @2)
  %74 = call i1 @_mf__String__contains__string(ptr nonnull %20, ptr nonnull @5)
  %75 = zext i1 %74 to i32
  %76 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %75)
  call void @_mp__String__ctor__string(ptr nonnull %21, ptr nonnull @2)
  %77 = call i1 @_mf__String__contains__string(ptr nonnull %21, ptr nonnull @7)
  %78 = zext i1 %77 to i32
  %79 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %78)
  call void @_mp__String__dtor(ptr nonnull %7)
  call void @_mp__String__dtor(ptr nonnull %8)
  call void @_mp__String__dtor(ptr nonnull %9)
  call void @_mp__String__dtor(ptr nonnull %10)
  call void @_mp__String__dtor(ptr nonnull %11)
  call void @_mp__String__dtor(ptr nonnull %12)
  call void @_mp__String__dtor(ptr nonnull %13)
  call void @_mp__String__dtor(ptr nonnull %14)
  call void @_mp__String__dtor(ptr nonnull %15)
  call void @_mp__String__dtor(ptr nonnull %16)
  call void @_mp__String__dtor(ptr nonnull %17)
  call void @_mp__String__dtor(ptr nonnull %18)
  call void @_mp__String__dtor(ptr nonnull %19)
  call void @_mp__String__dtor(ptr nonnull %20)
  call void @_mp__String__dtor(ptr nonnull %21)
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

declare i1 @_mf__String__contains__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__dtor(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
