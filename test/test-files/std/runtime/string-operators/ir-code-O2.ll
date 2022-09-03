; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long.0 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.1 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.4 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.5 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.6 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.7 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.8 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.9 = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@2 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@4 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@5 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@6 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@7 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@8 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@9 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %0 = alloca %_s__String__charptr_long_long.0, align 8
  call void @_mp__String__ctor__string_string(ptr nonnull %0, ptr nonnull @0, ptr nonnull @1)
  %1 = alloca ptr, align 8
  store ptr %0, ptr %1, align 8
  %2 = call ptr @_mf__String__getRaw(ptr nonnull %1)
  %.unpack = load ptr, ptr %2, align 8
  %3 = insertvalue %_s__String__charptr_long_long.1 undef, ptr %.unpack, 0
  %.elt6 = getelementptr inbounds %_s__String__charptr_long_long.1, ptr %2, i64 0, i32 1
  %.unpack7 = load i64, ptr %.elt6, align 8
  %4 = insertvalue %_s__String__charptr_long_long.1 %3, i64 %.unpack7, 1
  %.elt8 = getelementptr inbounds %_s__String__charptr_long_long.1, ptr %2, i64 0, i32 2
  %.unpack9 = load i64, ptr %.elt8, align 8
  %5 = insertvalue %_s__String__charptr_long_long.1 %4, i64 %.unpack9, 2
  %6 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long.1 %5)
  %7 = alloca %_s__String__charptr_long_long.4, align 8
  call void @_mp__String__ctor__string_string(ptr nonnull %7, ptr nonnull @0, ptr nonnull @1)
  %8 = alloca ptr, align 8
  store ptr %7, ptr %8, align 8
  %9 = call ptr @_mf__String__getRaw(ptr nonnull %8)
  %.unpack10 = load ptr, ptr %9, align 8
  %10 = insertvalue %_s__String__charptr_long_long.5 undef, ptr %.unpack10, 0
  %.elt11 = getelementptr inbounds %_s__String__charptr_long_long.5, ptr %9, i64 0, i32 1
  %.unpack12 = load i64, ptr %.elt11, align 8
  %11 = insertvalue %_s__String__charptr_long_long.5 %10, i64 %.unpack12, 1
  %.elt13 = getelementptr inbounds %_s__String__charptr_long_long.5, ptr %9, i64 0, i32 2
  %.unpack14 = load i64, ptr %.elt13, align 8
  %12 = insertvalue %_s__String__charptr_long_long.5 %11, i64 %.unpack14, 2
  %13 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long.5 %12)
  %14 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 zext (i1 icmp eq (ptr @3, ptr @4) to i32))
  %15 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 zext (i1 icmp eq (ptr @7, ptr @5) to i32))
  %16 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 1)
  %17 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 zext (i1 icmp ne (ptr @3, ptr @4) to i32))
  %18 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 zext (i1 icmp ne (ptr @7, ptr @5) to i32))
  %19 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 0)
  %s2 = alloca ptr, align 8
  %20 = alloca %_s__String__charptr_long_long.6, align 8
  call void @_mp__String__ctor__string(ptr nonnull %20, ptr nonnull @7)
  call void @_mp__String__append__char(ptr nonnull %20, i8 108)
  store ptr %20, ptr %s2, align 8
  %21 = call ptr @_mf__String__getRaw(ptr nonnull %s2)
  %.unpack15 = load ptr, ptr %21, align 8
  %22 = insertvalue %_s__String__charptr_long_long.7 undef, ptr %.unpack15, 0
  %.elt16 = getelementptr inbounds %_s__String__charptr_long_long.7, ptr %21, i64 0, i32 1
  %.unpack17 = load i64, ptr %.elt16, align 8
  %23 = insertvalue %_s__String__charptr_long_long.7 %22, i64 %.unpack17, 1
  %.elt18 = getelementptr inbounds %_s__String__charptr_long_long.7, ptr %21, i64 0, i32 2
  %.unpack19 = load i64, ptr %.elt18, align 8
  %24 = insertvalue %_s__String__charptr_long_long.7 %23, i64 %.unpack19, 2
  %25 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long.7 %24)
  %s3 = alloca ptr, align 8
  %26 = alloca %_s__String__charptr_long_long.8, align 8
  call void @_mp__String__ctor__string(ptr nonnull %26, ptr nonnull @8)
  call void @_mp__String__append__string(ptr nonnull %26, ptr nonnull @9)
  store ptr %26, ptr %s3, align 8
  %27 = call ptr @_mf__String__getRaw(ptr nonnull %s3)
  %.unpack20 = load ptr, ptr %27, align 8
  %28 = insertvalue %_s__String__charptr_long_long.9 undef, ptr %.unpack20, 0
  %.elt21 = getelementptr inbounds %_s__String__charptr_long_long.9, ptr %27, i64 0, i32 1
  %.unpack22 = load i64, ptr %.elt21, align 8
  %29 = insertvalue %_s__String__charptr_long_long.9 %28, i64 %.unpack22, 1
  %.elt23 = getelementptr inbounds %_s__String__charptr_long_long.9, ptr %27, i64 0, i32 2
  %.unpack24 = load i64, ptr %.elt23, align 8
  %30 = insertvalue %_s__String__charptr_long_long.9 %29, i64 %.unpack24, 2
  %31 = call i32 (ptr, ...) @printf(ptr nonnull @10, %_s__String__charptr_long_long.9 %30)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare void @_mp__String__ctor__string_string(ptr, ptr, ptr) local_unnamed_addr

declare ptr @_mf__String__getRaw(ptr) local_unnamed_addr

declare void @_mp__String__ctor__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__append__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__append__string(ptr, ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
