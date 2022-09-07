; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@2 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@4 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@5 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@6 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@7 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@8 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@9 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %0 = alloca %_s__String__charptr_long_long, align 8
  %1 = alloca ptr, align 8
  %2 = alloca %_s__String__charptr_long_long, align 8
  %3 = alloca ptr, align 8
  %4 = alloca %_s__String__charptr_long_long, align 8
  %5 = alloca ptr, align 8
  %6 = alloca %_s__String__charptr_long_long, align 8
  %7 = alloca ptr, align 8
  %8 = alloca %_s__String__charptr_long_long, align 8
  %9 = alloca ptr, align 8
  %10 = alloca %_s__String__charptr_long_long, align 8
  %11 = alloca ptr, align 8
  %s4 = alloca ptr, align 8
  %12 = alloca %_s__String__charptr_long_long, align 8
  %s5 = alloca ptr, align 8
  %13 = alloca %_s__String__charptr_long_long, align 8
  %s6 = alloca ptr, align 8
  %14 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__ctor__string_string(ptr nonnull %0, ptr nonnull @1, ptr nonnull @0)
  store ptr %0, ptr %1, align 8
  %15 = call ptr @_mf__String__getRaw(ptr nonnull %1)
  %.unpack = load ptr, ptr %15, align 8
  %16 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack, 0
  %.elt6 = getelementptr inbounds %_s__String__charptr_long_long, ptr %15, i64 0, i32 1
  %.unpack7 = load i64, ptr %.elt6, align 8
  %17 = insertvalue %_s__String__charptr_long_long %16, i64 %.unpack7, 1
  %.elt8 = getelementptr inbounds %_s__String__charptr_long_long, ptr %15, i64 0, i32 2
  %.unpack9 = load i64, ptr %.elt8, align 8
  %18 = insertvalue %_s__String__charptr_long_long %17, i64 %.unpack9, 2
  %19 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %18)
  call void @_mp__String__ctor__string_string(ptr nonnull %2, ptr nonnull @1, ptr nonnull @0)
  store ptr %2, ptr %3, align 8
  %20 = call ptr @_mf__String__getRaw(ptr nonnull %3)
  %.unpack10 = load ptr, ptr %20, align 8
  %21 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack10, 0
  %.elt11 = getelementptr inbounds %_s__String__charptr_long_long, ptr %20, i64 0, i32 1
  %.unpack12 = load i64, ptr %.elt11, align 8
  %22 = insertvalue %_s__String__charptr_long_long %21, i64 %.unpack12, 1
  %.elt13 = getelementptr inbounds %_s__String__charptr_long_long, ptr %20, i64 0, i32 2
  %.unpack14 = load i64, ptr %.elt13, align 8
  %23 = insertvalue %_s__String__charptr_long_long %22, i64 %.unpack14, 2
  %24 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %23)
  call void @_mp__String__ctor__string(ptr nonnull %4, ptr nonnull @9)
  call void @_mp__String__opMul__short(ptr nonnull %4, i16 4)
  store ptr %4, ptr %5, align 8
  %25 = call ptr @_mf__String__getRaw(ptr nonnull %5)
  %.unpack15 = load ptr, ptr %25, align 8
  %26 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack15, 0
  %.elt16 = getelementptr inbounds %_s__String__charptr_long_long, ptr %25, i64 0, i32 1
  %.unpack17 = load i64, ptr %.elt16, align 8
  %27 = insertvalue %_s__String__charptr_long_long %26, i64 %.unpack17, 1
  %.elt18 = getelementptr inbounds %_s__String__charptr_long_long, ptr %25, i64 0, i32 2
  %.unpack19 = load i64, ptr %.elt18, align 8
  %28 = insertvalue %_s__String__charptr_long_long %27, i64 %.unpack19, 2
  %29 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %28)
  call void @_mp__String__ctor__string(ptr nonnull %6, ptr nonnull @1)
  call void @_mp__String__opMul__int(ptr nonnull %6, i32 5)
  store ptr %6, ptr %7, align 8
  %30 = call ptr @_mf__String__getRaw(ptr nonnull %7)
  %.unpack20 = load ptr, ptr %30, align 8
  %31 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack20, 0
  %.elt21 = getelementptr inbounds %_s__String__charptr_long_long, ptr %30, i64 0, i32 1
  %.unpack22 = load i64, ptr %.elt21, align 8
  %32 = insertvalue %_s__String__charptr_long_long %31, i64 %.unpack22, 1
  %.elt23 = getelementptr inbounds %_s__String__charptr_long_long, ptr %30, i64 0, i32 2
  %.unpack24 = load i64, ptr %.elt23, align 8
  %33 = insertvalue %_s__String__charptr_long_long %32, i64 %.unpack24, 2
  %34 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %33)
  call void @_mp__String__ctor__char(ptr nonnull %8, i8 97)
  call void @_mp__String__opMul__int(ptr nonnull %8, i32 20)
  store ptr %8, ptr %9, align 8
  %35 = call ptr @_mf__String__getRaw(ptr nonnull %9)
  %.unpack25 = load ptr, ptr %35, align 8
  %36 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack25, 0
  %.elt26 = getelementptr inbounds %_s__String__charptr_long_long, ptr %35, i64 0, i32 1
  %.unpack27 = load i64, ptr %.elt26, align 8
  %37 = insertvalue %_s__String__charptr_long_long %36, i64 %.unpack27, 1
  %.elt28 = getelementptr inbounds %_s__String__charptr_long_long, ptr %35, i64 0, i32 2
  %.unpack29 = load i64, ptr %.elt28, align 8
  %38 = insertvalue %_s__String__charptr_long_long %37, i64 %.unpack29, 2
  %39 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %38)
  call void @_mp__String__ctor__char(ptr nonnull %10, i8 99)
  call void @_mp__String__opMul__int(ptr nonnull %10, i32 2)
  call void @_mp__String__opMul__int(ptr nonnull %10, i32 7)
  store ptr %10, ptr %11, align 8
  %40 = call ptr @_mf__String__getRaw(ptr nonnull %11)
  %.unpack30 = load ptr, ptr %40, align 8
  %41 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack30, 0
  %.elt31 = getelementptr inbounds %_s__String__charptr_long_long, ptr %40, i64 0, i32 1
  %.unpack32 = load i64, ptr %.elt31, align 8
  %42 = insertvalue %_s__String__charptr_long_long %41, i64 %.unpack32, 1
  %.elt33 = getelementptr inbounds %_s__String__charptr_long_long, ptr %40, i64 0, i32 2
  %.unpack34 = load i64, ptr %.elt33, align 8
  %43 = insertvalue %_s__String__charptr_long_long %42, i64 %.unpack34, 2
  %44 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %43)
  %45 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 zext (i1 icmp eq (ptr @3, ptr @4) to i32))
  %46 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 zext (i1 icmp eq (ptr @7, ptr @5) to i32))
  %47 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 1)
  %48 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 zext (i1 icmp ne (ptr @3, ptr @4) to i32))
  %49 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 zext (i1 icmp ne (ptr @7, ptr @5) to i32))
  %50 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 0)
  call void @_mp__String__ctor__string(ptr nonnull %12, ptr nonnull @7)
  call void @_mp__String__append__char(ptr nonnull %12, i8 108)
  store ptr %12, ptr %s4, align 8
  %51 = call ptr @_mf__String__getRaw(ptr nonnull %s4)
  %.unpack35 = load ptr, ptr %51, align 8
  %52 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack35, 0
  %.elt36 = getelementptr inbounds %_s__String__charptr_long_long, ptr %51, i64 0, i32 1
  %.unpack37 = load i64, ptr %.elt36, align 8
  %53 = insertvalue %_s__String__charptr_long_long %52, i64 %.unpack37, 1
  %.elt38 = getelementptr inbounds %_s__String__charptr_long_long, ptr %51, i64 0, i32 2
  %.unpack39 = load i64, ptr %.elt38, align 8
  %54 = insertvalue %_s__String__charptr_long_long %53, i64 %.unpack39, 2
  %55 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %54)
  call void @_mp__String__ctor__string(ptr nonnull %13, ptr nonnull @9)
  call void @_mp__String__append__string(ptr nonnull %13, ptr nonnull @8)
  store ptr %13, ptr %s5, align 8
  %56 = call ptr @_mf__String__getRaw(ptr nonnull %s5)
  %.unpack40 = load ptr, ptr %56, align 8
  %57 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack40, 0
  %.elt41 = getelementptr inbounds %_s__String__charptr_long_long, ptr %56, i64 0, i32 1
  %.unpack42 = load i64, ptr %.elt41, align 8
  %58 = insertvalue %_s__String__charptr_long_long %57, i64 %.unpack42, 1
  %.elt43 = getelementptr inbounds %_s__String__charptr_long_long, ptr %56, i64 0, i32 2
  %.unpack44 = load i64, ptr %.elt43, align 8
  %59 = insertvalue %_s__String__charptr_long_long %58, i64 %.unpack44, 2
  %60 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %59)
  call void @_mp__String__ctor__string(ptr nonnull %14, ptr nonnull @9)
  call void @_mp__String__opMul__int(ptr nonnull %14, i32 3)
  store ptr %14, ptr %s6, align 8
  %61 = call ptr @_mf__String__getRaw(ptr nonnull %s6)
  %.unpack45 = load ptr, ptr %61, align 8
  %62 = insertvalue %_s__String__charptr_long_long undef, ptr %.unpack45, 0
  %.elt46 = getelementptr inbounds %_s__String__charptr_long_long, ptr %61, i64 0, i32 1
  %.unpack47 = load i64, ptr %.elt46, align 8
  %63 = insertvalue %_s__String__charptr_long_long %62, i64 %.unpack47, 1
  %.elt48 = getelementptr inbounds %_s__String__charptr_long_long, ptr %61, i64 0, i32 2
  %.unpack49 = load i64, ptr %.elt48, align 8
  %64 = insertvalue %_s__String__charptr_long_long %63, i64 %.unpack49, 2
  %65 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long %64)
  call void @_mp__String__dtor(ptr nonnull %3)
  call void @_mp__String__dtor(ptr nonnull %7)
  call void @_mp__String__dtor(ptr nonnull %11)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare void @_mp__String__ctor__string_string(ptr, ptr, ptr) local_unnamed_addr

declare ptr @_mf__String__getRaw(ptr) local_unnamed_addr

declare void @_mp__String__ctor__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__opMul__short(ptr, i16) local_unnamed_addr

declare void @_mp__String__opMul__int(ptr, i32) local_unnamed_addr

declare void @_mp__String__ctor__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__append__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
