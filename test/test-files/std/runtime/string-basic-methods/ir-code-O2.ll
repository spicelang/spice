; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [7 x i8] c"Hello!\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"ell\00", align 1
@3 = private unnamed_addr constant [5 x i8] c"Wort\00", align 1
@4 = private unnamed_addr constant [2 x i8] c"H\00", align 1
@5 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@7 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@8 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@9 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@11 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
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
  call void @_mp__String__void__ctor__string(ptr nonnull %0, ptr nonnull @0)
  %17 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %0)
  %18 = zext i1 %17 to i32
  %19 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %18)
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @8)
  %20 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %1)
  %21 = zext i1 %20 to i32
  %22 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %21)
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @1)
  %23 = call i64 @_mf__String__long__getLength(ptr nonnull %2)
  %24 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %23)
  call void @_mp__String__void__ctor__string(ptr nonnull %3, ptr nonnull @11)
  %25 = call i64 @_mf__String__long__getLength(ptr nonnull %3)
  %26 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %25)
  call void @_mp__String__void__ctor__string(ptr nonnull %4, ptr nonnull @1)
  %27 = call i64 @_mf__String__long__getCapacity(ptr nonnull %4)
  %28 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %27)
  call void @_mp__String__void__ctor__string(ptr nonnull %5, ptr nonnull @11)
  %29 = call i64 @_mf__String__long__getCapacity(ptr nonnull %5)
  %30 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %29)
  call void @_mp__String__void__ctor__string(ptr nonnull %6, ptr nonnull @8)
  %31 = call i1 @_mf__String__bool__isFull(ptr nonnull %6)
  %32 = zext i1 %31 to i32
  %33 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %32)
  call void @_mp__String__void__ctor__string(ptr nonnull %7, ptr nonnull @11)
  %34 = call i1 @_mf__String__bool__isFull(ptr nonnull %7)
  %35 = zext i1 %34 to i32
  %36 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %35)
  call void @_mp__String__void__ctor__string(ptr nonnull %8, ptr nonnull @11)
  %37 = call i64 @_mf__String__long__find__string(ptr nonnull %8, ptr nonnull @2)
  %38 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %37)
  call void @_mp__String__void__ctor__string(ptr nonnull %9, ptr nonnull @11)
  %39 = call i64 @_mf__String__long__find__string(ptr nonnull %9, ptr nonnull @3)
  %40 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %39)
  call void @_mp__String__void__ctor__string(ptr nonnull %10, ptr nonnull @11)
  %41 = call i64 @_mf__String__long__find__string(ptr nonnull %10, ptr nonnull @4)
  %42 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %41)
  call void @_mp__String__void__ctor__string(ptr nonnull %11, ptr nonnull @11)
  %43 = call i64 @_mf__String__long__find__string(ptr nonnull %11, ptr nonnull @5)
  %44 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %43)
  call void @_mp__String__void__ctor__string(ptr nonnull %12, ptr nonnull @11)
  %45 = call i64 @_mf__String__long__find__string_int(ptr nonnull %12, ptr nonnull @6, i32 12)
  %46 = call i32 (ptr, ...) @printf(ptr nonnull @10, i64 %45)
  call void @_mp__String__void__ctor__string(ptr nonnull %13, ptr nonnull @11)
  %47 = call i1 @_mf__String__bool__contains__string(ptr nonnull %13, ptr nonnull @7)
  %48 = zext i1 %47 to i32
  %49 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %48)
  call void @_mp__String__void__ctor__string(ptr nonnull %14, ptr nonnull @11)
  %50 = call i1 @_mf__String__bool__contains__string(ptr nonnull %14, ptr nonnull @8)
  %51 = zext i1 %50 to i32
  %52 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %51)
  call void @_mp__String__void__ctor__string(ptr nonnull %15, ptr nonnull @11)
  %53 = call i1 @_mf__String__bool__contains__string(ptr nonnull %15, ptr nonnull @9)
  %54 = zext i1 %53 to i32
  %55 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %54)
  call void @_mp__String__void__ctor__string(ptr nonnull %16, ptr nonnull @11)
  %56 = call i1 @_mf__String__bool__contains__string(ptr nonnull %16, ptr nonnull @12)
  %57 = zext i1 %56 to i32
  %58 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %57)
  call void @_mp__String__void__dtor(ptr nonnull %7)
  call void @_mp__String__void__dtor(ptr nonnull %8)
  call void @_mp__String__void__dtor(ptr nonnull %9)
  call void @_mp__String__void__dtor(ptr nonnull %10)
  call void @_mp__String__void__dtor(ptr nonnull %11)
  call void @_mp__String__void__dtor(ptr nonnull %12)
  call void @_mp__String__void__dtor(ptr nonnull %13)
  call void @_mp__String__void__dtor(ptr nonnull %14)
  call void @_mp__String__void__dtor(ptr nonnull %15)
  call void @_mp__String__void__dtor(ptr nonnull %16)
  call void @_mp__String__void__dtor(ptr nonnull %0)
  call void @_mp__String__void__dtor(ptr nonnull %1)
  call void @_mp__String__void__dtor(ptr nonnull %2)
  call void @_mp__String__void__dtor(ptr nonnull %3)
  call void @_mp__String__void__dtor(ptr nonnull %4)
  call void @_mp__String__void__dtor(ptr nonnull %5)
  call void @_mp__String__void__dtor(ptr nonnull %6)
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

declare void @_mp__String__void__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
