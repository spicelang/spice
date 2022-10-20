; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [7 x i8] c"Hello!\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"ell\00", align 1
@4 = private unnamed_addr constant [5 x i8] c"Wort\00", align 1
@5 = private unnamed_addr constant [2 x i8] c"H\00", align 1
@6 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@7 = private unnamed_addr constant [2 x i8] c" \00", align 1
@8 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@9 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@10 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@11 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@12 = private unnamed_addr constant [4 x i8] c"o W\00", align 1

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
  %30 = alloca %_s__String__charptr_long_long, align 8
  %31 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %0, ptr nonnull @0)
  %32 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %0)
  %33 = zext i1 %32 to i32
  %34 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %33)
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @9)
  %35 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %1)
  %36 = zext i1 %35 to i32
  %37 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %36)
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @1)
  %38 = call i64 @_mf__String__long__getLength(ptr nonnull %2)
  %39 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %38)
  call void @_mp__String__void__ctor__string(ptr nonnull %3, ptr nonnull @2)
  %40 = call i64 @_mf__String__long__getLength(ptr nonnull %3)
  %41 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %40)
  call void @_mp__String__void__ctor__string(ptr nonnull %4, ptr nonnull @1)
  %42 = call i64 @_mf__String__long__getCapacity(ptr nonnull %4)
  %43 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %42)
  call void @_mp__String__void__ctor__string(ptr nonnull %5, ptr nonnull @2)
  %44 = call i64 @_mf__String__long__getCapacity(ptr nonnull %5)
  %45 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %44)
  call void @_mp__String__void__ctor__string(ptr nonnull %6, ptr nonnull @9)
  %46 = call i1 @_mf__String__bool__isFull(ptr nonnull %6)
  %47 = zext i1 %46 to i32
  %48 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %47)
  call void @_mp__String__void__ctor__string(ptr nonnull %7, ptr nonnull @2)
  %49 = call i1 @_mf__String__bool__isFull(ptr nonnull %7)
  %50 = zext i1 %49 to i32
  %51 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %50)
  call void @_mp__String__void__ctor__string(ptr nonnull %8, ptr nonnull @2)
  %52 = call i64 @_mf__String__long__find__string(ptr nonnull %8, ptr nonnull @3)
  %53 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %52)
  call void @_mp__String__void__ctor__string(ptr nonnull %9, ptr nonnull @2)
  %54 = call i64 @_mf__String__long__find__string(ptr nonnull %9, ptr nonnull @4)
  %55 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %54)
  call void @_mp__String__void__ctor__string(ptr nonnull %10, ptr nonnull @2)
  %56 = call i64 @_mf__String__long__find__string(ptr nonnull %10, ptr nonnull @5)
  %57 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %56)
  call void @_mp__String__void__ctor__string(ptr nonnull %11, ptr nonnull @2)
  %58 = call i64 @_mf__String__long__find__string(ptr nonnull %11, ptr nonnull @6)
  %59 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %58)
  call void @_mp__String__void__ctor__string(ptr nonnull %12, ptr nonnull @2)
  %60 = call i64 @_mf__String__long__find__string_int(ptr nonnull %12, ptr nonnull @7, i32 12)
  %61 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %60)
  call void @_mp__String__void__ctor__string(ptr nonnull %13, ptr nonnull @2)
  %62 = call i1 @_mf__String__bool__contains__string(ptr nonnull %13, ptr nonnull @8)
  %63 = zext i1 %62 to i32
  %64 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %63)
  call void @_mp__String__void__ctor__string(ptr nonnull %14, ptr nonnull @2)
  %65 = call i1 @_mf__String__bool__contains__string(ptr nonnull %14, ptr nonnull @9)
  %66 = zext i1 %65 to i32
  %67 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %66)
  call void @_mp__String__void__ctor__string(ptr nonnull %15, ptr nonnull @2)
  %68 = call i1 @_mf__String__bool__contains__string(ptr nonnull %15, ptr nonnull @10)
  %69 = zext i1 %68 to i32
  %70 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %69)
  call void @_mp__String__void__ctor__string(ptr nonnull %16, ptr nonnull @2)
  %71 = call i1 @_mf__String__bool__contains__string(ptr nonnull %16, ptr nonnull @12)
  %72 = zext i1 %71 to i32
  %73 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %72)
  %putchar = call i32 @putchar(i32 10)
  call void @_mp__String__void__ctor__string(ptr nonnull %17, ptr nonnull @0)
  %74 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %17)
  %75 = zext i1 %74 to i32
  %76 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %75)
  call void @_mp__String__void__ctor__string(ptr nonnull %18, ptr nonnull @9)
  %77 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %18)
  %78 = zext i1 %77 to i32
  %79 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %78)
  call void @_mp__String__void__ctor__string(ptr nonnull %19, ptr nonnull @1)
  %80 = call i64 @_mf__String__long__getLength(ptr nonnull %19)
  %81 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %80)
  call void @_mp__String__void__ctor__string(ptr nonnull %20, ptr nonnull @2)
  %82 = call i64 @_mf__String__long__getLength(ptr nonnull %20)
  %83 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %82)
  call void @_mp__String__void__ctor__string(ptr nonnull %21, ptr nonnull @1)
  %84 = call i64 @_mf__String__long__getCapacity(ptr nonnull %21)
  %85 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %84)
  call void @_mp__String__void__ctor__string(ptr nonnull %22, ptr nonnull @2)
  %86 = call i64 @_mf__String__long__getCapacity(ptr nonnull %22)
  %87 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %86)
  call void @_mp__String__void__ctor__string(ptr nonnull %23, ptr nonnull @2)
  %88 = call i64 @_mf__String__long__find__string(ptr nonnull %23, ptr nonnull @3)
  %89 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %88)
  call void @_mp__String__void__ctor__string(ptr nonnull %24, ptr nonnull @2)
  %90 = call i64 @_mf__String__long__find__string(ptr nonnull %24, ptr nonnull @4)
  %91 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %90)
  call void @_mp__String__void__ctor__string(ptr nonnull %25, ptr nonnull @2)
  %92 = call i64 @_mf__String__long__find__string(ptr nonnull %25, ptr nonnull @5)
  %93 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %92)
  call void @_mp__String__void__ctor__string(ptr nonnull %26, ptr nonnull @2)
  %94 = call i64 @_mf__String__long__find__string(ptr nonnull %26, ptr nonnull @6)
  %95 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %94)
  call void @_mp__String__void__ctor__string(ptr nonnull %27, ptr nonnull @2)
  %96 = call i64 @_mf__String__long__find__string_int(ptr nonnull %27, ptr nonnull @7, i32 12)
  %97 = call i32 (ptr, ...) @printf(ptr nonnull @11, i64 %96)
  call void @_mp__String__void__ctor__string(ptr nonnull %28, ptr nonnull @2)
  %98 = call i1 @_mf__String__bool__contains__string(ptr nonnull %28, ptr nonnull @8)
  %99 = zext i1 %98 to i32
  %100 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %99)
  call void @_mp__String__void__ctor__string(ptr nonnull %29, ptr nonnull @2)
  %101 = call i1 @_mf__String__bool__contains__string(ptr nonnull %29, ptr nonnull @9)
  %102 = zext i1 %101 to i32
  %103 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %102)
  call void @_mp__String__void__ctor__string(ptr nonnull %30, ptr nonnull @2)
  %104 = call i1 @_mf__String__bool__contains__string(ptr nonnull %30, ptr nonnull @10)
  %105 = zext i1 %104 to i32
  %106 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %105)
  call void @_mp__String__void__ctor__string(ptr nonnull %31, ptr nonnull @2)
  %107 = call i1 @_mf__String__bool__contains__string(ptr nonnull %31, ptr nonnull @12)
  %108 = zext i1 %107 to i32
  %109 = call i32 (ptr, ...) @printf(ptr nonnull @11, i32 %108)
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
  call void @_mp__String__dtor(ptr nonnull %22)
  call void @_mp__String__dtor(ptr nonnull %23)
  call void @_mp__String__dtor(ptr nonnull %24)
  call void @_mp__String__dtor(ptr nonnull %25)
  call void @_mp__String__dtor(ptr nonnull %26)
  call void @_mp__String__dtor(ptr nonnull %27)
  call void @_mp__String__dtor(ptr nonnull %28)
  call void @_mp__String__dtor(ptr nonnull %29)
  call void @_mp__String__dtor(ptr nonnull %30)
  call void @_mp__String__dtor(ptr nonnull %31)
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

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

declare i1 @_mf__String__bool__isEmpty(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getCapacity(ptr) local_unnamed_addr

declare i1 @_mf__String__bool__isFull(ptr) local_unnamed_addr

declare i64 @_mf__String__long__find__string(ptr, ptr) local_unnamed_addr

declare i64 @_mf__String__long__find__string_int(ptr, ptr, i32) local_unnamed_addr

declare i1 @_mf__String__bool__contains__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__dtor(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
