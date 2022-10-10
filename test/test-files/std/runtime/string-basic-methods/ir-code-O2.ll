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
@8 = private unnamed_addr constant [6 x i8] c"'%s'\0A\00", align 1

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
  %22 = alloca %_s__String__charptr_long_long, align 8
  %23 = alloca %_s__String__charptr_long_long, align 8
  %24 = alloca %_s__String__charptr_long_long, align 8
  %25 = alloca %_s__String__charptr_long_long, align 8
  %26 = alloca %_s__String__charptr_long_long, align 8
  %27 = alloca %_s__String__charptr_long_long, align 8
  %28 = alloca %_s__String__charptr_long_long, align 8
  %29 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__ctor__string(ptr nonnull %0, ptr nonnull @0)
  %30 = call i1 @_mf__String__isEmpty(ptr nonnull %0)
  %31 = zext i1 %30 to i32
  %32 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %31)
  call void @_mp__String__ctor__string(ptr nonnull %1, ptr nonnull @4)
  %33 = call i1 @_mf__String__isEmpty(ptr nonnull %1)
  %34 = zext i1 %33 to i32
  %35 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %34)
  call void @_mp__String__ctor__string(ptr nonnull %2, ptr nonnull @1)
  %36 = call i64 @_mf__String__getLength(ptr nonnull %2)
  %37 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %36)
  call void @_mp__String__ctor__string(ptr nonnull %3, ptr nonnull @2)
  %38 = call i64 @_mf__String__getLength(ptr nonnull %3)
  %39 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %38)
  call void @_mp__String__ctor__string(ptr nonnull %4, ptr nonnull @1)
  %40 = call i64 @_mf__String__getCapacity(ptr nonnull %4)
  %41 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %40)
  call void @_mp__String__ctor__string(ptr nonnull %5, ptr nonnull @2)
  %42 = call i64 @_mf__String__getCapacity(ptr nonnull %5)
  %43 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %42)
  call void @_mp__String__ctor__string(ptr nonnull %6, ptr nonnull @4)
  %44 = call i1 @_mf__String__isFull(ptr nonnull %6)
  %45 = zext i1 %44 to i32
  %46 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %45)
  call void @_mp__String__ctor__string(ptr nonnull %7, ptr nonnull @2)
  %47 = call i1 @_mf__String__isFull(ptr nonnull %7)
  %48 = zext i1 %47 to i32
  %49 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %48)
  call void @_mp__String__ctor__string(ptr nonnull %8, ptr nonnull @2)
  %50 = call i1 @_mf__String__contains__string(ptr nonnull %8, ptr nonnull @3)
  %51 = zext i1 %50 to i32
  %52 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %51)
  call void @_mp__String__ctor__string(ptr nonnull %9, ptr nonnull @2)
  %53 = call i1 @_mf__String__contains__string(ptr nonnull %9, ptr nonnull @4)
  %54 = zext i1 %53 to i32
  %55 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %54)
  call void @_mp__String__ctor__string(ptr nonnull %10, ptr nonnull @2)
  %56 = call i1 @_mf__String__contains__string(ptr nonnull %10, ptr nonnull @5)
  %57 = zext i1 %56 to i32
  %58 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %57)
  call void @_mp__String__ctor__string(ptr nonnull %11, ptr nonnull @2)
  %59 = call i1 @_mf__String__contains__string(ptr nonnull %11, ptr nonnull @7)
  %60 = zext i1 %59 to i32
  %61 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %60)
  call void @_mp__String__ctor__string(ptr nonnull %12, ptr nonnull @2)
  %62 = call ptr @_mf__String__substring__int_int(ptr nonnull %12, i32 0, i32 5)
  %63 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %62)
  call void @_mp__String__ctor__string(ptr nonnull %13, ptr nonnull @2)
  %64 = call ptr @_mf__String__substring__int_int(ptr nonnull %13, i32 4, i32 2)
  %65 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %64)
  call void @_mp__String__ctor__string(ptr nonnull %14, ptr nonnull @2)
  %66 = call ptr @_mf__String__substring__int(ptr nonnull %14, i32 6)
  %67 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %66)
  call void @_mp__String__ctor__string(ptr nonnull %15, ptr nonnull @2)
  %68 = call ptr @_mf__String__substring__int_int(ptr nonnull %15, i32 2, i32 0)
  %69 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %68)
  %putchar = call i32 @putchar(i32 10)
  call void @_mp__String__ctor__string(ptr nonnull %16, ptr nonnull @0)
  %70 = call i1 @_mf__String__isEmpty(ptr nonnull %16)
  %71 = zext i1 %70 to i32
  %72 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %71)
  call void @_mp__String__ctor__string(ptr nonnull %17, ptr nonnull @4)
  %73 = call i1 @_mf__String__isEmpty(ptr nonnull %17)
  %74 = zext i1 %73 to i32
  %75 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %74)
  call void @_mp__String__ctor__string(ptr nonnull %18, ptr nonnull @1)
  %76 = call i64 @_mf__String__getLength(ptr nonnull %18)
  %77 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %76)
  call void @_mp__String__ctor__string(ptr nonnull %19, ptr nonnull @2)
  %78 = call i64 @_mf__String__getLength(ptr nonnull %19)
  %79 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %78)
  call void @_mp__String__ctor__string(ptr nonnull %20, ptr nonnull @1)
  %80 = call i64 @_mf__String__getCapacity(ptr nonnull %20)
  %81 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %80)
  call void @_mp__String__ctor__string(ptr nonnull %21, ptr nonnull @2)
  %82 = call i64 @_mf__String__getCapacity(ptr nonnull %21)
  %83 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %82)
  call void @_mp__String__ctor__string(ptr nonnull %22, ptr nonnull @2)
  %84 = call i1 @_mf__String__contains__string(ptr nonnull %22, ptr nonnull @3)
  %85 = zext i1 %84 to i32
  %86 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %85)
  call void @_mp__String__ctor__string(ptr nonnull %23, ptr nonnull @2)
  %87 = call i1 @_mf__String__contains__string(ptr nonnull %23, ptr nonnull @4)
  %88 = zext i1 %87 to i32
  %89 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %88)
  call void @_mp__String__ctor__string(ptr nonnull %24, ptr nonnull @2)
  %90 = call i1 @_mf__String__contains__string(ptr nonnull %24, ptr nonnull @5)
  %91 = zext i1 %90 to i32
  %92 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %91)
  call void @_mp__String__ctor__string(ptr nonnull %25, ptr nonnull @2)
  %93 = call i1 @_mf__String__contains__string(ptr nonnull %25, ptr nonnull @7)
  %94 = zext i1 %93 to i32
  %95 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 %94)
  call void @_mp__String__ctor__string(ptr nonnull %26, ptr nonnull @2)
  %96 = call ptr @_mf__String__substring__int_int(ptr nonnull %26, i32 0, i32 5)
  %97 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %96)
  call void @_mp__String__ctor__string(ptr nonnull %27, ptr nonnull @2)
  %98 = call ptr @_mf__String__substring__int_int(ptr nonnull %27, i32 4, i32 2)
  %99 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %98)
  call void @_mp__String__ctor__string(ptr nonnull %28, ptr nonnull @2)
  %100 = call ptr @_mf__String__substring__int(ptr nonnull %28, i32 6)
  %101 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %100)
  call void @_mp__String__ctor__string(ptr nonnull %29, ptr nonnull @2)
  %102 = call ptr @_mf__String__substring__int_int(ptr nonnull %29, i32 4, i32 0)
  %103 = call i32 (ptr, ...) @printf(ptr nonnull @8, ptr %102)
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
  call void @_mp__String__dtor(ptr nonnull %22)
  call void @_mp__String__dtor(ptr nonnull %23)
  call void @_mp__String__dtor(ptr nonnull %24)
  call void @_mp__String__dtor(ptr nonnull %25)
  call void @_mp__String__dtor(ptr nonnull %0)
  call void @_mp__String__dtor(ptr nonnull %26)
  call void @_mp__String__dtor(ptr nonnull %27)
  call void @_mp__String__dtor(ptr nonnull %28)
  call void @_mp__String__dtor(ptr nonnull %29)
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

declare ptr @_mf__String__substring__int_int(ptr, i32, i32) local_unnamed_addr

declare ptr @_mf__String__substring__int(ptr, i32) local_unnamed_addr

declare void @_mp__String__dtor(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
