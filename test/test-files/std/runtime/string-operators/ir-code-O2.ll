; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long.0 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.1 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.4 = type { ptr, i64, i64 }
%_s__String__charptr_long_long.5 = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 1
@3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@4 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@5 = private unnamed_addr constant [12 x i8] c"String: %d\0A\00", align 1
@6 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@7 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %0 = alloca %_s__String__charptr_long_long.0, align 8
  call void @_mp__String__ctor__string_string(ptr nonnull %0, ptr nonnull @0, ptr nonnull @1)
  %1 = alloca ptr, align 8
  store ptr %0, ptr %1, align 8
  %2 = call ptr @_mf__String__getRaw(ptr nonnull %1)
  %.unpack = load ptr, ptr %2, align 8
  %3 = insertvalue %_s__String__charptr_long_long.1 undef, ptr %.unpack, 0
  %.elt4 = getelementptr inbounds %_s__String__charptr_long_long.1, ptr %2, i64 0, i32 1
  %.unpack5 = load i64, ptr %.elt4, align 8
  %4 = insertvalue %_s__String__charptr_long_long.1 %3, i64 %.unpack5, 1
  %.elt6 = getelementptr inbounds %_s__String__charptr_long_long.1, ptr %2, i64 0, i32 2
  %.unpack7 = load i64, ptr %.elt6, align 8
  %5 = insertvalue %_s__String__charptr_long_long.1 %4, i64 %.unpack7, 2
  %6 = call i32 (ptr, ...) @printf(ptr nonnull @2, %_s__String__charptr_long_long.1 %5)
  %7 = alloca %_s__String__charptr_long_long.4, align 8
  call void @_mp__String__ctor__string_string(ptr nonnull %7, ptr nonnull @0, ptr nonnull @1)
  %8 = alloca ptr, align 8
  store ptr %7, ptr %8, align 8
  %9 = call ptr @_mf__String__getRaw(ptr nonnull %8)
  %.unpack8 = load ptr, ptr %9, align 8
  %10 = insertvalue %_s__String__charptr_long_long.5 undef, ptr %.unpack8, 0
  %.elt9 = getelementptr inbounds %_s__String__charptr_long_long.5, ptr %9, i64 0, i32 1
  %.unpack10 = load i64, ptr %.elt9, align 8
  %11 = insertvalue %_s__String__charptr_long_long.5 %10, i64 %.unpack10, 1
  %.elt11 = getelementptr inbounds %_s__String__charptr_long_long.5, ptr %9, i64 0, i32 2
  %.unpack12 = load i64, ptr %.elt11, align 8
  %12 = insertvalue %_s__String__charptr_long_long.5 %11, i64 %.unpack12, 2
  %13 = call i32 (ptr, ...) @printf(ptr nonnull @2, %_s__String__charptr_long_long.5 %12)
  %14 = call i32 (ptr, ...) @printf(ptr nonnull @5, i32 zext (i1 icmp eq (ptr @3, ptr @4) to i32))
  %15 = call i32 (ptr, ...) @printf(ptr nonnull @5, i32 zext (i1 icmp eq (ptr @6, ptr @7) to i32))
  %16 = call i32 (ptr, ...) @printf(ptr nonnull @5, i32 zext (i1 icmp ne (ptr @3, ptr @4) to i32))
  %17 = call i32 (ptr, ...) @printf(ptr nonnull @5, i32 zext (i1 icmp ne (ptr @6, ptr @7) to i32))
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare void @_mp__String__ctor__string_string(ptr, ptr, ptr) local_unnamed_addr

declare ptr @_mf__String__getRaw(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
